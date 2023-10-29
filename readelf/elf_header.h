#ifndef ELF_HEADER_H
#define ELF_HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include <stdint.h>
#include <netinet/in.h>


typedef struct
{
        void *elf_header;
        void *shdr;
        int count;
        char *strtab;
        int is_64bit;
} SectionHeaderInfo;


typedef struct
{
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint64_t sh_entsize;
    uint64_t sh_flags;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
} ConvertedSectionHeader64;


typedef struct
{
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_entsize;
    uint32_t sh_flags;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
} ConvertedSectionHeader32;


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

int open_file(const char *filename);
int check_file_stat(int fd, struct stat *sb);
void *map_file(int fd, struct stat *sb);
void handle_elf_file(void *mem, struct stat *sb);

void print_common_headers(int num_headers, Elf64_Off offset, int is_64bit);
void print_flags_section(int is_64bit);
void swap_and_assign_section_data_64(Elf64_Shdr *section, ConvertedSectionHeader64 *converted, int isLittleEndian);
void handle_64bit_section(SectionHeaderInfo *info, int isLittleEndian);
void swap_and_assign_section_data_32(Elf32_Shdr *section, ConvertedSectionHeader32 *converted, int isLittleEndian);
void handle_32bit_section(SectionHeaderInfo *info, int isLittleEndian);
void print_section_headers(SectionHeaderInfo *info);

uint16_t swap_uint16(uint16_t val);
uint32_t swap_uint32(uint32_t val);
uint64_t swap_uint64(uint64_t val);

void validate_file_size(size_t fileSize, int is_64bit);
void get_section_header_info(void *elf_header, size_t fileSize, int is_64bit, int is_big_endian, uint64_t *e_shoff, uint16_t *count);
void get_string_table_index(void *elf_header, uint16_t *e_shstrndx, int is_64bit, int is_big_endian);
void handle_elf(char *mem, size_t fileSize, int is_64bit, int is_big_endian);

char *section_type_to_string(uint32_t type);



/* task 2 */


#endif /* ELF_HEADER_H */
