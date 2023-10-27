#include "elf_header.h"

/**
 * read_elf_header - program that reads the ELF header from a given file
 *
 * this function reads the ELF header from the file and stores it in the
 * provided header pointer;
 * depending on whether the ELF file is 64-bit or 32-bit, it reads
 * the header into either an Elf64_Ehdr or an Elf32_Ehdr structure;
 *
 * @file: a pointer to the file from which the ELF header will be read
 * @header: a pointer to the memory location where the ELF header
 *          will be stored
 * @is_elf64: a flag to indicate if the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void read_elf_header(FILE *file, void *header, int is_elf64)
{
	if (is_elf64)
	{
		fread(header, sizeof(Elf64_Ehdr), 1, file);
	}
	else
	{
		fread(header, sizeof(Elf32_Ehdr), 1, file);
	}
}
