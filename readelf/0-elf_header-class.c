#include "elf_header.h"

/**
 * print_class - program that prints the ELF class of the ELF file
 *
 * this function reads the `e_ident[EI_CLASS]` field from the ELF header
 * to determine the class (whether it is a 32-bit or 64-bit ELF file);
 * it then prints this information to standard output;
 *
 * @header: a pointer to the ELF header structure, which could be either
 *          Elf64_Ehdr or Elf32_Ehdr
 * @is_elf64: a flag to specify whether the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void print_class(void *header, int is_elf64)
{
	unsigned char elf_class = is_elf64 ?
		((Elf64_Ehdr *)header)->e_ident[EI_CLASS] :
		((Elf32_Ehdr *)header)->e_ident[EI_CLASS];

	printf("  Class:                             ");
	switch (elf_class)
	{
	case ELFCLASS32:
		printf("ELF32\n");
		break;
	case ELFCLASS64:
		printf("ELF64\n");
		break;
	default:
		printf("Unknown\n");
		break;
	}
}
