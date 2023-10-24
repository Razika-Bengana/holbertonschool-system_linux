#include "elf_header.h"

/**
 * print_header_details - program that prints the detailed information
 * contained in an ELF header
 *
 * this function displays the fields in the ELF header based on its class
 * (32-bit or 64-bit);
 * it prints fields such as version, entry point address, offsets for program
 * and section headers, flags, sizes, and counts related to headers;
 *
 * @header: a pointer to the ELF header structure, which could be either
 *          Elf64_Ehdr or Elf32_Ehdr
 * @is_elf64: a flag to specify whether the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void print_header_details(void *header, int is_elf64)
{
	if (is_elf64)
	{
		Elf64_Ehdr *hdr = (Elf64_Ehdr *)header;

		printf("  Version:                           0x%x\n",
		       hdr->e_version);
		printf("  Entry point address:               0x%lx\n",
		       hdr->e_entry);
		printf("  Start of program headers:          %ld (bytes into file)\n",
		       hdr->e_phoff);
		printf("  Start of section headers:          %ld (bytes into file)\n",
		       hdr->e_shoff);
		printf("  Flags:                             0x%x\n",
		       hdr->e_flags);
		printf("  Size of this header:               %d (bytes)\n",
		       hdr->e_ehsize);
		printf("  Size of program headers:           %d (bytes)\n",
		       hdr->e_phentsize);
		printf("  Number of program headers:         %d\n",
		       hdr->e_phnum);
		printf("  Size of section headers:           %d (bytes)\n",
		       hdr->e_shentsize);
		printf("  Number of section headers:         %d\n",
		       hdr->e_shnum);
		printf("  Section header string table index: %d\n",
		       hdr->e_shstrndx);
	}
	else
	{
		Elf32_Ehdr *hdr = (Elf32_Ehdr *)header;

		printf("  Version:                           0x%x\n",
		       hdr->e_version);
		printf("  Entry point address:               0x%x\n",
		       hdr->e_entry);
		printf("  Start of program headers:          %d (bytes into file)\n",
		       hdr->e_phoff);
		printf("  Start of section headers:          %d (bytes into file)\n",
		       hdr->e_shoff);
		printf("  Flags:                             0x%x\n",
		       hdr->e_flags);
		printf("  Size of this header:               %d (bytes)\n",
		       hdr->e_ehsize);
		printf("  Size of program headers:           %d (bytes)\n",
		       hdr->e_phentsize);
		printf("  Number of program headers:         %d\n",
		       hdr->e_phnum);
		printf("  Size of section headers:           %d (bytes)\n",
		       hdr->e_shentsize);
		printf("  Number of section headers:         %d\n",
		       hdr->e_shnum);
		printf("  Section header string table index: %d\n",
		       hdr->e_shstrndx);
	}
}
