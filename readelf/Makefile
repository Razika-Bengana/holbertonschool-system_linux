CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99
DEBUGFLAGS = -g

TARGET_0 = 0-hreadelf
TARGET_1 = 1-hreadelf
SOURCE_0 = elf_header-main.c elf_header-details.c elf_header-machine.c elf_header-type.c elf_header-abi_version.c elf_header-os_abi.c elf_header-version.c elf_header-data.c elf_header-class.c elf_header-magic.c elf_header-read.c elf_header-endian.c
SOURCE_1 = elf_sec_header.c

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
