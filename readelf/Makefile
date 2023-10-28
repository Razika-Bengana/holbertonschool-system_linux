CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99
DEBUGFLAGS = -g

TARGET_0 = 0-hreadelf
TARGET_1 = 1-hreadelf
SOURCE_0 = 0-elf_header-main.c 0-elf_header-details.c 0-elf_header-machine.c 0-elf_header-type.c 0-elf_header-abi_version.c 0-elf_header-os_abi.c 0-elf_header-version.c 0-elf_header-data.c 0-elf_header-class.c 0-elf_header-magic.c 0-elf_header-read.c 0-elf_header-endian.c
SOURCE_1 = 1-elf_sec_header-main.c 1-elf_sec_header-flags.c 1-elf_sec_header-handle.c 1-elf_sec_header-print.c 1-elf_sec_header-swap.c 1-elf_sec_header-type.c

all: $(TARGET_0) $(TARGET_1)


debug: CFLAGS += $(DEBUGFLAGS)
debug: all


$(TARGET_0): $(SOURCE_0)
	$(CC) $(CFLAGS) -o $(TARGET_0) $(SOURCE_0)

$(TARGET_1): $(SOURCE_1)
	$(CC) $(CFLAGS) -o $(TARGET_1) $(SOURCE_1)


clean:
	rm -f $(TARGET)
	rm -f *.o
	rm -f *.out
	rm -f *~


.PHONY: all clean debug
