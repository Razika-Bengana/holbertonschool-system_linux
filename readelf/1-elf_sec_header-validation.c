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
