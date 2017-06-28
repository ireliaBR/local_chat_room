#include <stdio.h>
#include <file_operation.h>
#include <protocol.h>
#include <users.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 200
#define PUB_FIFO "PUB_FIFO"

void eventloop(int pub_fd);
void login_event(Login l);
void msg_event(Msg msg);
void logout_event(Logout logout);

int main(int argc, char *argv[])
{
    int pub_fd;
    int fileIsExists = file_is_exists(PUB_FIFO);
    if(fileIsExists == -1){
        mkfifo_file(PUB_FIFO);     
    }
    pub_fd = open_file(PUB_FIFO, 1);

    eventloop(pub_fd);
    
    unlink(PUB_FIFO);
    close(pub_fd);
    return 0;
}

void eventloop(int pub_fd)
{
    char buf[BUF_SIZE];

    while(1){
        int n = read(pub_fd, buf, BUF_SIZE);
        if(n != 200){
            continue;
        }
        int num = get_protocolnum(buf, n);
        switch(num)
        {
            case 1:
                {
                    Login login = generator_loginpack(buf, n);
                    login_event(login);
                }
                break;
            case 2:
                {
                    Msg msg = generator_msgpack(buf, n);
                    msg_event(msg); 
                }
                break;
            case 4:
                {
                    Logout logout = generator_logoutpack(buf, n);
                    logout_event(logout);
                }
                break;
        }
    }
}

void login_event(Login l)
{
    int re;
    int push_fd;
    User *user;
    Nononline msg;
    msg.num = 3;
    re = user_islogin(l.name, &user);
    if(re == 1){
        printf("user %s login success\n", l.name);
        strcpy(msg.desc, "login success");
        write_protocol(user->fd, (char*)&msg, sizeof(msg));
        return;
    }

    int fileIsExists = file_is_exists(l.name);
    if(fileIsExists == -1){
        mkfifo_file(l.name);     
    }
    push_fd = open_file(l.name, 1);

    re = login_user(l.name, push_fd, &user);

    if(re == 0){
        printf("user %s login success\n", l.name);
        strcpy(msg.desc, "login success");
    }else{
        printf("user %s login fail\n", l.name);
        strcpy(msg.desc, "login fail");
    }
        write_protocol(user->fd, (char*)&msg, sizeof(msg));
}
void msg_event(Msg msg)
{
    int re;
    User *sender;
    User *receiver;
    re = user_islogin(msg.sender, &sender);
    if(re != 1){
        return;
    }
    re = user_islogin(msg.receiver, &receiver);
    if(re == -1){
        Nononline line;
        line.num = 3;
        printf("user %s not exits\n", msg.receiver);
        strcpy(line.desc, "user not exists");
        write_protocol(sender->fd, (char*)&line, sizeof(line));
        return;
    }
    if(re == 0){
        Nononline line;
        line.num = 3;
        printf("user %s logout\n", msg.receiver);
        strcpy(line.desc, "user logout");
        write_protocol(receiver->fd, (char*)&line, sizeof(line));
        return;
    }

    write_protocol(receiver->fd, (char*)&msg, sizeof(msg));
}
void logout_event(Logout logout)
{
    int re;
    User *user;
    re = user_islogin(logout.name, &user);
    if(re != 1){
        return;
    }
    logout_user(logout.name);

    Nononline msg;
    msg.num = 3;
    printf("user %s logout success\n", logout.name);
    strcpy(msg.desc, "logout success");
    unlink(user->name);
    close(user->fd);
}
