#include "elf_header.h"

/**
 * print_abi_version - program that prints the ABI version specified
 * in the ELF header
 *
 * this function reads the `e_ident[EI_ABIVERSION]` field from the ELF header
 * to determine the ABI (Application Binary Interface) version used
 * by the ELF file;
 * the ABI version is then printed to standard output;
 *
 * @header: a pointer to the ELF header structure, which could be either
 *          Elf64_Ehdr or Elf32_Ehdr
 * @is_elf64: a flag to specify whether the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void print_abi_version(void *header, int is_elf64)
{
	unsigned char elf_abi_version = is_elf64 ?
		((Elf64_Ehdr *)header)->e_ident[EI_ABIVERSION] :
		((Elf32_Ehdr *)header)->e_ident[EI_ABIVERSION];

	printf("  ABI Version:                       ");
	printf("%d\n", elf_abi_version);
}
