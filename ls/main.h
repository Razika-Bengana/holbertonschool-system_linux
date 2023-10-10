#ifndef MAIN_H
#define MAIN_H


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>


void list_directory(const char *program_name, const char *dir, int op_a, int op_l);
void permissions(struct stat file_stat);
void my_strcat(char *dest, const char *src);
void my_itoa(long num, char *str);
size_t my_strlen(const char *str);


#endif /* MAIN_H */
