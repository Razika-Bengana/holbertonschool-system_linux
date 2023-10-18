#include "main.h"

/**
 * list_directory - program that lists the files and directories in a specified directory
 *
 * this function takes a directory path as input and provides options to display its contents:
 * - it opens the specified directory, reads its contents, and prints them to the console;
 * - it can filter out hidden files (those starting with '.') based on the 'op_a' parameter;
 * - it can format the output as a list if 'op_l' is set;
 * - it can display each entry on a separate line if 'op_1' is set;
 * - it can show all entries except for '.' and '..' if 'op_A' is set;
 * - it handles errors appropriately, such as when the directory is not found or not readable;
 *
 * @program_name: a pointer to a string representing the name of the program invoking this function
 * @dir: a pointer to a string representing the directory to list
 * @opts: a struct containing options for listing, including:
 *        - op_a (hidden files),
 *        - op_l (list format),
 *        - op_1 (one entry per line),
 *        - op_A (show all except for '.' and '..');
 *
 * Return: nothing (void)
 */

void list_directory(const char *program_name, const char *dir, struct Options opts)
{
    struct dirent *d;
    struct stat file_stat;
    char filepath[PATH_MAX]; /* For portability */
    char nlink_str[20] = "";
    char size_str[20] = "";
    char *ctime_str = NULL;
    char formatted_ctime[30];

    DIR *dh = opendir(dir);

    if (dh == NULL)
    {
        perror(program_name);
        return;
    }
    while ((d = readdir(dh)) != NULL)
    {
        /* Skip hidden files if neither -a nor -A is set */
        if (d->d_name[0] == '.' && !opts.op_a && !opts.op_A)
            continue;

        /* Show all except for '.' and '..' if -A is set */
        if (opts.op_A && (my_strcmp(d->d_name, ".") == 0 || my_strcmp(d->d_name, "..") == 0))
            continue;

        sprintf(filepath, "%s/%s", dir, d->d_name);

        if (lstat(filepath, &file_stat) == -1)
        {
            perror("lstat");
            continue;
        }

        if (opts.op_1)
        {
            printf("%s\n", d->d_name);
        }

        else if (opts.op_l)
        {
            struct passwd *pw = getpwuid(file_stat.st_uid);
            struct group  *gr = getgrgid(file_stat.st_gid);

            const char *username = pw ? pw->pw_name : NULL;
            const char *groupname = gr ? gr->gr_name : NULL;

            char uid_str[20];
            char gid_str[20];

            if (username)
            {
                my_strncpy(uid_str, username, sizeof(uid_str));
                uid_str[sizeof(uid_str) - 1] = '\0';
            }
            else
            {
                sprintf(uid_str, "%ld", (long) file_stat.st_uid);
            }

            if (groupname)
            {
                my_strncpy(gid_str, groupname, sizeof(gid_str));
                gid_str[sizeof(gid_str) - 1] = '\0';
            }
            else
            {
                sprintf(gid_str, "%ld", (long) file_stat.st_gid);
            }

            permissions(file_stat);

            sprintf(nlink_str, "%ld", (long) file_stat.st_nlink);
            sprintf(size_str, "%ld", (long) file_stat.st_size);

            ctime_str = ctime(&file_stat.st_mtime);
            ctime_str[my_strlen(ctime_str) - 1] = '\0';  /* Remove the trailing newline */

            /* To format the date */
            my_strncpy(formatted_ctime, &ctime_str[4], 12);
            formatted_ctime[12] = '\0';

            printf(" %s %s %s %s %s %s\n",
                   nlink_str,
                   uid_str,
                   gid_str,
                   size_str,
                   formatted_ctime,
                   d->d_name);
        }
        else
        {
            printf("%s  ", d->d_name);
        }
    }

    if(!opts.op_l && !opts.op_1)
    {
        printf("\n");
    }

    closedir(dh);
}