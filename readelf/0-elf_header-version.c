#include "elf_header.h"

/**
 * print_version - program that prints the ELF file version specified
 * in the ELF header
 *
 * this function reads the `e_ident[EI_VERSION]` field from the ELF header
 * to determine the version of the ELF file format being used;
 * it prints the version to the standard output, displaying "1 (current)"
 * for the current version or "Unknown" for unsupported versions;
 *
 * @header: a pointer to the ELF header structure, which could be either
 *          Elf64_Ehdr or Elf32_Ehdr
 * @is_elf64: a flag to specify whether the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void print_version(void *header, int is_elf64)
{
	unsigned char elf_version = is_elf64 ?
		((Elf64_Ehdr *)header)->e_ident[EI_VERSION] :
		((Elf32_Ehdr *)header)->e_ident[EI_VERSION];

	printf("  Version:                           ");
	switch (elf_version)
	{
	case EV_CURRENT:
		printf("1 (current)\n");
		break;
	default:
		printf("Unknown (%d)\n", elf_version);
		break;
	}
}
