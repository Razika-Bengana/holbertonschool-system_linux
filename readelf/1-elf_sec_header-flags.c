#include "elf_header.h"

/**
 * print_flags - program that populates a buffer with flag characters
 * representing the properties of an ELF section
 *
 * the function examines each flag bit in the 'flags' parameter to determine
 * which attributes are set for the ELF section;
 * these attributes are then represented by specific characters
 * ('W' for SHF_WRITE, 'A' for SHF_ALLOC, etc.), which are placed into
 * the provided buffer;
 *
 * Flag representations:
 * - 'W': SHF_WRITE, the section contains writable data;
 * - 'A': SHF_ALLOC, the section occupies memory during execution;
 * - 'X': SHF_EXECINSTR, the section contains executable instructions;
 * - 'M': SHF_MERGE, the section might be merged;
 * - 'S': SHF_STRINGS, the section holds a string table;
 * - 'I': SHF_INFO_LINK, sh_info contains SHT index;
 * - 'L': SHF_LINK_ORDER, preserve order after combining;
 * - 'O': SHF_OS_NONCONFORMING, non-standard OS specific handling required;
 * - 'E': SHF_EXCLUDE, section is excluded unless referenced or allocated;
 *
 * @flags: the unsigned long integer representing the ELF section flags
 * @buf: the character buffer to populate with flag characters
 *
 * Return: nothing (void)
 */

void print_flags(unsigned long flags, char *buf)
{
	int index = 0;

	if (flags & SHF_WRITE)
	{
		buf[index++] = 'W';
	}
	if (flags & SHF_ALLOC)
	{
		buf[index++] = 'A';
	}
	if (flags & SHF_EXECINSTR)
	{
		buf[index++] = 'X';
	}
	if (flags & SHF_MERGE)
	{
		buf[index++] = 'M';
	}
	if (flags & SHF_STRINGS)
	{
		buf[index++] = 'S';
	}
	if (flags & SHF_INFO_LINK)
	{
		buf[index++] = 'I';
	}
	if (flags & SHF_LINK_ORDER)
	{
		buf[index++] = 'L';
	}
	if (flags & SHF_OS_NONCONFORMING)
	{
		buf[index++] = 'O';
	}
	if (flags & SHF_EXCLUDE)
	{
		buf[index++] = 'E';
	}
	buf[index] = '\0';
}
