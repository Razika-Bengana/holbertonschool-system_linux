CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic


# task 0
0_strace.o: 0_strace.c
	$(CC) -c $(CFLAGS) 0_strace.c

strace_0: syscalls.h 0_strace.o
	$(CC) $(CFLAGS) syscalls.h 0_strace.o -o strace_0


clean:
	rm -f strace_0