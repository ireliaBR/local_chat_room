#include <sys/types.h>

#ifndef FILE_OPERATION_H_
#define FILE_OPERATION_H_

int open_file(const char *pathname, int isBlock);
int mkfifo_file(const char *pathname);
void close_file(int fd);
ssize_t read_file(int fd, void *buf, size_t count);
ssize_t write_file(int fd, const void *buf, size_t count);
int file_is_exists(const char *pathname);

ssize_t write_protocol(int fd, const char *buf, size_t count);

#endif
