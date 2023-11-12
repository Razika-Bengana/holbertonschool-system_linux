#include "hobjdump.h"

uint32_t setFileFlags(Elf_Ehdr header, void *section_headers, int is_64, int is_little_endian, int is_big_endian)
{
    uint32_t flags = 0;

    if (!header.elf64)  /* On vérifie la validité du pointeur */
        return (0);

    uint16_t e_type = is_64 ? header.elf64->e_type : header.elf32->e_type;

    switch (e_type)
    {
        case ET_REL:
            flags |= HAS_RELOC;
            break;
        case ET_EXEC:
            flags |= EXEC_P | D_PAGED;
            break;
        case ET_DYN:
            flags |= DYNAMIC | D_PAGED;
            break;
        default:
            flags |= D_PAGED;
            break;
    }
    int symtab_found = 0;
    uint16_t num_sections = is_64 ? header.elf64->e_shnum : header.elf32->e_shnum;
    int i;

    /* Ici, on caste le pointeur de section vers le type approprié (en fonction de is_64) */
    Elf64_Shdr *sections64 = is_64 ? (Elf64_Shdr *)section_headers : NULL;
    Elf32_Shdr *sections32 = is_64 ? NULL : (Elf32_Shdr *)section_headers;

    for (i = 0; i < num_sections; ++i)
    {
        uint32_t section_type;

        if (is_64)
        {
            section_type = convert_endianness64(sections64[i].sh_type, is_little_endian, is_big_endian);
        }
        else
        {
            section_type = convert_endianness32(sections32[i].sh_type, is_little_endian, is_big_endian);
        }

        if (section_type == SHT_SYMTAB)
        {
            symtab_found = 1;
            break;
        }
    }

    if (symtab_found)
    {
        flags |= HAS_SYMS;
    }

    return (flags);
}

void print_flags(uint32_t flags)
{
    printf("flags 0x%08x:\n", flags);

    bool has_exec_p = (flags & EXEC_P) != 0;
    bool has_syms = (flags & HAS_SYMS) != 0;
    bool has_reloc = (flags & HAS_RELOC) != 0;
    bool has_dyn = (flags & DYNAMIC) != 0;
    bool has_paged = (flags & D_PAGED) != 0;
    bool is_unknown = !(has_exec_p || has_syms || has_reloc || has_dyn || has_paged);

    /* Buffer pour stocker les flags */
    char buffer[256] = {0}; /* S'assurer que le buffer est assez grand */
    char* ptr = buffer;

    if (has_reloc)
    {
        ptr += sprintf(ptr, "HAS_RELOC, ");
    }
    if (has_syms)
    {
        ptr += sprintf(ptr, "HAS_SYMS, ");
    }
    if (has_exec_p)
    {
        ptr += sprintf(ptr, "EXEC_P, ");
    }
    if (has_dyn)
    {
        ptr += sprintf(ptr, "DYNAMIC, ");
    }
    if (has_paged)
    {
        ptr += sprintf(ptr, "D_PAGED, ");
    }
    if (is_unknown)
    {
        ptr += sprintf(ptr, "UNKNOWN, ");
    }

    /* Retirer la dernière virgule et espace si le buffer n'est pas vide */
    if (buffer[0] != '\0')
    {
        ptr[-2] = '\0'; /* Écraser la dernière virgule par la terminaison de la chaîne */
    }

    printf("%s\n", buffer);
}

