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


void list_directory(const char *dir, int op_a, int op_l);
void permissions(struct stat file_stat);


#endif /* MAIN_H */
