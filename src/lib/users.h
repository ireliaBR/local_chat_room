
#ifndef USERS_H_
#define USERS_H_


struct User_struct{
    char name[36];
    int fd;
    int islogin;//0 logout; 1 login;
};

typedef struct User_struct User;


int login_user(const char *name, int fd, User **user);
int logout_user(const char *name);
/*
 * return 1 login; 0 logout; -1 not exist;
 */
int user_islogin(const char *name, User **user);

#endif
