#include <stdio.h>
#include <file_operation.h>
#include <protocol.h>

#define BUF_SIZE 200

int main(int argc, char *argv[])
{
    char buf[BUF_SIZE] = {
        0x01,
        0x00,
        0x00,
        0x00,
        'a',
        'c',
        'b'
    };
    int num = get_protocolnum(buf, BUF_SIZE);
    switch(num){
        case 1:
            {
                Login l = generator_loginpack(buf, BUF_SIZE);
                printf("success\n");
            }
            break;
        case 2:
            {
                Msg m = generator_msgpack(buf, BUF_SIZE);
                printf("success\n");
            }
            break;
        case 3:
            {
                Nononline n = generator_onlinepack(buf, BUF_SIZE);
                printf("success\n");
            }
            break;
        case 4:
            {
                Logout l = generator_logoutpack(buf, BUF_SIZE);
                printf("success\n");
            }
            break;
    }
    return 0;
}
