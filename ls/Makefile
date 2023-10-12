# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -Werror -Wextra -std=c99

# Source files and object files
SRCS = hls.c list_directory.c permissions.c my_strlen.c my_strcmp.c my_strncpy.c
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = hls

# Build the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

# Build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(EXEC)
