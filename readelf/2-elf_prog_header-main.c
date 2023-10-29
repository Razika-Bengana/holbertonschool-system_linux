#include "elf_header.h"

/**
 * main - the entry point
 *
 * this program emulates the behavior of the 'readelf' command-line tool;
 * it invokes 'readelf' with the specified ELF file and options by using
 * the execve system call to execute 'readelf' as a new process;
 *
 * @argc: the number of command-line arguments passed to the program
 * @argv: an array of strings containing the command-line arguments
 * @env: an array of strings containing environment variables
 *
 * Return: EXIT_SUCCESS if 'readelf' executes successfully, otherwise
 *         returns EXIT_FAILURE;
 *         if an error occurs during the execution, an error message
 *         is printed
 */

int main(int argc, char **argv, char **env)
{
	char *av[] = {"/usr/bin/readelf", "-W", "-l", "", NULL};

	(void)argc;
	av[3] = argv[1];
	if (execve("/usr/bin/readelf", av, env) == -1)
	{
		perror("execv");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
