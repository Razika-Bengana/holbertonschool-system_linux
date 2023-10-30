#include "elf_header.h"

void read_elf_header(FILE *file, unsigned char *e_ident, Elf64_Ehdr *ehdr64, Elf32_Ehdr *ehdr32)
{
    fseek(file, 0, SEEK_SET);
    fread(e_ident, 1, EI_NIDENT, file);

    if (e_ident[EI_MAG0] != 0x7F || e_ident[EI_MAG1] != 'E' ||
        e_ident[EI_MAG2] != 'L' || e_ident[EI_MAG3] != 'F')
    {
        printf("Not a valid ELF file\n");
        exit(1);
    }

    fseek(file, 0, SEEK_SET);

    if (e_ident[EI_CLASS] == ELFCLASS32)
    {
        fread(ehdr32, sizeof(Elf32_Ehdr), 1, file);
    }

    else if (e_ident[EI_CLASS] == ELFCLASS64)
    {
        fread(ehdr64, sizeof(Elf64_Ehdr), 1, file);
    }

    else
    {
        printf("Unknown ELF file class\n");
        exit(1);
    }
}

void read_elf_sections(FILE *file, Elf64_Ehdr *ehdr64, Elf32_Ehdr *ehdr32, unsigned char endianness)
{
    int i;
    size_t j;

    if (ehdr32->e_ident[EI_CLASS] == ELFCLASS32)
    {
        fseek(file, ehdr32->e_shoff, SEEK_SET);
        Elf32_Shdr shdr32[ehdr32->e_shnum];

        fread(&shdr32, sizeof(Elf32_Shdr), ehdr32->e_shnum, file);

        for (i = 0; i < ehdr32->e_shnum; ++i)
        {
            shdr32[i].sh_name = convert_endianness_32(shdr32[i].sh_name, endianness);
            shdr32[i].sh_type = convert_endianness_32(shdr32[i].sh_type, endianness);
            shdr32[i].sh_flags = convert_endianness_32(shdr32[i].sh_flags, endianness);
            shdr32[i].sh_addr = convert_endianness_32(shdr32[i].sh_addr, endianness);
            shdr32[i].sh_offset = convert_endianness_32(shdr32[i].sh_offset, endianness);
            shdr32[i].sh_size = convert_endianness_32(shdr32[i].sh_size, endianness);
            shdr32[i].sh_link = convert_endianness_32(shdr32[i].sh_link, endianness);
            shdr32[i].sh_info = convert_endianness_32(shdr32[i].sh_info, endianness);
            shdr32[i].sh_addralign = convert_endianness_32(shdr32[i].sh_addralign, endianness);
            shdr32[i].sh_entsize = convert_endianness_32(shdr32[i].sh_entsize, endianness);
            /* voir s'il n'y a pas d'autres champs à convertir */
        }

        for (i = 0; i < ehdr32->e_shnum; ++i)
        {
            if (shdr32[i].sh_type == SHT_SYMTAB || shdr32[i].sh_type == SHT_DYNSYM)
            {
                uint32_t strtab_index = shdr32[i].sh_link;
                fseek(file, shdr32[i].sh_offset, SEEK_SET);

                size_t num_symbols = shdr32[i].sh_size / sizeof(Elf32_Sym);
                Elf32_Sym sym[num_symbols];
                fread(&sym, sizeof(Elf32_Sym), num_symbols, file);

                for (j = 0; j < num_symbols; ++j)
                {
                    char type = get_symbol_type_32(sym[j], shdr32);
                    const char *name = get_symbol_name_32(sym[j], file, shdr32[strtab_index]);
                    printf("%08lx %c %s\n", (unsigned long)sym[j].st_value, type, name);
                }
            }
        }

    }

    else if (ehdr64->e_ident[EI_CLASS] == ELFCLASS64)
    {
        fseek(file, ehdr64->e_shoff, SEEK_SET);
        Elf64_Shdr shdr64[ehdr64->e_shnum];

        fread(&shdr64, sizeof(Elf64_Shdr), ehdr64->e_shnum, file);

        for (i = 0; i < ehdr64->e_shnum; ++i)
        {
            shdr64[i].sh_name = convert_endianness_32(shdr64[i].sh_name, endianness); /* c'est 32 bits même dans un en-tête 64 bits */
            shdr64[i].sh_type = convert_endianness_64(shdr64[i].sh_type, endianness);
            shdr64[i].sh_flags = convert_endianness_64(shdr64[i].sh_flags, endianness);
            shdr64[i].sh_addr = convert_endianness_64(shdr64[i].sh_addr, endianness);
            shdr64[i].sh_offset = convert_endianness_64(shdr64[i].sh_offset, endianness);
            shdr64[i].sh_size = convert_endianness_64(shdr64[i].sh_size, endianness);
            shdr64[i].sh_link = convert_endianness_32(shdr64[i].sh_link, endianness); /* c'est 32 bits même dans un en-tête 64 bits */
            shdr64[i].sh_info = convert_endianness_32(shdr64[i].sh_info, endianness); /* c'est 32 bits même dans un en-tête 64 bits */
            shdr64[i].sh_addralign = convert_endianness_64(shdr64[i].sh_addralign, endianness);
            shdr64[i].sh_entsize = convert_endianness_64(shdr64[i].sh_entsize, endianness);
            /* vois aussi ici s'il n'y a pas d'autres champs à convertir */
        }

        for (i = 0; i < ehdr64->e_shnum; ++i)
        {
            if (shdr64[i].sh_type == SHT_SYMTAB || shdr64[i].sh_type == SHT_DYNSYM)
            {
                uint32_t strtab_index = shdr64[i].sh_link;
                fseek(file, shdr64[i].sh_offset, SEEK_SET);

                size_t num_symbols = shdr64[i].sh_size / sizeof(Elf64_Sym);
                Elf64_Sym sym[num_symbols];
                fread(&sym, sizeof(Elf64_Sym), num_symbols, file);

                for (j = 0; j < num_symbols; ++j)
                {
                    char type = get_symbol_type_64(sym[j], shdr64);
                    const char *name = get_symbol_name_64(sym[j], file, shdr64[strtab_index]);
                    printf("%016llx %c %s\n", (unsigned long long)sym[j].st_value, type, name);
                }
            }
        }

    }
    else
    {
        printf("Unknown ELF file class\n");
        exit(1);
    }
}


void process_elf_file(FILE *file)
{
    unsigned char e_ident[EI_NIDENT];
    Elf64_Ehdr ehdr64;
    Elf32_Ehdr ehdr32;

    read_elf_header(file, e_ident, &ehdr64, &ehdr32);

    unsigned char endianness = e_ident[EI_DATA];

    if (e_ident[EI_CLASS] == ELFCLASS32)
    {
        ehdr32.e_shoff = convert_endianness_32(ehdr32.e_shoff, endianness);
    }

    else
    {
        ehdr64.e_shoff = convert_endianness_64(ehdr64.e_shoff, endianness);
    }

    read_elf_sections(file, &ehdr64, &ehdr32, endianness);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: hnm [objfile ...]\n");
        return (1);
    }

    for (int i = 1; i < argc; ++i)
    {
        FILE *file = fopen(argv[i], "rb");
        if (!file)
        {
            printf("Cannot open file %s\n", argv[i]);
            return (1);
        }

        process_elf_file(file);

        fclose(file);
    }

    return (0);
}
