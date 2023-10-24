#include "elf_header.h"

/**
 * print_data - program that prints the data encoding of the ELF file and swaps
 * the byte order if necessary
 *
 * this function reads the `e_ident[EI_DATA]` field from the ELF header to
 * determine the data encoding of the ELF file;
 * it then prints this information to standard output;
 * additionally, if the data encoding in the ELF header does not match
 * the endianness of the system running this code, it performs byte-swapping
 * on several fields in the header;
 *
 * @header: a pointer to the ELF header structure, which could be either
 *          Elf64_Ehdr or Elf32_Ehdr
 * @is_elf64: a flag to specify whether the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void print_data(void *header, int is_elf64)
{
	unsigned char elf_data = is_elf64 ?
		((Elf64_Ehdr *)header)->e_ident[EI_DATA] :
		((Elf32_Ehdr *)header)->e_ident[EI_DATA];

	if ((system_is_big_endian() && elf_data == ELFDATA2LSB) ||
	    (system_is_little_endian() && elf_data == ELFDATA2MSB))
	{
		if (is_elf64)
		{
			Elf64_Ehdr *hdr64 = (Elf64_Ehdr *)header;

			swap_bytes(&hdr64->e_type, sizeof(hdr64->e_type));
			swap_bytes(&hdr64->e_machine, sizeof(hdr64->e_machine));
			swap_bytes(&hdr64->e_version, sizeof(hdr64->e_version));
			swap_bytes(&hdr64->e_entry, sizeof(hdr64->e_entry));
			swap_bytes(&hdr64->e_phoff, sizeof(hdr64->e_phoff));
			swap_bytes(&hdr64->e_shoff, sizeof(hdr64->e_shoff));
			swap_bytes(&hdr64->e_flags, sizeof(hdr64->e_flags));
			swap_bytes(&hdr64->e_ehsize, sizeof(hdr64->e_ehsize));
			swap_bytes(&hdr64->e_phentsize, sizeof(hdr64->
							       e_phentsize));
			swap_bytes(&hdr64->e_phnum, sizeof(hdr64->e_phnum));
			swap_bytes(&hdr64->e_shentsize, sizeof(hdr64->
							       e_shentsize));
			swap_bytes(&hdr64->e_shnum, sizeof(hdr64->e_shnum));
			swap_bytes(&hdr64->e_shstrndx, sizeof(hdr64->
							      e_shstrndx));
		}
		else
		{
			Elf32_Ehdr *hdr32 = (Elf32_Ehdr *)header;

			swap_bytes(&hdr32->e_type, sizeof(hdr32->e_type));
			swap_bytes(&hdr32->e_machine, sizeof(hdr32->e_machine));
			swap_bytes(&hdr32->e_version, sizeof(hdr32->e_version));
			swap_bytes(&hdr32->e_entry, sizeof(hdr32->e_entry));
			swap_bytes(&hdr32->e_phoff, sizeof(hdr32->e_phoff));
			swap_bytes(&hdr32->e_shoff, sizeof(hdr32->e_shoff));
			swap_bytes(&hdr32->e_flags, sizeof(hdr32->e_flags));
			swap_bytes(&hdr32->e_ehsize, sizeof(hdr32->e_ehsize));
			swap_bytes(&hdr32->e_phentsize, sizeof(hdr32->
							       e_phentsize));
			swap_bytes(&hdr32->e_phnum, sizeof(hdr32->e_phnum));
			swap_bytes(&hdr32->e_shentsize, sizeof(hdr32->
							       e_shentsize));
			swap_bytes(&hdr32->e_shnum, sizeof(hdr32->e_shnum));
			swap_bytes(&hdr32->e_shstrndx, sizeof(hdr32->
							      e_shstrndx));
		}
	}
	printf("  Data:                              ");
	switch (elf_data)
	{
	case ELFDATA2LSB:
		printf("2's complement, little endian\n");
		break;
	case ELFDATA2MSB:
		printf("2's complement, big endian\n");
		break;
	default:
		printf("Unknown\n");
		break;
	}
}
