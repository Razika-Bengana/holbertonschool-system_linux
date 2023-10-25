#ifndef ELF_HEADER_H
#define ELF_HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>


/* task 0: ELF file header */

void *initialize_elf_header(FILE *file, int *is_elf64_ptr);
void print_elf_header_details(void *header, int is_elf64);

void swap_bytes(void *value, size_t size);
int system_is_little_endian(void);
int system_is_big_endian(void);

void print_header_details_64(Elf64_Ehdr *header);
void print_header_details_32(Elf32_Ehdr *header);
void print_header_details(void *header, int is_elf64);

void swap_fields_64(Elf64_Ehdr *hdr64);
void swap_fields_32(Elf32_Ehdr *hdr32);
void print_data_encoding(unsigned char elf_data);
void print_data(void *header, int is_elf64);

void read_elf_header(FILE *file, void *header, int is_elf64);
void print_magic(void *header, int is_elf64);
void print_class(void *header, int is_elf64);
void print_version(void *header, int is_elf64);
void print_os_abi(void *header, int is_elf64);
void print_abi_version(void *header, int is_elf64);
void print_type(void *header, int is_elf64);
void print_machine_type(void *header, int is_elf64);


/* task 1: ELF sections' headers */
void print_flags(unsigned long flags, char *buf);
char* section_type_to_string(uint32_t type);


#endif /* ELF_HEADER_H */
