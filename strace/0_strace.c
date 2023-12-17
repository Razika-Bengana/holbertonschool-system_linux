#include "strace.h"

/**
 * execute_and_trace_process - program that executes and traces a child process
 *
 * this function is responsible for setting up tracing for a child process
 * and executing a given command;
 * it also handles errors during execution
 *
 * @command_args: argument vector for execve
 * @environment: environment for execve
 *
 * Return: nothing (void)
 */

void execute_and_trace_process(char **command_args, char **environment)
{
	ptrace(PTRACE_TRACEME, 0, 0, 0);
	kill(getpid(), SIGSTOP);

	if (execve(command_args[1], command_args + 1, environment) == -1)
	{
		fprintf(stderr, "Exec failed: %d\n", errno);
		exit(EXIT_FAILURE);
	}
}



/**
 * monitor_and_trace_syscalls - program that monitors and traces system calls of a child process
 *
 * this function handles tracing of system calls made by the child process;
 * it waits for system call events from the child and prints them
 *
 * @child_pid: PID of the child process to monitor
 *
 * Return: nothing (void)
 */

void monitor_and_trace_syscalls(pid_t child_pid)
{
	int status;
	struct user_regs_struct regs;
	int is_syscall_entry = 1;

	waitpid(child_pid, &status, 0);
	ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_TRACESYSGOOD);

	while (1)
	{
		ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
		waitpid(child_pid, &status, 0);

		if (WIFEXITED(status))
			break;

		if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
		{
			if (is_syscall_entry)
			{
				memset(&regs, 0, sizeof(regs));
				ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
				printf("%lu\n", (unsigned long)regs.orig_rax);
			}
			is_syscall_entry = !is_syscall_entry;
		}
	}
}

/**
 * main - the entry point
 *
 * this function is the starting point of the program;
 * it forks the process into a parent and child;
 * the child process executes the command, and the parent traces the child's system calls
 *
 * @argc: argument count
 * @argv: argument vector
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

	child_pid = fork();
	if (child_pid == -1)
	{
		fprintf(stderr, "Fork failed: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	else if (child_pid == 0)
	{
		execute_and_trace_process(argv, envp);
	}
	else
	{
		monitor_and_trace_syscalls(child_pid);
	}

	return (EXIT_SUCCESS);
}
