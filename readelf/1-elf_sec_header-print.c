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
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude),x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
}


/**
 * swap_and_assign_section_data_64 - program that converts and swaps
 * (if necessary) 64-bit ELF section headers
 * from the original ELF format to a more manageable format
 *
 * @section: the original 64-bit ELF section header
 * @converted: the converted section header
 * @isLittleEndian: indicates if the ELF file uses little-endian encoding
 *
 * Return: nothing (void)
 */

void swap_and_assign_section_data_64(Elf64_Shdr *section,
				     ConvertedSectionHeader64 *converted,
				     int isLittleEndian)
{
	if (isLittleEndian)
	{
		converted->sh_name = section->sh_name;
		converted->sh_type = section->sh_type;
		converted->sh_addr = section->sh_addr;
		converted->sh_offset = section->sh_offset;
		converted->sh_size = section->sh_size;
		converted->sh_entsize = section->sh_entsize;
		converted->sh_flags = section->sh_flags;
		converted->sh_link = section->sh_link;
		converted->sh_info = section->sh_info;
		converted->sh_addralign = section->sh_addralign;
	}
	else
	{
		converted->sh_name = swap_uint32(section->sh_name);
		converted->sh_type = swap_uint32(section->sh_type);
		converted->sh_addr = swap_uint64(section->sh_addr);
		converted->sh_offset = swap_uint64(section->sh_offset);
		converted->sh_size = swap_uint64(section->sh_size);
		converted->sh_entsize = swap_uint64(section->sh_entsize);
		converted->sh_flags = swap_uint64(section->sh_flags);
		converted->sh_link = swap_uint32(section->sh_link);
		converted->sh_info = swap_uint32(section->sh_info);
		converted->sh_addralign = swap_uint64(section->sh_addralign);
	}
}


/**
 * handle_64bit_section - program that processes and prints the section headers
 * for a 64-bit ELF file
 *
 * @info: a struct containing all the necessary info about the ELF file
 * @isLittleEndian: indicates if the ELF file uses little-endian encoding
 *
 * Return: nothing (void)
 */

void handle_64bit_section(SectionHeaderInfo *info, int isLittleEndian)
{
	char flag_str[5];

	for (int i = 0; i < info->count; i++)
	{
		ConvertedSectionHeader64 converted;
		Elf64_Shdr *section = &((Elf64_Shdr *)info->shdr)[i];

		swap_and_assign_section_data_64(section, &converted,
						isLittleEndian);

		print_flags(converted.sh_flags, flag_str);

		printf("  [%2d] %-17s %-15s %016lx %06lx %06lx %02lx %3s %2d %3u %2lu\n",
		       i,
		       info->strtab + converted.sh_name,
		       section_type_to_string(converted.sh_type),
		       (unsigned long)converted.sh_addr,
		       (unsigned long)converted.sh_offset,
		       (unsigned long)converted.sh_size,
		       (unsigned long)converted.sh_entsize,
		       flag_str,
		       (int)converted.sh_link,
		       (unsigned int)converted.sh_info,
		       (unsigned long)converted.sh_addralign);
	}
	print_flags_section(1);
}


/**
 * swap_and_assign_section_data_32 - program that converts and swaps
 * (if necessary) 32-bit ELF section headers from the original ELF format
 * to a more manageable format
 *
 * @section: the original 32-bit ELF section header
 * @converted: a converted section header
 * @isLittleEndian: indicates if the ELF file uses little-endian encoding
 *
 * Return: nothing (void)
 */

void swap_and_assign_section_data_32(Elf32_Shdr *section,
				     ConvertedSectionHeader32 *converted,
				     int isLittleEndian)
{
	if (isLittleEndian)
	{
		converted->sh_name = section->sh_name;
		converted->sh_type = section->sh_type;
		converted->sh_addr = section->sh_addr;
		converted->sh_offset = section->sh_offset;
		converted->sh_size = section->sh_size;
		converted->sh_entsize = section->sh_entsize;
		converted->sh_flags = section->sh_flags;
		converted->sh_link = section->sh_link;
		converted->sh_info = section->sh_info;
		converted->sh_addralign = section->sh_addralign;
	}
	else
	{
		converted->sh_name = ntohl(section->sh_name);
		converted->sh_type = ntohl(section->sh_type);
		converted->sh_addr = ntohl(section->sh_addr);
		converted->sh_offset = ntohl(section->sh_offset);
		converted->sh_size = ntohl(section->sh_size);
		converted->sh_entsize = ntohl(section->sh_entsize);
		converted->sh_flags = ntohl(section->sh_flags);
		converted->sh_link = ntohl(section->sh_link);
		converted->sh_info = ntohl(section->sh_info);
		converted->sh_addralign = ntohl(section->sh_addralign);
	}
}


/**
 * handle_32bit_section - program that processes and prints the section headers
 * for a 32-bit ELF file
 *
 * @info: a struct containing all the necessary info about the ELF file
 * @isLittleEndian: indicates if the ELF file uses little-endian encoding
 *
 * Return: nothing (void)
 */

void handle_32bit_section(SectionHeaderInfo *info, int isLittleEndian)
{
	char flag_str[5];

	for (int i = 0; i < info->count; i++)
	{
		ConvertedSectionHeader32 converted;
		Elf32_Shdr *section = &((Elf32_Shdr *) info->shdr)[i];

		swap_and_assign_section_data_32(section, &converted,
						isLittleEndian);

		print_flags(converted.sh_flags, flag_str);

		printf("  [%2d] %-17s %-15s %08lx %06lx %06lx %02lx %3s %2lu %3lu %2lu\n",
		       i,
		       info->strtab + converted.sh_name,
		       section_type_to_string(converted.sh_type),
		       (unsigned long) converted.sh_addr,
		       (unsigned long) converted.sh_offset,
		       (unsigned long) converted.sh_size,
		       (unsigned long) converted.sh_entsize,
		       flag_str,
		       (unsigned long) converted.sh_link,
		       (unsigned long) converted.sh_info,
		       (unsigned long) converted.sh_addralign);
	}
	print_flags_section(0);
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
