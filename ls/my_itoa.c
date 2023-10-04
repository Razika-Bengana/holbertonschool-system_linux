#include "main.h"

void my_itoa(long num, char *str)
{
    char temp[20];
    int i = 0;

    do {
        temp[i++] = '0' + (num % 10);
        num /= 10;
    } while (num);

    while (i)
    {
        *str++ = temp[--i];
    }
    *str = '\0';
}
