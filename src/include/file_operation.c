#include "file_operation.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int open_file(const char *pathname, int isBlock)
{
    int flags = O_RDWR;
    if(!isBlock){
        flags |= O_NONBLOCK;
    }
    int fd = open(pathname, flags);
    if(fd < 0){
        perror("open");
        exit(1);
    }
    return fd;
}

int mkfifo_file(const char *pathname)
{
    int fd = mkfifo(pathname, 00755);
    if(fd < 0){
        perror("mkfifo");
        exit(1);
    }
    return fd;
}

void close_file(int fd)
{
    close(fd);
}

ssize_t read_file(int fd, void *buf, size_t count)
{
    ssize_t n = read(fd, buf, count);
    if(n < 0){
        perror("read_file");
        exit(1);
    }
    return n;
}

ssize_t write_file(int fd, const void *buf, size_t count)
{
    ssize_t n = write(fd, buf, count);
    if(n < 0){
        perror("write");
        exit(1);
    }
    return n;
}

int file_is_exists(const char *pathname)
{
    return access(pathname, F_OK);
}

char * file_strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++)
        dest[i] = src[i];
    return dest;
}

ssize_t write_protocol(int fd, const char *buf, size_t count)
{
    char dest[200] = {
        0
    };
    file_strncpy(dest, buf, count);
    write_file(fd, dest, 200);
}
