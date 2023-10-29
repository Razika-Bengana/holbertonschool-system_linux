#include "elf_header.h"

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
