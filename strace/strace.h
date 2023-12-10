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


#endif /* STRACE_H */