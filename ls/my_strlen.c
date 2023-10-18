#include "main.h"

/**
 * my_strlen - program that calculates the length (number of characters)
 * of the input string 'str'
 *
 * @str: the input string for which the length is to be calculated
 *
 * Return: the length of the input string 'str'
 */

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