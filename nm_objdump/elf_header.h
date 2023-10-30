#ifndef ELF_HEADER_H
#define ELF_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <elf.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <endian.h>
#include <netinet/in.h>


char get_symbol_type_32(Elf32_Sym sym, Elf32_Shdr *shdr);
char get_symbol_type_64(Elf64_Sym sym, Elf64_Shdr *shdr);
char* get_symbol_name_32(Elf32_Sym sym, FILE *file, Elf32_Shdr strtab);
char* get_symbol_name_64(Elf64_Sym sym, FILE *file, Elf64_Shdr strtab);

int is_little_endian();
uint32_t convert_endianness_32(uint32_t val, unsigned char endianness);
uint64_t convert_endianness_64(uint64_t val, unsigned char endianness);
uint16_t convert_endianness_16(uint16_t val, unsigned char endianness);

void read_elf_header(FILE *file, unsigned char *e_ident, Elf64_Ehdr *ehdr64, Elf32_Ehdr *ehdr32);
void read_elf_sections(FILE *file, Elf64_Ehdr *ehdr64, Elf32_Ehdr *ehdr32, unsigned char endianness);
void process_elf_file(FILE *file);

#endif /* ELF_HEADER_H */
