#include <stdio.h>
#include <string.h>


int main(void)
{
    char a[10] = {
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
    };
    char b[100] = {
        0x00
    };
    a[9] = 0x20;
    strncpy(b, a, 10);
    return 0;
}
