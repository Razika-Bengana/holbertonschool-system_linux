#include "main.h"

/**
 * list_directory - program that list the files and directories in a specified directory
 *
 * this function takes a directory path as input and provides options to display its contents;
 * it opens the specified directory, reads its contents, and prints them to the console;
 * it can filter out hidden files based on the 'op_a' parameter and format the output as a list if 'op_l' is set;
 * if the directory is not found or not readable, appropriate error messages are displayed
 *
 * @dir: a pointer to a string representing the directory to list
 * @op_a: an indicator (0 or 1) to specify whether to display hidden files (starting with '.')
 * @op_l: an indicator (0 or 1) to specify whether to display the results in list mode (one entry per line)
 *
 * Return: nothing (void)
 */

void list_directory(const char *dir, int op_a, int op_l)
{
    struct dirent *d;
    struct stat file_stat;
    char filepath[512] = "";
    char nlink_str[20] = "";
    char size_str[20] = "";
    char *ctime_str;

    DIR *dh = opendir(dir);

    if (!dh)
    {
        if (errno == ENOENT)
        {
            perror("Directory doesn't exist");
        }
        else
        {
            perror("Unable to read directory");
        }
        exit(EXIT_FAILURE);
    }

    while ((d = readdir(dh)) != NULL)
    {
        filepath[0] = '\0';

        if (!op_a && d->d_name[0] == '.')
            continue;

        my_strcat(filepath, dir);
        my_strcat(filepath, "/");
        my_strcat(filepath, d->d_name);

        if (lstat(filepath, &file_stat) == -1)
        {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        if (op_l)
        {
            permissions(file_stat);

            my_itoa((long) file_stat.st_nlink, nlink_str);
            my_itoa((long) file_stat.st_size, size_str);

            ctime_str = ctime(&file_stat.st_mtime);
            ctime_str[my_strlen(ctime_str) - 1] = '\0';  /* Remove the trailing newline */

            printf(" %s %s %s %s %s %s\n",
                   nlink_str,
                   "root", "root",
                   size_str,
                   ctime_str,
                   d->d_name);
        }
        else
        {
            printf("%s  ", d->d_name);
        }
    }

    if(!op_l)
    {
        printf("\n");
    }

    closedir(dh);
}