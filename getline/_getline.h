#ifndef _GETLINE_H
#define _GETLINE_H

#define READ_SIZE 4096

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *buffer_append(char *buffer, char *append, size_t len);
char *_getline(const int fd);


#endif /* _GETLINE_H */