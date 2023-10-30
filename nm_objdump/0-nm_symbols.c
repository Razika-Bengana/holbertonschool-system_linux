#include "elf_header.h"

char get_symbol_type_32(Elf32_Sym sym, Elf32_Shdr *shdr)
{
    char type = ' ';

    switch (ELF32_ST_TYPE(sym.st_info))
    {
        case STT_NOTYPE:
            if (sym.st_shndx == SHN_UNDEF)
                type = 'U';
            else
                type = 'n';
            break;
        case STT_OBJECT: type = 'd'; break;
        case STT_FUNC: type = 't'; break;
        case STT_SECTION:
            if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
                (shdr[sym.st_shndx].sh_flags & SHF_ALLOC) &&
                (shdr[sym.st_shndx].sh_flags & SHF_WRITE))
                type = 'd';
            else
                type = 't';
            break;
        case STT_FILE: type = 'f'; break;
        case STT_COMMON: type = 'B'; break;  /* B pour les symboles communs ? */
        case STT_TLS: type = 'l'; break;
    }

    if (ELF32_ST_BIND(sym.st_info) == STB_WEAK)
    {
        type = 'W';
    }

    if (sym.st_shndx == SHN_ABS)
    {
        type = 'A';
    }

    if (ELF32_ST_BIND(sym.st_info) == STB_WEAK && type != ' ')
    {
        type = toupper(type);
    }
    else if (ELF32_ST_BIND(sym.st_info) == STB_GLOBAL && type != ' ')
    {
        type = toupper(type);
    }

    return (type);
}



char get_symbol_type_64(Elf64_Sym sym, Elf64_Shdr *shdr)
{
    char type = ' ';

    /* symboles indéfinis */
    if (sym.st_shndx == SHN_UNDEF)
    {
        return (ELF64_ST_BIND(sym.st_info) == STB_WEAK ? 'w' : 'U');
    }

    switch (ELF64_ST_TYPE(sym.st_info))
    {
        case STT_NOTYPE:  type = 'n'; break;
        case STT_OBJECT:  type = 'd'; break;
        case STT_FUNC:    type = 't'; break;
        case STT_SECTION:
            if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS)
            {
                if (shdr[sym.st_shndx].sh_flags & SHF_ALLOC)
                {
                    if (shdr[sym.st_shndx].sh_flags & SHF_WRITE)
                        type = 'd';
                    else
                        type = 'r';
                }
                else
                {
                    type = 't';
                }
            }
            else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS)
            {
                type = 'b';
            }
            else
            {
                type = 't';
            }
            break;
        case STT_FILE:    type = 'f'; break;
        case STT_COMMON:  type = 'B'; break;
        case STT_TLS:     type = 'l'; break;
    }

    if (sym.st_shndx == SHN_ABS)
    {
        type = 'A';
    }

    if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
    {
        type = tolower(type);  /* Symboles faibles en minuscules */
    }
    else if (ELF64_ST_BIND(sym.st_info) == STB_GLOBAL)
    {
        type = toupper(type);  /* Symboles globaux en majuscules */
    }

    return (type);
}

char* get_symbol_name_32(Elf32_Sym sym, FILE *file, Elf32_Shdr strtab)
{
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    if (strtab.sh_offset + sym.st_name >= file_size)
    {
        fprintf(stderr, "Invalid symbol name offset for 32-bit ELF.\n");
        return (NULL);
    }

    fseek(file, strtab.sh_offset + sym.st_name, SEEK_SET);
    char *name = malloc(256);

    if (!name)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    size_t read_size = fread(name, sizeof(char), 255, file);

    if (read_size == 0)
    {
        fprintf(stderr, "Failed to read symbol name for 32-bit ELF.\n");
        free(name);
        return (NULL);
    }

    name[read_size] = '\0';
    return (name);
}


char* get_symbol_name_64(Elf64_Sym sym, FILE *file, Elf64_Shdr strtab)
{
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    if (strtab.sh_offset + sym.st_name >= (Elf64_Off)file_size)
    {
        fprintf(stderr, "Invalid symbol name offset for 64-bit ELF.\n");
        return (NULL);
    }

    fseek(file, strtab.sh_offset + sym.st_name, SEEK_SET);
    char *name = malloc(256);

    if (!name)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return (NULL);
    }

    size_t read_size = fread(name, sizeof(char), 255, file);

    if (read_size == 0)
    {
        fprintf(stderr, "Failed to read symbol name for 64-bit ELF.\n");
        free(name);
        return (NULL);
    }

    name[read_size] = '\0';
    return (name);
}




