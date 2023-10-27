#include "elf_header.h"

/**
 * print_type - program that prints the ELF file type specified
 * in the ELF header
 *
 * this function reads the `e_type` field from the ELF header to determine
 * the type of the ELF file;
 * depending on the type, it prints one of the following:
 * NONE, REL, EXEC, DYN, or CORE, along with a brief description of each type;
 *
 * @header: a pointer to the ELF header structure, which could be either
 *          Elf64_Ehdr or Elf32_Ehdr
 * @is_elf64: a flag to specify whether the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void print_type(void *header, int is_elf64)
{
	Elf64_Half elf_type = is_elf64 ?
		((Elf64_Ehdr *)header)->e_type :
		((Elf32_Ehdr *)header)->e_type;

	printf("  Type:                              ");
	switch (elf_type)
	{
	case ET_NONE:
		printf("NONE (No file type)\n");
		break;
	case ET_REL:
		printf("REL (Relocatable file)\n");
		break;
	case ET_EXEC:
		printf("EXEC (Executable file)\n");
		break;
	case ET_DYN:
		printf("DYN (Shared object file)\n");
		break;
	case ET_CORE:
		printf("CORE (Core file)\n");
		break;
	default:
		printf("Unknown (%d)\n", elf_type);
		break;
	}
}
