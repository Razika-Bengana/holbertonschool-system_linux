#include "hobjdump.h"

uint32_t setFileFlags(Elf_Ehdr header, void *section_headers, int is_64)
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
        uint32_t section_type = is_64 ? sections64[i].sh_type : sections32[i].sh_type;
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

    if ((flags & EXEC_P) != 0)
    {
        printf("EXEC_P, ");
    }
    if ((flags & HAS_SYMS) != 0)
    {
        printf("HAS_SYMS, ");
    }
    if ((flags & D_PAGED) != 0)
    {
        printf("D_PAGED");
    }
    printf("\n");
}

