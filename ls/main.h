#ifndef MAIN_H
#define MAIN_H


#ifndef PATH_MAX
#define PATH_MAX 4096
#endif


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>

struct Options {
    int op_a;
    int op_l;
    int op_1;
    int op_A;
};

void list_directory(const char *program_name, const char *dir, struct Options opts);
void permissions(struct stat file_stat);
size_t my_strlen(const char *str);
int my_strcmp(const char *str1, const char *str2);
char *my_strncpy(char *dest, const char *src, size_t n);


#endif /* MAIN_H */
