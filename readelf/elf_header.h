#ifndef ELF_HEADER_H
#define ELF_HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>


/* task 0: ELF file header */
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


#endif /* ELF_HEADER_H */
