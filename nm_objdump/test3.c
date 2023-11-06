#include "main.h"

void print_symbol_table32(Elf32_Shdr *section_header, Elf32_Sym *symbol_table, char *string_table, Elf32_Shdr *section_headers)
{
    int i;
    int symbol_count = section_header->sh_size / sizeof(Elf32_Sym);

    for (i = 0; i < symbol_count; i++)
    {
        Elf32_Sym symbol = symbol_table[i];
        char *symbol_name = string_table + symbol.st_name;

        /* On s'assure que le nom du symbole n'est pas nul et en plus qu'il ne s'agit pas d'un fichier */
        if (symbol.st_name != 0 && ELF32_ST_TYPE(symbol.st_info) != STT_FILE)
        {
            char symbol_type = '?';

            /* Vérifier d'abord les indices de section spéciale pour le format 32 bits, contrairement au 64 format */
            if (symbol.st_shndx == SHN_UNDEF)
                symbol_type = 'U';
            else if (symbol.st_shndx == SHN_ABS)
                symbol_type = 'A';
            else if (symbol.st_shndx == SHN_COMMON)
                symbol_type = 'C';
            else if (symbol.st_shndx < SHN_LORESERVE)
            {
                /* S'il ne s'agit pas d'une section spéciale, récupèrer alors l'en-tête de la section */
                Elf32_Shdr symbol_section = section_headers[symbol.st_shndx];

                /* Check pour les symboles faibles et uniques */
                if (ELF32_ST_BIND(symbol.st_info) == STB_GNU_UNIQUE)
                    symbol_type = 'u';
                else if (ELF32_ST_BIND(symbol.st_info) == STB_WEAK && ELF32_ST_TYPE(symbol.st_info) == STT_OBJECT)
                    symbol_type = symbol.st_shndx == SHN_UNDEF ? 'v' : 'V';
                else if (ELF32_ST_BIND(symbol.st_info) == STB_WEAK)
                    symbol_type = symbol.st_shndx == SHN_UNDEF ? 'w' : 'W';

                /* Vérifier les types de section et les flags */
                else if (symbol_section.sh_type == SHT_NOBITS && symbol_section.sh_flags == (SHF_ALLOC | SHF_WRITE))
                    symbol_type = 'B';
                else if (symbol_section.sh_type == SHT_PROGBITS)
                {
                    if (symbol_section.sh_flags == SHF_ALLOC)
                        symbol_type = 'R';
                    else if (symbol_section.sh_flags == (SHF_ALLOC | SHF_WRITE))
                        symbol_type = 'D';
                    else if (symbol_section.sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
                        symbol_type = 'T';
                }
                else if (symbol_section.sh_type == SHT_DYNAMIC)
                    symbol_type = 'D';
            }

            /* Convertir en minuscule si le symbole est local */
            if (ELF32_ST_BIND(symbol.st_info) == STB_LOCAL)
            {
                symbol_type = tolower(symbol_type);
            }

            /* Ne pas afficher l'adresse du symbole si elle équivaut à U ou w */
            if (symbol_type != 'U' && symbol_type != 'w')
            {
                printf("%08x %c %s\n", symbol.st_value, symbol_type, symbol_name);
            }
            else
            {
                printf("         %c %s\n", symbol_type, symbol_name);
            }
        }
    }
}

void process_elf_file32(char *file_path)
{
    FILE *file = fopen(file_path, "rb");

    if (file == NULL)
    {
        fprintf(stderr,"./hnm: %s: failed to open file\n", file_path);
        return;
    }

    Elf32_Ehdr elf_header;
    fread(&elf_header, sizeof(Elf32_Ehdr), 1, file);

    /* il faut vérifier le type de fichier ELF */
    if (elf_header.e_ident[EI_CLASS] != ELFCLASS32 && elf_header.e_ident[EI_CLASS] != ELFCLASS64)
    {
        fprintf(stderr,"./hnm: %s: unsupported ELF file format\n", file_path);
        fclose(file);
        return;
    }

    /* Pour l'endianness */
    int is_little_endian = (elf_header.e_ident[EI_DATA] == ELFDATA2LSB);
    int is_big_endian = (elf_header.e_ident[EI_DATA] == ELFDATA2MSB);
    if (!is_little_endian && !is_big_endian)
    {
        fprintf(stderr,"./hnm: %s: unsupported ELF file endianness\n", file_path);
        fclose(file);
        return;
    }

    /* pour la table des sections */
    Elf32_Shdr *section_headers = malloc(elf_header.e_shentsize * elf_header.e_shnum);
    if (section_headers == NULL)
    {
        fprintf(stderr,"./hnm: %s: memory allocation error for section_headers\n", file_path);
        fclose(file);
        return;
    }

    fseek(file, elf_header.e_shoff, SEEK_SET);
    fread(section_headers, elf_header.e_shentsize, elf_header.e_shnum, file);

    /* il faut trouver l'index de la section de la table des symboles */
    int symbol_table_index = -1;
    int i;

    for (i = 0; i < elf_header.e_shnum; i++)
    {
        if (i < elf_header.e_shnum && section_headers[i].sh_type == SHT_SYMTAB)
        {
            symbol_table_index = i;
            break;
        }
    }

    if (symbol_table_index == -1)
    {
        fprintf(stderr, "./hnm: %s: no symbols\n", file_path);
        fclose(file);
        free(section_headers);
        return;
    }
    // il faut maintenant lire la table des symboles */
    Elf32_Shdr symbol_table_header = section_headers[symbol_table_index];
    Elf32_Sym *symbol_table = malloc(symbol_table_header.sh_size);
    fseek(file, symbol_table_header.sh_offset, SEEK_SET);
    fread(symbol_table, symbol_table_header.sh_size, 1, file);

    /* partie pour la table des chaînes de caractères */
    int string_table_index = symbol_table_header.sh_link;

    Elf32_Shdr string_table_header = section_headers[string_table_index];

    char *string_table = malloc(string_table_header.sh_size);

    fseek(file, string_table_header.sh_offset, SEEK_SET);
    fread(string_table, string_table_header.sh_size, 1, file);

    /* printer la table des symboles */
    print_symbol_table32(&symbol_table_header, symbol_table, string_table, section_headers);

    fclose(file);
    free(section_headers);
    free(symbol_table);
    free(string_table);
}