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
    int read_count = 0;
    size_t buffer_len = 0;
    size_t buffer_size = READ_SIZE;
    ssize_t bytes_read;
    char *buffer = malloc(buffer_size);
    char *new_buffer;
    char temp_buffer[READ_SIZE];
    int i;

    if (!buffer)
    {
        return (NULL);
    }

    while ((bytes_read = read(fd, temp_buffer, READ_SIZE)) > 0)
    {
        read_count++;
        if (buffer_len + bytes_read >= buffer_size)
        {
            buffer_size *= 2;
            new_buffer = realloc(buffer, buffer_size);

            if (!new_buffer)
            {
                free(buffer);
                return (NULL);
            }

            buffer = new_buffer;
        }
        memcpy(buffer + buffer_len, temp_buffer, bytes_read);
        buffer_len += bytes_read;

        for (i = 0; i < bytes_read; i++)
        {
            if (temp_buffer[i] == '\n')
            {
                buffer[buffer_len - 1] = '\0';
                break;
            }
        }
    }

    if (bytes_read <= 0 && buffer_len == 0)
    {
        free(buffer);
        return (NULL);
    }

    buffer[buffer_len] = '\0';

    return (buffer);
}
