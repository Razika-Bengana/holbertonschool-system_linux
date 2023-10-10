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
    int op_a = 0, op_l = 0;
    int i;
    char *p;

    if (argc == 1)
    {
        list_directory(argv[0], ".", op_a, op_l);
        return (0);
    }

    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            p = (char *)(argv[i] + 1);

            while (*p)
            {
                if (*p == 'a')
                    op_a = 1;
                else if (*p == 'l')
                    op_l = 1;
                else
                {
                    fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], *p);
                    return (EXIT_FAILURE);
                }
                p++;
            }
        }
        else
        {
            DIR *dh = opendir(argv[i]);

            if (!dh)
            {
                perror(argv[0]);
                continue;
            }
            closedir(dh);
            list_directory(argv[0], argv[i], op_a, op_l);
        }
    }
    return (0);
}
