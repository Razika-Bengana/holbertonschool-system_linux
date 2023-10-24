CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99
DEBUGFLAGS = -g
TARGET = 0-hreadelf
SOURCE = elf_header-main.c elf_header-details.c elf_header-machine.c elf_header-type.c elf_header-abi_version.c elf_header-os_abi.c elf_header-version.c elf_header-data.c elf_header-class.c elf_header-magic.c elf_header-read.c elf_header-endian.c


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
