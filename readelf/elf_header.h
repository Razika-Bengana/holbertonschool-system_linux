#ifndef ELF_HEADER_H
#define ELF_HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>


/* task 0: ELF file header */
void swap_bytes(void *value, size_t size);
int system_is_little_endian(void);
int system_is_big_endian(void);

void read_elf_header(FILE *file, void *header, int is_elf64);
void print_magic(void *header, int is_elf64);
void print_class(void *header, int is_elf64);
void print_data(void *header, int is_elf64);
void print_version(void *header, int is_elf64);
void print_os_abi(void *header, int is_elf64);
void print_abi_version(void *header, int is_elf64);
void print_type(void *header, int is_elf64);
void print_machine_type(void *header, int is_elf64);
void print_header_details(void *header, int is_elf64);

/* task 1: ELF sections' headers */


#endif /* ELF_HEADER_H */
