#include "main.h"

void my_strcat(char *dest, const char *src)
{
    while (*dest) dest++;
    while ((*dest++ = *src++));
}