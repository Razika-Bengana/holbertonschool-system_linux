#include "main.h"

/**
 * main - the entry point
 *
 * program for a custom command-line utility to list files and directories
 *
 * this function processes command-line arguments and calls the 'list_directory' function
 * to display the contents of the specified directory with optional options
 *
 * @argc: the number of command-line arguments
 * @argv: an array of strings representing the command-line arguments
 *
 * if no command-line arguments are provided, it lists the contents of the current directory without options;
 * if one command-line argument is provided, it checks if it starts with a hyphen '-' to interpret it as options;
 *   - 'a' option: display hidden files;
 *   - 'l' option: display the results in list mode (one entry per line);
 * Ii an invalid option is encountered, it displays an error message and exits with an error code
 *
 * Return: nothing (void)
 */

int main(int argc, const char *argv[])
{
    struct Options opts = {0, 0, 0, 0};  /* op_a, op_l, op_1, op_A */
    struct stat path_stat;
    int dir_count = 0;
    int i;
    char *p;
    DIR *dh = NULL;

    int firstDir = 1; /* Variable to track if this is the first output */

    int file_count = 0; /* Adding a counter for files */

    int firstFile = 1;

    int needNewLine = 0;

    /* Count the number of directories */
    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] != '-')
        {
            dir_count++;
        }
    }

    /* Processing options */
    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            p = (char *) (argv[i] + 1);

            while (*p)
            {
                if (*p == 'a')
                    opts.op_a = 1;
                else if (*p == 'l')
                    opts.op_l = 1;
                else if (*p == 'A')
                    opts.op_A = 1;
                else if (*p == '1')
                    opts.op_1 = 1;
                else
                {
                    fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], *p);
                    return (EXIT_FAILURE);
                }
                p++;
            }
        }
    }
    /* If no directory is provided, list the current directory */
    if (dir_count == 0)
    {
        list_directory(argv[0], ".", opts);
        return (0);
    }

    /* Processing files first */
    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] != '-')
        {
            if (lstat(argv[i], &path_stat) == -1)
            {
                fprintf(stderr, "%s: cannot access %s: No such file or directory\n", argv[0], argv[i]);
                continue;
            }

            if (S_ISREG(path_stat.st_mode)) /* It's a file */
            {
                if (needNewLine)
                {
                    printf("\n");
                }
                printf("%s\n", argv[i]);
                needNewLine = 1;
                file_count++;
            }
        }
    }

    /* Add a newline if files have already been displayed */
    if (file_count > 0)
    {
        printf("\n");
    }

    /* Processing directories afterwards */
    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] != '-')
        {
            if (lstat(argv[i], &path_stat) == -1)
            {
                continue;
            }

            if (S_ISDIR(path_stat.st_mode)) /* It's a directory */
            {
                if (needNewLine)
                {
                    printf("\n");
                }
                printf("%s:\n", argv[i]);
                needNewLine = 1;

                if (dir_count > 1 || (file_count > 0 && firstDir))
                {
                    if (firstDir)
                    {
                        firstDir = 0;
                    }
                    else
                    {
                        if (firstFile == 0) /* Si des fichiers ont déjà été affichés, ne pas ajouter de ligne vide */
                        {
                            firstFile = 1;  /* Réinitialise pour les prochains fichiers */
                        }
                        else
                        {
                            printf("\n");  /* Ligne vide ajoutée entre les répertoires */
                        }
                    }
                    printf("%s:\n", argv[i]);
                }

                dh = opendir(argv[i]);

                if (dh == NULL)
                {
                    if (errno == ENOENT)
                    {
                        fprintf(stderr, "%s: cannot access %s: No such file or directory\n", argv[0], argv[i]);
                    }
                    else if (errno == EACCES)
                    {
                        fprintf(stderr, "%s: cannot open directory %s: Permission denied\n", argv[0], argv[i]);
                    }
                    else
                    {
                        fprintf(stderr, "%s: cannot access %s: ", argv[0], argv[i]);
                        perror("");
                    }
                    continue;
                }

                list_directory(argv[0], argv[i], opts);

                closedir(dh);
                dh = NULL;

            }
        }
    }

    return (0);
}