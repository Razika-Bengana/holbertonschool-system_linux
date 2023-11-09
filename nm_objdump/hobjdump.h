#ifndef HOBJDUMP_H
#define HOBJDUMP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <byteswap.h>
#include <endian.h>
#include <stdint.h>


#define HAS_RELOC 0x01
#define EXEC_P    0x02
#define HAS_SYMS  0x10
#define D_PAGED   0x100
#define DYNAMIC   0x40


typedef union
{
    Elf32_Ehdr *elf32;
    Elf64_Ehdr *elf64;
} Elf_Ehdr;


void print_flags(uint32_t flags);
uint32_t setFileFlags(Elf_Ehdr header, void *section_headers, int is_64);

uint32_t convert_endianness32(uint32_t value, int is_little_endian, int is_big_endian);
uint64_t convert_endianness64(uint64_t value, int is_little_endian, int is_big_endian);

void print_section_content(FILE *file, void *shdr_generic, int is_64);

void process_elf_file(const char *filename);
void process_elf_header32(FILE *file, const char *filename);
void process_elf_header64(FILE *file, const char *filename);
void print_elf_info32(Elf32_Ehdr header, const char *filename, Elf32_Shdr *sections);
void print_elf_info64(Elf64_Ehdr header, const char *filename, Elf64_Shdr *sections);
void read_and_process_sections64(FILE *file, Elf64_Ehdr header);
void read_and_process_sections32(FILE *file, Elf32_Ehdr header);


#endif /* HOBJDUMP_H */
