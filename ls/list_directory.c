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

void list_directory(const char *program_name, const char *dir, int op_a, int op_l)
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
            fprintf(stderr, "%s: %s: No such file or directory\n", program_name, dir);
        }

        else
        {
            fprintf(stderr, "%s: %s: Permission denied\n", program_name, dir);
        }
        exit(EXIT_FAILURE);
    }

    while ((d = readdir(dh)) != NULL)
    {

        if (!op_a && d->d_name[0] == '.')
            continue;

        sprintf(filepath, "%s/%s", dir, d->d_name);

        if (lstat(filepath, &file_stat) == -1)
        {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        if (op_l)
        {
            permissions(file_stat);

            sprintf(nlink_str, "%ld", (long) file_stat.st_nlink);
            sprintf(size_str, "%ld", (long) file_stat.st_size);

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