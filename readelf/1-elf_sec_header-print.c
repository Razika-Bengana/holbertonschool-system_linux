#include "elf_header.h"

/**
 * print_common_headers - program that prints the common header lines
 * for ELF section headers
 *
 * @num_headers: the number of section headers
 * @offset: the offset where the section headers begin
 * @is_64bit: indicates if the ELF file is 64-bit
 *
 * Return: nothing (void)
 */

void print_common_headers(int num_headers, Elf64_Off offset, int is_64bit)
{
	printf("There are %d section headers, starting at offset 0x%lx:\n",
	       num_headers, (unsigned long)offset);
	printf("\nSection Headers:\n");
	printf("  [Nr] Name              Type            ");
	if (is_64bit)
	{
		printf("Address          ");
	}
	else
	{
		printf("Addr     ");
	}
	printf("Off    Size   ES Flg Lk Inf Al\n");
}


/**
 * print_flags_section - program that prints the legend explaining the flags
 * in the section headers
 *
 * @is_64bit: indicates if the ELF file is 64-bit
 *
 * Return: nothing (void)
 */

void print_flags_section(int is_64bit)
{
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings)");
	if (is_64bit)
	{
		printf(", l (large)");
	}
	printf("\n");
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude),");
	printf(" x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific), ");
	printf("p (processor specific)\n");
}


/**
 * print_section_headers - main function for printing ELF section headers
 *
 * Based on the information available in the SectionHeaderInfo structure,
 * this function determines whether the ELF file is 32-bit or 64-bit;
 * it also figures out if the file uses little-endian or big-endian encoding;
 * subsequently, it calls the appropriate functions to handle and print
 * the section headers accordingly
 *
 * @info: a pointer to a SectionHeaderInfo struct that contains
 *        essential details about the ELF file, such as whether it is
 *        32-bit or 64-bit, as well as pointers to the ELF header and
 *        section header arrays
 *
 * Return: nothing (void)
 */

void print_section_headers(SectionHeaderInfo *info)
{
	int num_headers, isLittleEndian;
	Elf64_Off offset;

	if (info->is_64bit)
	{
		Elf64_Ehdr *ehdr = (Elf64_Ehdr *)info->elf_header;

		isLittleEndian = (ehdr->e_ident[EI_DATA] == ELFDATA2LSB);
		num_headers = isLittleEndian ? ehdr->e_shnum :
			ntohs(ehdr->e_shnum);
		offset = isLittleEndian ? ehdr->e_shoff : ntohl(ehdr->e_shoff);
		print_common_headers(num_headers, offset, 1);
		handle_64bit_section(info, isLittleEndian);
	}
	else
	{
		Elf32_Ehdr *ehdr = (Elf32_Ehdr *)info->elf_header;

		isLittleEndian = (ehdr->e_ident[EI_DATA] == ELFDATA2LSB);
		num_headers = isLittleEndian ? ehdr->e_shnum :
			ntohs(ehdr->e_shnum);
		offset = isLittleEndian ? ehdr->e_shoff : ntohl(ehdr->e_shoff);
		print_common_headers(num_headers, offset, 0);
		handle_32bit_section(info, isLittleEndian);
	}
}
