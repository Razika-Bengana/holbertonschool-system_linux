#ifndef MAIN_H
#define MAIN_H


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

struct dirent *d;


void list_directory(const char *dir, int op_a, int op_l);


#endif /* MAIN_H */
