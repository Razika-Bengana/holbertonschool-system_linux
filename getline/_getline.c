#include "_getline.h"

#define BUFFER_SIZE 1024

char *_getline(const int fd)
{
    size_t buffer_len = 0;
    size_t buffer_size = BUFFER_SIZE;
    ssize_t bytes_read;
    char *buffer = malloc(buffer_size);
    char *new_buffer;
    char c;

    if (!buffer)
    {
        return (NULL);
    }
    while ((bytes_read = read(fd, &c, 1)) > 0)
    {
        if (buffer_len >= buffer_size)
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

        buffer[buffer_len] = c;
        buffer_len++;

        if (c == '\n')
        {
            buffer[buffer_len -1] = '\0';
            break;
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
