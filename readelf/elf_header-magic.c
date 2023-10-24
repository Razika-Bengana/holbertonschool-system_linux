#include "elf_header.h"

/**
 * print_magic - program that prints the magic number of the ELF file
 *
 * this function reads the `e_ident` array from the ELF header,
 * specifically focusing on the bytes that make up the magic number,
 * and prints them in hexadecimal format to the standard output;
 * the magic number is crucial for identifying the file as an ELF file;
 *
 * @header: a pointer to the ELF header structure, which could be either
 *          Elf64_Ehdr or Elf32_Ehdr
 * @is_elf64: a flag to specify whether the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void print_magic(void *header, int is_elf64)
{
	unsigned char *e_ident = is_elf64 ?
		((Elf64_Ehdr *)header)->e_ident :
		((Elf32_Ehdr *)header)->e_ident;

	printf("  Magic:   ");
	for (int i = 0; i < EI_NIDENT; ++i)
	{
		printf("%02x ", e_ident[i]);
	}
	printf("\n");
}
