#include "elf_header.h"

/**
 * validate_file_size - program that validates the size of the ELF file based
 * on its architecture
 *
 * this function checks if the given ELF file size meets the minimum
 * requirements for a valid ELF-32 or ELF-64 file;
 * if not, it prints an error and exits the program;
 *
 * @fileSize: the size of the ELF file in bytes
 * @is_64bit: a boolean flag that indicates whether the ELF file is 64-bit
 *
 * Return: nothing (void)
 */

void validate_file_size(size_t fileSize, int is_64bit)
{
	if (is_64bit && fileSize < sizeof(Elf64_Ehdr))
	{
		fprintf(stderr, "The file is too small to be a valid ELF-64 file.\n");
		exit(EXIT_FAILURE);
	}
	if (!is_64bit && fileSize < sizeof(Elf32_Ehdr))
	{
		fprintf(stderr, "The file is too small to be a valid ELF-32 file.\n");
		exit(EXIT_FAILURE);
	}
}


/**
 * get_section_header_info - program that retrieves and validates the section
 * header table information from the ELF header
 *
 * this function populates the section header offset and the section header
 * count from the ELF header;
 * it also performs validation to make sure the section header table does not
 * extend beyond the end of the file;
 *
 * @elf_header: a pointer to the ELF header in memory
 * @fileSize: the size of the ELF file in bytes
 * @is_64bit: a boolean flag indicating whether the ELF file is 64-bit
 * @is_big_endian: a boolean flag indicating if the ELF file is big-endian
 * @e_shoff: a pointer to store the section header table offset
 * @count: a pointer to store the number of section headers
 *
 * Return: nothing (void)
 */

void get_section_header_info(void *elf_header, size_t fileSize, int is_64bit,
			     int is_big_endian, uint64_t *e_shoff,
			     uint16_t *count)
{
	*e_shoff = is_64bit ?
		((Elf64_Ehdr *)elf_header)->e_shoff :
		((Elf32_Ehdr *)elf_header)->e_shoff;
	*count = is_64bit ?
		((Elf64_Ehdr *)elf_header)->e_shnum :
		((Elf32_Ehdr *)elf_header)->e_shnum;

	if (is_big_endian)
	{
		*e_shoff = is_64bit ?
			swap_uint64(*e_shoff) :
			swap_uint32((uint32_t)*e_shoff);
		*count = swap_uint16(*count);
	}

	size_t sh_size = is_64bit ? sizeof(Elf64_Shdr) : sizeof(Elf32_Shdr);

	if (*e_shoff == 0 || *count == 0 ||
	    *e_shoff + sh_size * *count > fileSize)
	{
		fprintf(stderr, "Section header table goes past end of file.\n");
		exit(EXIT_FAILURE);
	}
}


/**
 * get_string_table_index - program that fetches the index of the section
 * header string table from the ELF header
 *
 * this function retrieves the index of the section header string table which
 * holds the names of the sections;
 * it converts the index to the correct endianness if necessary
 *
 * @elf_header: a pointer to the ELF header in memory
 * @e_shstrndx: a pointer to store the section header string table index
 * @is_64bit: a boolean flag indicating whether the ELF file is 64-bit
 * @is_big_endian: a boolean flag indicating if the ELF file is big-endian
 *
 * Return: nothing (void)
 */

void get_string_table_index(void *elf_header, uint16_t *e_shstrndx,
			    int is_64bit, int is_big_endian)
{
	*e_shstrndx = is_64bit ? ((Elf64_Ehdr *)elf_header)->e_shstrndx :
		((Elf32_Ehdr *)elf_header)->e_shstrndx;

	if (is_big_endian)
	{
		*e_shstrndx = swap_uint16(*e_shstrndx);
	}
}


/**
 * init_section_header_info - program that initializes a SectionHeaderInfo structure
 *
 * this function initializes a SectionHeaderInfo structure with the given
 * parameters, facilitating easy passage of multiple related variables
 *
 * @elf_header: a pointer to the ELF header
 * @shdr: a pointer to the section header
 * @count: the number of section headers
 * @strtab: a pointer to the string table
 * @is_64bit: the flag indicating if the ELF is 64-bit
 *
 * Return: the initialized SectionHeaderInfo structure
 */

SectionHeaderInfo init_section_header_info(void *elf_header, void *shdr, uint16_t count, char *strtab, int is_64bit)
{
    SectionHeaderInfo info = {
            .elf_header = elf_header,
            .shdr = shdr,
            .count = count,
            .strtab = strtab,
            .is_64bit = is_64bit
    };
    return (info);
}


/**
 * compute_strtab_offset - program that computes the offset of the string table
 *
 * this function computes the offset of the string table based on various
 * parameters and endian-ness;
 * it supports both 64-bit and 32-bit ELF files;
 *
 * @shdr: a pointer to the section header
 * @e_shstrndx: the index of the section header string table
 * @sh_size: the size of a single section header
 * @is_64bit: the flag indicating if the ELF is 64-bit
 * @is_big_endian: the flag indicating if the ELF is big-endian
 *
 * Return: the offset of the string table
 */

uint64_t compute_strtab_offset(void *shdr, uint16_t e_shstrndx, size_t sh_size, int is_64bit, int is_big_endian)
{
    uint64_t strtab_offset = is_64bit ?
                             ((Elf64_Shdr *)((char *)shdr + e_shstrndx * sh_size))->sh_offset :
                             ((Elf32_Shdr *)((char *)shdr + e_shstrndx * sh_size))->sh_offset;

    if (is_big_endian)
    {
        return is_64bit ? swap_uint64(strtab_offset) : swap_uint32((uint32_t)strtab_offset);
    }

    return (strtab_offset);
}


/**
 * handle_elf - main function for handling the ELF file
 *
 * this function serves as the primary interface for parsing and validating ELF files;
 * it orchestrates the calls to helper functions for specific validations and fetch operations;
 * the function will also initialize data structures to be used in further processing;
 *
 * @mem: a pointer to the beginning of the ELF file in memory
 * @fileSize: the size of the ELF file in bytes
 * @is_64bit: a flag indicating whether the ELF file is 64-bit
 * @is_big_endian: a flag indicating if the ELF file is big-endian
 *
 * Return: nothing (void)
 */

void handle_elf(char *mem, size_t fileSize, int is_64bit, int is_big_endian)
{
    void *elf_header = (void *)mem;
    validate_file_size(fileSize, is_64bit);

    uint64_t e_shoff;
    uint16_t count;
    get_section_header_info(elf_header, fileSize, is_64bit, is_big_endian, &e_shoff, &count);

    void *shdr = mem + e_shoff;
    uint16_t e_shstrndx;
    get_string_table_index(elf_header, &e_shstrndx, is_64bit, is_big_endian);

    if (e_shstrndx >= count)
    {
        fprintf(stderr, "Invalid string table index.\n");
        return;
    }

    size_t sh_size = is_64bit ? sizeof(Elf64_Shdr) : sizeof(Elf32_Shdr);
    uint64_t strtab_offset = compute_strtab_offset(shdr, e_shstrndx, sh_size, is_64bit, is_big_endian);

    if (strtab_offset == 0 || strtab_offset >= fileSize)
    {
        fprintf(stderr, "Invalid string table offset.\n");
        return;
    }

    char *strtab = (char *)(mem + strtab_offset);
    SectionHeaderInfo info = init_section_header_info(elf_header, shdr, count, strtab, is_64bit);

    print_section_headers(&info);
}

