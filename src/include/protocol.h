
#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/*
 * protocol desc
 * 
 * ***********************************************
 * *              *        *          *          *
 * * protocol num * sender * receiver *   data   *
 * *      4B      *  36B   *   36B    *   124B   *
 * *              *        *          *          *
 * ***********************************************
 *
 * total 4+36+36+124 = 200B
 *
 * protocol num:
 *  1:login
 *  2:msg
 *  3:user not online
 *  4:logout
 *
 */


//protocol num 1
typedef struct {
    int num;
    char name[36];
} Login;

//protocol num 2
typedef struct {
    int num;
    char sender[36];
    char receiver[36];
    char data[124];
} Msg;

//protocol num 3
typedef struct {
    int num;
    char desc[196];
} Nononline;

//protocol num 4
typedef struct {
    int num;
    char name[36];
} Logout;

/*
 * more than 0 mean is protocol num
 * on error -1;
 */
int get_protocolnum(char *data, int count);
Login generator_loginpack(char *data, int count);
Msg generator_msgpack(char *data, int count);
Nononline generator_onlinepack(char *data, int count);
Logout generator_logoutpack(char *data, int count);

#endif
