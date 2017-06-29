#include "protocol.h"
#include <string.h>
#include <stdlib.h>


/*
 * more than 0 mean is protocol num
 * on error -1;
 */
int get_protocolnum(char *data, int count)
{
    if(count != 200){
        return -1;
    }
    int *num = (int*)data;
    if(*num <1 || *num > 4){
        return -1;
    }
    return *num;
}

Login generator_loginpack(char *data, int count)
{
    if(get_protocolnum(data, count) == -1){     
        exit(2);
    }
    Login *l = (Login*)data;
    return *l; 
}

Msg generator_msgpack(char *data, int count)
{
    if(get_protocolnum(data, count) == -1){     
        exit(2);
    }
    Msg *l = (Msg*)data;
    return *l; 
}

Nononline generator_onlinepack(char *data, int count)
{
    if(get_protocolnum(data, count) == -1){     
        exit(2);
    }
    Nononline *l = (Nononline*)data;
    return *l; 
}

Logout generator_logoutpack(char *data, int count)
{
    if(get_protocolnum(data, count) == -1){     
        exit(2);
    }
    Logout *l = (Logout*)data;
    return *l; 
}
