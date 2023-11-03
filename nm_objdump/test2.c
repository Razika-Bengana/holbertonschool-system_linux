#include "main.h"

void print_symbol_table64(Elf64_Shdr *section_header, Elf64_Sym *symbol_table, char *string_table)
{
    int symbol_count = section_header->sh_size / sizeof(Elf64_Sym);
    int i;
    for (i = 0; i < symbol_count; i++)
    {
        Elf64_Sym symbol = symbol_table[i];
        char *symbol_name = string_table + symbol.st_name;
        if (symbol.st_name != 0 && ELF64_ST_TYPE(symbol.st_info) != STT_FILE)
        {
            char symbol_type = '?';
            switch (ELF64_ST_TYPE(symbol.st_info))
            {
                case STT_NOTYPE:
                    if (ELF64_ST_BIND(symbol.st_info) == STB_GLOBAL || ELF64_ST_BIND(symbol.st_info) == STB_WEAK)
                    {
                        symbol_type = 'w';
                    }
                    else
                    {
                        symbol_type = 'n';
                    }
                    break;
                case STT_OBJECT:
                    symbol_type = (ELF64_ST_BIND(symbol.st_info) == STB_GLOBAL) ? 'D' : 'd';
                    break;
                case STT_FUNC:
                    symbol_type = (ELF64_ST_BIND(symbol.st_info) == STB_GLOBAL) ? 'T' : 't';
                    break;
                case STT_SECTION:
                    symbol_type = 's';
                    break;
                case STT_FILE:
                    symbol_type = 'f';
                    break;
                case STT_TLS:
                    symbol_type = (ELF64_ST_BIND(symbol.st_info) == STB_GLOBAL) ? 'D' : 'd';
                    break;
                case STT_LOOS:
                    symbol_type = 'B';
                    break;
                case STT_HIOS:
                    symbol_type = 'b';
                    break;
                case STT_LOPROC:
                    symbol_type = 'R';
                    break;
                case STT_HIPROC:
                    symbol_type = 'r';
                    break;
            }
            if (symbol.st_shndx == SHN_UNDEF)
            {
                symbol_type = 'U';
            }
            /* weak symbols */
            if (ELF64_ST_BIND(symbol.st_info) == STB_WEAK)
            {
                symbol_type = (symbol_type == 'D' || symbol_type == 'd') ? 'W' : 'w';
            }
            printf("%016lx %c %s\n", symbol.st_value, symbol_type, symbol_name);
        }
    }
}



void process_elf_file64(char *file_path)
{
    FILE *file = fopen(file_path, "rb");

    if (file == NULL)
    {
        printf("Erreur pour l'ouverture du fichier\n");
        return;
    }

    Elf64_Ehdr elf_header;

    fread(&elf_header, sizeof(Elf64_Ehdr), 1, file);

    /* il faut vérifier le type de fichier ELF */
    if (elf_header.e_ident[EI_CLASS] != ELFCLASS32 && elf_header.e_ident[EI_CLASS] != ELFCLASS64)
    {
        printf("Type de format ELF non pris en charge\n");
        fclose(file);
        return;
    }

    /* Pour l'endianness */
    int is_little_endian = (elf_header.e_ident[EI_DATA] == ELFDATA2LSB);
    int is_big_endian = (elf_header.e_ident[EI_DATA] == ELFDATA2MSB);

    if (!is_little_endian && !is_big_endian)
    {
        printf("Endianness du fichier ELF non pris en charge\n");
        fclose(file);
        return;
    }

    /* pour la table des sections */
    Elf64_Shdr *section_headers = malloc(elf_header.e_shentsize * elf_header.e_shnum);

    if (section_headers == NULL)
    {
        printf("Erreur pour l'allocation de mémoire pour section_headers\n");
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
        printf("Aucune table des symboles trouvée\n");
        fclose(file);
        free(section_headers);
        return;
    }

    // il faut maintenant lire la table des symboles */
    Elf64_Shdr symbol_table_header = section_headers[symbol_table_index];
    Elf64_Sym *symbol_table = malloc(symbol_table_header.sh_size);
    fseek(file, symbol_table_header.sh_offset, SEEK_SET);
    fread(symbol_table, symbol_table_header.sh_size, 1, file);

    /* partie pour la table des chaînes de caractères */
    int string_table_index = symbol_table_header.sh_link;

    Elf64_Shdr string_table_header = section_headers[string_table_index];
    char *string_table = malloc(string_table_header.sh_size);
    fseek(file, string_table_header.sh_offset, SEEK_SET);
    fread(string_table, string_table_header.sh_size, 1, file);

    /* printer la table des symboles */
    printf("Adresse       Type Symbole\n");
    print_symbol_table64(&symbol_table_header, symbol_table, string_table);

    fclose(file);
    free(section_headers);
    free(symbol_table);
    free(string_table);
}