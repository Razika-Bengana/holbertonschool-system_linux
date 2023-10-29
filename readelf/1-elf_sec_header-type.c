#include "elf_header.h"

/**
 * section_type_to_string - program that converts an ELF section type constant
 * to its corresponding string representation
 *
 * given a uint32_t type value that represents an ELF section type,
 * this function returns a pointer to a string that describes the section type
 * in human-readable form;
 *
 * Supported ELF section types:
 * - SHT_NULL: represents an unused section header;
 * - SHT_SYMTAB: contains a symbol table for link editing;
 * - SHT_STRTAB: holds a string table;
 * - SHT_RELA: holds relocation entries with explicit addends;
 * - SHT_HASH: holds a hash table;
 * - SHT_DYNAMIC: holds dynamic linking information;
 * - SHT_NOTE: holds information that marks a file;
 * - SHT_NOBITS: holds information, but no file data;
 * - SHT_REL: holds relocation entries without explicit addends;
 * - SHT_SHLIB: reserved, undefined semantics;
 * - SHT_DYNSYM: holds a minimal set of dynamic linking symbols;
 * - SHT_INIT_ARRAY, SHT_FINI_ARRAY, SHT_PREINIT_ARRAY: array of pointers
 *   to initialization and termination functions;
 * - SHT_GROUP: section group;
 * - SHT_SYMTAB_SHNDX: extended section indeces;
 * - SHT_NUM: number of defined types;
 * - SHT_PROGBITS: holds program data;
 * - Vendor-specific types (VERSYM, VERDEF, VERNEED, GNU_HASH, LOOS, LOUSER,
 *   etc...)
 *
 * @type: a uint32_t value representing the ELF section type
 *
 * Return: a pointer to a string describing the section type,
 *         or "UNKNOWN" for unrecognized types
 */

char *section_type_to_string(uint32_t type)
{
	switch (type)
	{
	case SHT_NULL: return "NULL";
	case SHT_SYMTAB: return "SYMTAB";
	case SHT_STRTAB: return "STRTAB";
	case SHT_RELA: return "RELA";
	case SHT_HASH: return "HASH";
	case SHT_DYNAMIC: return "DYNAMIC";
	case SHT_NOTE: return "NOTE";
	case SHT_NOBITS: return "NOBITS";
	case SHT_REL: return "REL";
	case SHT_SHLIB: return "SHLIB";
	case SHT_DYNSYM: return "DYNSYM";
	case SHT_INIT_ARRAY: return "INIT_ARRAY";
	case SHT_FINI_ARRAY: return "FINI_ARRAY";
	case SHT_PREINIT_ARRAY: return "PREINIT_ARRAY";
	case SHT_GROUP: return "GROUP";
	case SHT_SYMTAB_SHNDX: return "SYMTAB_SHNDX";
	case SHT_NUM: return "NUM";
	case SHT_PROGBITS: return "PROGBITS";
	case 0x6fffffff: return "VERSYM";
	case 0x6ffffffc: return "VERDEF";
	case 0x6ffffffE: return "VERNEED";
	case 0x6FFFFFF6: return "GNU_HASH";
	case 0x60000000: return "LOOS";
	case 0x60000000 + 0xFFFFFF3: return "LOOS+ffffff3";
	case 0x60000000 + 0xFFFFFF1: return "LOOS+ffffff1";
	case 0x60000001: return "LOUSER";
	default: return "UNKNOWN";
	}
}
