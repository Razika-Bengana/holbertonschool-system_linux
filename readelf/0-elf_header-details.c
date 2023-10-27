#include "elf_header.h"

/**
 * print_header_details_64 - program that prints the ELF header details
 * for 64-bit files
 *
 * this function takes an ELF header structure for a 64-bit file and prints
 * all the relevant details such as version, entry point, program headers,
 * section headers, etc...;
 *
 * @header: a pointer to an Elf64_Ehdr structure containing
 *          the ELF header details
 *
 * Return: nothing (void)
 */

void print_header_details_64(Elf64_Ehdr *header)
{
	printf("  Version:                           0x%x\n",
	       header->e_version);
	printf("  Entry point address:               0x%lx\n",
	       header->e_entry);
	printf("  Start of program headers:          %ld (bytes into file)\n",
	       header->e_phoff);
	printf("  Start of section headers:          %ld (bytes into file)\n",
	       header->e_shoff);
	printf("  Flags:                             0x%x\n",
	       header->e_flags);
	printf("  Size of this header:               %d (bytes)\n",
	       header->e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n",
	       header->e_phentsize);
	printf("  Number of program headers:         %d\n",
	       header->e_phnum);
	printf("  Size of section headers:           %d (bytes)\n",
	       header->e_shentsize);
	printf("  Number of section headers:         %d\n",
	       header->e_shnum);
	printf("  Section header string table index: %d\n",
	       header->e_shstrndx);
}


/**
 * print_header_details_32 - program that prints the ELF header details
 * for 32-bit files
 *
 * this function takes an ELF header structure for a 32-bit file and prints
 * all the relevant details such as version, entry point, program headers,
 * section headers, etc...;
 *
 * @header: a pointer to an Elf32_Ehdr structure containing the ELF header
 *          details
 *
 * Return: nothing (void)
 */

void print_header_details_32(Elf32_Ehdr *header)
{
	printf("  Version:                           0x%x\n",
	       header->e_version);
	printf("  Entry point address:               0x%x\n",
	       header->e_entry);
	printf("  Start of program headers:          %d (bytes into file)\n",
	       header->e_phoff);
	printf("  Start of section headers:          %d (bytes into file)\n",
	       header->e_shoff);
	printf("  Flags:                             0x%x\n",
	       header->e_flags);
	printf("  Size of this header:               %d (bytes)\n",
	       header->e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n",
	       header->e_phentsize);
	printf("  Number of program headers:         %d\n",
	       header->e_phnum);
	printf("  Size of section headers:           %d (bytes)\n",
	       header->e_shentsize);
	printf("  Number of section headers:         %d\n",
	       header->e_shnum);
	printf("  Section header string table index: %d\n",
	       header->e_shstrndx);
}


/**
 * print_header_details - program that prints the ELF header details
 * for either 32-bit or 64-bit files
 *
 * this function serves as a wrapper for the specific 32-bit and 64-bit
 * ELF header detail printing functions;
 * it takes a generic pointer to an ELF header and a flag indicating whether
 * the ELF file is 64-bit;
 * it then casts the header to the appropriate type and calls the corresponding
 * function to print the details;
 *
 * @header: a generic pointer to an ELF header structure
 * @is_elf64: a flag indicating whether the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void print_header_details(void *header, int is_elf64)
{
	if (is_elf64)
	{
		print_header_details_64((Elf64_Ehdr *)header);
	}
	else
	{
		print_header_details_32((Elf32_Ehdr *)header);
	}
}
