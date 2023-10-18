#include "main.h"

/**
 * my_strncpy - program that copies at most 'n' characters from the 'src' string to the 'dest' string,
 * ensuring the 'dest' string is null-terminated
 *
 * if 'n' is greater than or equal to the length of 'src', it copies the entire 'src' string
 *
 * @dest: the destination buffer to store the copied string
 * @src: the source string to be copied from
 * @n: the maximum number of characters to copy
 *
 * Return: a pointer to the destination string 'dest'
 */

char *my_strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; ++i)
    {
        dest[i] = src[i];
    }

    for (; i < n; ++i)
    {
        dest[i] = '\0';
    }
    return (dest);
}