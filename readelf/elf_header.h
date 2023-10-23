#ifndef ELF_HEADER_H
#define ELF_HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>

/* task 0: ELF file header */
void read_elf_header(int fd, Elf64_Ehdr *header);
void print_magic(Elf64_Ehdr *header);
void print_class(Elf64_Ehdr *header);
void print_data(Elf64_Ehdr *header);
void print_type(Elf64_Ehdr *header);
void print_machine_type(Elf64_Ehdr *header);
void print_header_details(Elf64_Ehdr *header);


#endif /* ELF_HEADER_H */
