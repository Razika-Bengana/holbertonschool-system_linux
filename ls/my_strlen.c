#include "main.h"

size_t my_strlen(const char *str)
{
    size_t length = 0;

    while (*str != '\0')
    {
        length++;
        str++;
    }
    return (length);
}