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
    char filepath[512];
    DIR *dh = opendir(dir);

    if (!dh)
    {
        if (errno == ENOENT)
        {
            /* If the directory is not found */
            perror("Directory doesn't exist");
        }
        else
        {
            /* If the directory is not readable, then print the error and exit */
            perror("Unable to read directory");
        }
        exit(EXIT_FAILURE);
    }

    /* While the next entry is not readable, we will print directory files */
    while ((d = readdir(dh)) != NULL)
    {
        /* If hidden files are found we continue */
        if (!op_a && d->d_name[0] == '.')
            continue;

        snprintf(filepath, sizeof(filepath), "%s/%s", dir, d->d_name);
        if (stat(filepath, &file_stat) == -1)
        {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (op_l)
        {
            permissions(file_stat);

            char buff[20];
            struct tm *timeinfo = localtime(&(file_stat.st_mtime));
            strftime(buff, 20, "%b %d %H:%M", timeinfo);

            printf(" %ld %s %s %ld %s %s\n",
                   (long) file_stat.st_nlink,
                   "root", "root", // You may want to get actual user and group here
                   (long) file_stat.st_size,
                   buff,
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