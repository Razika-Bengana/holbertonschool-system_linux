#include "_getline.h"

#define READ_SIZE 1024

/**
 * _getline - program that reads a line from a file descriptor and returns it as a string
 *
 * @fd: the file descriptor from which to read
 *
 * the function allocates memory for the string, which must be freed by the caller;
 * it reads one character at a time from the file descriptor, and dynamically
 * reallocates the buffer to hold the string;
 * the string will not include the trailing newline character;
 *
 * Return: a pointer to the string read, or NULL if an error occurs
 * or end-of-file is reached
 */

char *_getline(const int fd)
{
    static char *buffer = NULL;
    static size_t buffer_len = 0;
    static size_t buffer_size = 1024;
    static size_t read_pos = 0;
    size_t line_len = 0;
    char *line;

    if (buffer == NULL)
    {
        buffer = malloc(buffer_size);

        if (buffer == NULL)
            return NULL;
    }

    while (read_pos < buffer_len || (buffer_len = read(fd, buffer, buffer_size)) > 0)
    {
        if (buffer_len == buffer_size)
        {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);
            if (new_buffer == NULL)
            {
                free(buffer);
                return (NULL);
            }
            buffer = new_buffer;
        }

        for (; read_pos < buffer_len; ++read_pos)
        {
            if (buffer[read_pos] == '\n')
            {
                line = malloc(line_len + 1);
                if (line == NULL)
                    return (NULL);

                strncpy(line, buffer, line_len);
                line[line_len] = '\0';

                memmove(buffer, buffer + read_pos + 1, buffer_len - (read_pos + 1));
                buffer_len -= (read_pos + 1);
                read_pos = 0;

                return (line);
            }
            ++line_len;
        }
    }
    if (buffer_len <= 0)
    {
        free(buffer);
        buffer = NULL;
    }
    return (NULL);
}
