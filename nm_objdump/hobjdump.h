#ifndef HOBJDUMP_H
#define HOBJDUMP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <byteswap.h>
#include <endian.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>


#define HAS_RELOC 0x01
#define EXEC_P    0x02
#define HAS_SYMS  0x10
#define D_PAGED   0x100
#define DYNAMIC   0x40

#define BSS_SECTION ".bss"
#define SHSTRTAB_SECTION ".shstrtab"
#define SYMTAB_SECTION ".symtab"
#define STRTAB_SECTION ".strtab"
#define TM_CLONE_TABLE_SECTION ".tm_clone_table"
#define REL_TEXT_SECTION ".rel.text"
#define REL_DATA_SECTION ".rel.data"
#define RELA_EH_FRAME_SECTION ".rela.eh_frame"
#define RELA_TEXT_STARTUP_SECTION ".rela.text.startup"


typedef union
{
    Elf32_Ehdr *elf32;
    Elf64_Ehdr *elf64;
} Elf_Ehdr;


void print_flags(uint32_t flags);
uint32_t setFileFlags(Elf_Ehdr header, void *section_headers, int is_64, int is_little_endian, int is_big_endian);

uint32_t convert_endianness32(uint32_t value, int is_little_endian, int is_big_endian);
uint64_t convert_endianness64(uint64_t value, int is_little_endian, int is_big_endian);

void print_section_content(FILE *file, void *shdr_generic, int is_64, const char *section_name, int is_little_endian, int is_big_endian);

void process_elf_file(const char *filename);
void process_elf_header32(FILE *file, const char *filename);
void process_elf_header64(FILE *file, const char *filename);
void print_elf_info32(Elf32_Ehdr header, const char *filename, Elf32_Shdr *sections);
void print_elf_info64(Elf64_Ehdr header, const char *filename, Elf64_Shdr *sections);
void read_and_process_sections64(FILE *file, Elf64_Ehdr header);
void read_and_process_sections32(FILE *file, Elf32_Ehdr header);


#endif /* HOBJDUMP_H */
