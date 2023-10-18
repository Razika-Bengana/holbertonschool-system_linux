#include "main.h"

/**
 * my_strcmp - program that compares two input strings 'str1' and 'str2' lexicographically
 *
 * @str1: the first string to be compared
 * @str2: the second string to be compared
 *
 * Return: an integer less than, equal to, or greater than zero if 'str1'
 * is found to be less than, equal to, or greater than 'str2'
 * lexicographically, respectively.
 */

int my_strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            return (unsigned char)(*str1) - (unsigned char)(*str2);
        }
        str1++;
        str2++;
    }
    return (unsigned char)(*str1) - (unsigned char)(*str2);
}