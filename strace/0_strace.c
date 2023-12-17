#include "strace.h"

void executeChildProcess(char **arguments, char **environment);
void monitorChildProcess(pid_t childProcessID);
int waitForSystemCall(pid_t childProcessID);

/**
 * main - the entry point
 *
 * @argumentCount: the count of arguments passed
 * @arguments: the argument vector
 * @environment: environment variables
 *
 * Return: EXIT_SUCCESS on success, otherwise EXIT_FAILURE on failure
 */

int main(int argumentCount, char **arguments, char **environment)
{
	pid_t childProcessID;

	if (argumentCount < 2)
	{
		printf("Usage: %s command [args...]\n", arguments[0]);
		return (EXIT_FAILURE);
	}

	setbuf(stdout, NULL);
	childProcessID = fork();

	if (childProcessID == -1)
	{
		fprintf(stderr, "Fork failed: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	else if (childProcessID == 0)
	{
		executeChildProcess(arguments, environment);
	}

	else
	{
		monitorChildProcess(childProcessID);
	}
	return (EXIT_SUCCESS);
}



/**
 * executeChildProcess - program that executes the child process
 *
 * this function sets up the tracing for the child process and executes the command
 *
 * @arguments: the argument vector for execve
 * @environment: the environment variables for execve
 *
 * Return: nothing (void)
 */

void executeChildProcess(char **arguments, char **environment)
{
	ptrace(PTRACE_TRACEME, 0, 0, 0);
	kill(getpid(), SIGSTOP);

	if (execve(arguments[1], arguments + 1, environment) == -1)
	{
		fprintf(stderr, "Exec failed: %d\n", errno);
		exit(EXIT_FAILURE);
	}
}



/**
 * monitorChildProcess - program that monitors the child process
 *
 * this function handles the tracing of system calls made by the child process
 *
 * @childProcessID: PID of the child process
 *
 * Return: nothing (void)
 */

void monitorChildProcess(pid_t childProcessID)
{
	int status;
	struct user_regs_struct registerValues;

	waitpid(childProcessID, &status, 0);
	ptrace(PTRACE_SETOPTIONS, childProcessID, 0, PTRACE_O_TRACESYSGOOD);

	while (1)
	{
		if (waitForSystemCall(childProcessID))
			break;

		memset(&registerValues, 0, sizeof(registerValues));
		ptrace(PTRACE_GETREGS, childProcessID, 0, &registerValues);
		printf("%lu\n", (long)registerValues.orig_rax);

		if (waitForSystemCall(childProcessID))
			break;
	}
}



/**
 * waitForSystemCall - program that waits for a system call from the child process
 *
 * this function pauses execution until a system call is made by the child process
 *
 * @childProcessID: PID of the child process
 *
 * Return: 0 if a system call is made, otherwise 1 if the child process exits
 */

int waitForSystemCall(pid_t childProcessID)
{
	int status;

	while (1)
	{
		ptrace(PTRACE_SYSCALL, childProcessID, 0, 0);
		waitpid(childProcessID, &status, 0);

		if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
			return (0);

		if (WIFEXITED(status))
			return (1);
	}
}
