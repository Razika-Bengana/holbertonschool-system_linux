#include "_getline.h"

/**
 * buffer_append - program that appends a string to an existing buffer
 *
 * @buffer: the existing buffer to which the string will be appended
 * @append: the string to append to the buffer
 * @len: the length of the string to append
 *
 * this function takes an existing buffer and appends a given string to it;
 * it uses realloc to expand the existing buffer to fit the new content;
 *
 * Return: a pointer to the new buffer, or NULL if allocation fails
 */

char *buffer_append(char *buffer, char *append, size_t len)
{
    size_t buffer_len = buffer ? strlen(buffer) : 0;
    char *new_buffer = realloc(buffer, buffer_len + len + 1);
    if (!new_buffer)
    {
        return (NULL);
    }

    memcpy(new_buffer + buffer_len, append, len);
    new_buffer[buffer_len + len] = '\0';
    return (new_buffer);
}

/**
 * _getline - program that reads an entire line from a file descriptor
 *
 * @fd: the file descriptor from which to read
 *
 * this function reads lines from a file descriptor, returning each line
 * one at a time when the function is called multiple times;
 * it reads READ_SIZE bytes at a time from the file descriptor as specified by the macro;
 * the function also makes use of a static remainder buffer to keep track of any
 * remaining characters from the last read that did not yet form a complete line;
 *
 * Return: a null-terminated string that does not include the newline character,
 * or NULL if there are no more lines to return or if there's an error
 */

char *_getline(const int fd)
{
    static char *remainder;
    char *line = NULL;
    char buf[READ_SIZE];
    ssize_t bytes_read;
    size_t i, start;

    if (remainder)
    {
        line = strdup(remainder);
        free(remainder);
        remainder = NULL;
    }

    while ((bytes_read = read(fd, buf, READ_SIZE)) > 0)
    {
        start = 0;
        for (i = 0; i < (size_t)bytes_read; ++i)
        {
            if (buf[i] == '\n')
            {
                line = buffer_append(line, buf + start, i - start);
                remainder = strdup(buf + i + 1);
                return (line);
            }
        }
        line = buffer_append(line, buf + start, bytes_read - start);
    }

    if (bytes_read <= 0 && !line && !remainder)
    {
        return (NULL);
    }

    return (line);
}
