#ifndef STRACE_H
#define STRACE_H

#include "syscalls.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <string.h>


/* task 0 */
int traceSyscalls(pid_t childPid);
int executeAndTrace(const char *command, char *const args[]);

/* task 1 + task 2 */
int traceProcess(char *args[], char *env[]);
int waitSyscall(pid_t child);

/* task 3 */
void print_params(struct user_regs_struct *regs);
int trace_loop(pid_t child_pid);


#endif /* STRACE_H */