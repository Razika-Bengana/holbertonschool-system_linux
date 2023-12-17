#include "strace.h"

/**
 * execute_trace - program that combines tracing of child and parent process
 *
 * this function handles both child and parent process tracing;
 * for the child, it sets up the tracing and executes the given command;
 * for the parent, it handles the syscall tracing and output
 *
 * @args: argument vector for execve
 * @env: environ for execve
 * @child_pid: pid of child, 0 if current process is child
 *
 * Return: nothing (void)
 */

void execute_trace(char **args, char **env, pid_t child_pid)
{
	int status;
	struct user_regs_struct regs;
	int in_syscall = 0;

	if (child_pid == 0)
	{  /* Child process */
		ptrace(PTRACE_TRACEME);
		kill(getpid(), SIGSTOP);

		if (execve(args[1], args + 1, env) == -1)
		{
			fprintf(stderr, "Exec failed: %d\n", errno);
			exit(EXIT_FAILURE);
		}
	}

	else
	{  /* Parent process */
		waitpid(child_pid, &status, 0);
		ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_TRACESYSGOOD);

		while (1)
		{
			ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
			waitpid(child_pid, &status, 0);

			if (WIFEXITED(status)) break;

			if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
			{
				if (in_syscall)
				{
					memset(&regs, 0, sizeof(regs));
					ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
					printf("%lu\n", (unsigned long)regs.orig_rax);
				}
				in_syscall = !in_syscall;
			}
		}
	}
}



/**
 * main - the entry point
 *
 * this function is the starting point of the program;
 * it initializes the process, decides if the current process is the parent or the child for tracing,
 * and then calls the "execute_trace" function to perform the actual tracing
 *
 * @argc: the argument count
 * @argv: the argument vector
 * @envp: environment variables
 *
 * Return: EXIT_SUCCESS on successful execution, otherwise EXIT_FAILURE
 */

int main(int argc, char **argv, char **envp)
{
	pid_t child_pid;

	if (argc < 2)
	{
		printf("Usage: %s command [args...]\n", argv[0]);
		return (EXIT_FAILURE);
	}

	setvbuf(stdout, NULL, _IONBF, 0);

	child_pid = fork();

	if (child_pid == -1)
	{
		fprintf(stderr, "Fork failed: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	execute_trace(argv, envp, child_pid);

	return (EXIT_SUCCESS);
}
