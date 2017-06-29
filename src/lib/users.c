
#include "users.h"
#include <string.h>
#include <stdlib.h>

static User *users[100] = {
    0
};
static int count = 0;

User *get_user(const char *name)
{
    if(!name){
        return NULL;
    }
    int i;
    for(i=0; i<count; i++){
        User *ownUser = users[i];
        if(0 == strcmp(ownUser->name, name)){
            return ownUser;
        }
    }
    return NULL;
}

int login_user(const char *name, int fd, User **user)
{
    if(!name){
        return -1;
    }
    if(fd<1){
        return -1;
    }
    User *ownUser = get_user(name);
    if(!ownUser){
        ownUser = (User*)malloc(sizeof(User));
        strcpy(ownUser->name, name);
        users[count++]  = ownUser;
    }
    ownUser->islogin = 1;
    ownUser->fd = fd;
    *user = ownUser;
    return 0;
}

int logout_user(const char *name)
{
    if(!name){
        return -1;
    }
    User *ownUser = get_user(name);
    if(!ownUser){
        return -1;
    }
    ownUser->islogin = 0;
    ownUser->fd = -1;
    return 0;
}

/*
 * return 1 login; 0 logout; -1 not exist;
 */
int user_islogin(const char *name, User **user)
{
    User *ownUser = get_user(name);
    if(!ownUser){
        return -1;
    }
    *user = ownUser;
    return ownUser->islogin;

}
