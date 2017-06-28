#include <stdio.h>
#include <file_operation.h>
#include <protocol.h>
#include <users.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/types.h>

#define BUF_SIZE 200
#define PUB_FIFO "PUB_FIFO"

static char name[36] = {
    0x00
};

//return read_fd
int login(int pub_fd);
void read_event_loop(int read_fd);
void write_event_loop(int pub_fd);
void read_fd_event(char *buf, int count);
void write_fd_event(char *buf, int count, int pub_fd);

int main(void)
{
    int pub_fd;
    int read_fd;
    int fileIsExists = file_is_exists(PUB_FIFO);
    if(fileIsExists == -1){
        printf("server not exists\n");
        exit(1);
    }
    pub_fd = open_file(PUB_FIFO, 1);

    // stdin nonblock
    /*int flags = fcntl(STDIN_FILENO, F_GETFL);
    flags |= O_NONBLOCK;
    if(fcntl(STDIN_FILENO, F_SETFL, flags) == -1){
        perror("fcntl stdin");
        exit(1);
    }*/

    read_fd = login(pub_fd);

    pid_t pid = fork();
    if(pid == 0){
        read_event_loop(read_fd);
    }else{
        write_event_loop(pub_fd);
        kill(pid, 9);
        wait(NULL);
    }


    close(pub_fd);
    close(read_fd);

    return 0;
}


int login(int pub_fd)
{
    char buf[BUF_SIZE];
    int read_fd;
    printf("welcome come to chat\n");
    printf("please input loginname:\n");
    scanf("%s",buf);
    Login login;
    login.num = 1;
    strncpy(login.name, buf, strlen(buf)); 
    strcpy(name, buf);
    write_protocol(pub_fd, (char*)&login, sizeof(login));

    while(1){
        int re = file_is_exists(login.name);
        if(re == -1){
            sleep(1);
            continue;
        }

        read_fd = open_file(login.name, 1);
        break;
    }

    int n = read_file(read_fd, buf, BUF_SIZE);
    if(n != 200){
        perror("login return msg size not than 200");
        exit(1);
    }

    read_fd_event(buf, n);

    return read_fd;
}

void read_event_loop(int read_fd)
{
    // stdin nonblock
    int flags = fcntl(read_fd, F_GETFL);
    flags |= O_NONBLOCK;
    if(fcntl(read_fd, F_SETFL, flags) == -1){
        perror("fcntl stdin");
        exit(1);
    }
    char read_buf[BUF_SIZE];
    ssize_t read_n;
    while(1){
        read_n = read(read_fd, read_buf, BUF_SIZE);
        if(read_n < 0){
            if(errno == EAGAIN){
                sleep(1);
                continue;
            }
            perror("child read");
            exit(1);
        }
        read_fd_event(read_buf, read_n);
    }
}


void read_fd_event(char *buf, int count)
{
    if(count != 200){
        return;
    }

    int num = get_protocolnum(buf, count);
    switch(num)
    {
        case 2:
            {
                Msg msg = generator_msgpack(buf, count);
                printf("sender:%s, msg:%s\n", msg.sender, msg.data);
            }
            break;
        case 3:
            {
                Nononline nononline = generator_onlinepack(buf, count);
                printf("pid:%d, systemmsg: %s\n", getpid(), nononline.desc);
            }
            break;
    }
    
}

void write_event_loop(int pub_fd)
{
    char buf[BUF_SIZE];
    while(1){
        int num;
        printf("menu:\n");
        printf("\t2:send msg;\t4:logout\n");
        scanf("%d",&num);
        if(num == 2){
            Msg msg;
            msg.num = 2;
            strcpy(msg.sender, name);

            printf("please input receiver:\n");
            scanf("%s", buf);
            if(strlen(buf) > 36){
                perror("receiver");
                exit(1);
            }
            strcpy(msg.receiver, buf);

            printf("please input msg content:\n");
            scanf("%s", buf);
            if(strlen(buf) > 124){
                perror("msg content");
                exit(1);
            }
            strcpy(msg.data, buf);

            write_fd_event((char*)&msg, sizeof(msg), pub_fd);
            continue;
        }

        if(num == 4){
            Logout logout;
            logout.num = 4;
            strcpy(logout.name, name);

            write_fd_event((char*)&logout, sizeof(logout), pub_fd);
            printf("logout success\n");

            break;
        }

    }
}

void write_fd_event(char *buf, int count, int pub_fd)
{
    write_protocol(pub_fd, buf, count);
}
