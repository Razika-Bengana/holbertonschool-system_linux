#include "elf_header.h"

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