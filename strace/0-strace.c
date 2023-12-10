#include "strace.h"

/**
 * traceSyscalls - program that traces the system calls made by a child process
 *
 * @childPid: PID of the child process to trace
 *
 * Return: 0 on success, 1 on failure.
 */

int traceSyscalls(pid_t childPid)
{
	int status, isSyscallEntry;

	isSyscallEntry = 0;

	while (1)
	{
		if (wait(&status) == -1)
			return (1);

		if (WIFEXITED(status))
			break;

		if (!isSyscallEntry)
		{
			struct user_regs_struct regs;

			if (ptrace(PTRACE_GETREGS, childPid, NULL, &regs) == -1)
				return (1);

			printf("%lu\n", (unsigned long)regs.orig_rax);
			fflush(stdout);
		}

		if (ptrace(PTRACE_SYSCALL, childPid, NULL, NULL) == -1)
			return (1);

		isSyscallEntry = !isSyscallEntry;
	}

	return (0);
}

/**
 * executeAndTrace - program that executes a command and traces
 * its system calls
 *
 * @command: the command to execute
 * @args: the arguments for the command
 *
 * Return: 0 on success, 1 on failure
 */

int executeAndTrace(const char *command, char *const args[])
{
	pid_t childPid;

	switch (childPid = fork())
	{
		case -1:
			return (1);
		case 0:
			if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
				return (1);
			if (execvp(command, args) == -1)
				return (1);
		default:
			break;
	}

	return (traceSyscalls(childPid));
}

/**
 * main - the entry point of the strace_0 program
 *
 * @argc: the number of command-line arguments
 * @argv: an array of strings containing the command-line arguments
 *
 * Return: 0 on success, 1 on failure
 */

int main(int argc, char *argv[])
{
	if (argc < 2 || !argv)
	{
		fprintf(stderr, "usage: %s <prog> <prog args>...\n", argv[0]);
		return (1);
	}

	return (executeAndTrace(argv[1], argv + 1));
}