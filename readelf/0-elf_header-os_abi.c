#include "elf_header.h"

/**
 * print_os_abi - program that prints the OS/ABI field specified
 * in the ELF header
 *
 * this function reads the `e_ident[EI_OSABI]` field from the ELF header
 * to determine the OS/ABI (Operating System/Application Binary Interface)
 * for which the ELF file is intended;
 * it then prints a human-readable description of the OS/ABI
 * to the standard output;
 *
 * @header: a pointer to the ELF header structure, which could be either
 *          Elf64_Ehdr or Elf32_Ehdr
 * @is_elf64: a flag to specify whether the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void print_os_abi(void *header, int is_elf64)
{
	unsigned char elf_osabi = is_elf64 ?
		((Elf64_Ehdr *)header)->e_ident[EI_OSABI] :
		((Elf32_Ehdr *)header)->e_ident[EI_OSABI];

	printf("  OS/ABI:                            ");
	switch (elf_osabi)
	{
	case ELFOSABI_NONE:
		printf("UNIX - System V\n");
		break;
	case ELFOSABI_LINUX:
		printf("Linux\n");
		break;
	case ELFOSABI_SOLARIS:
		printf("UNIX - Solaris\n");
		break;
	case ELFOSABI_NETBSD:
		printf("UNIX - NetBSD\n");
		break;
	case ELFOSABI_FREEBSD:
		printf("UNIX - FreeBSD\n");
		break;
	default:
		printf("<unknown: %x>\n", elf_osabi);
		break;
	}
}
