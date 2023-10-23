CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99
DEBUGFLAGS = -g
TARGET = 0-hreadelf
SOURCE = elf_header.c


all: $(TARGET)


debug: CFLAGS += $(DEBUGFLAGS)
debug: all


$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)


clean:
	rm -f $(TARGET)
	rm -f *.o
	rm -f *.out
	rm -f *~


.PHONY: all clean debug test
