#include "main.h"

void print_symbol_table32(Elf32_Shdr *section_header, Elf32_Sym *symbol_table, char *string_table)
{
    int symbol_count = section_header->sh_size / sizeof(Elf32_Sym);
    int i;

    for (i = 0; i < symbol_count; i++)
    {
        Elf32_Sym symbol = symbol_table[i];
        char *symbol_name = string_table + symbol.st_name;

        if (symbol.st_name != 0 && symbol.st_info != STT_FILE)
        {
            char symbol_type;
            switch (ELF32_ST_TYPE(symbol.st_info))
            {
                case STT_NOTYPE:
                    symbol_type = 'U';
                    break;
                case STT_OBJECT:
                    symbol_type = 'd';
                    break;
                case STT_FUNC:
                    symbol_type = 't';
                    break;
                case STT_SECTION:
                    symbol_type = 'S';
                    break;
                case STT_FILE:
                    symbol_type = 'F';
                    break;
                default:
                    symbol_type = '?';
                    break;
            }
            printf("%08x %c %s\n", symbol.st_value, symbol_type, symbol_name);
        }
    }
}

void process_elf_file32(char *file_path)
{
    int i;

    FILE *file = fopen(file_path, "rb");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }
    /* Lire l'en-tête ELF */
    Elf32_Ehdr elf_header;
    fread(&elf_header, sizeof(Elf32_Ehdr), 1, file);

    if (elf_header.e_ident[EI_CLASS] != ELFCLASS32 && elf_header.e_ident[EI_CLASS] != ELFCLASS64) {
        printf("Type de fichier ELF non pris en charge.\n");
        fclose(file);
        return;
    }

    int is_little_endian = (elf_header.e_ident[EI_DATA] == ELFDATA2LSB);
    int is_big_endian = (elf_header.e_ident[EI_DATA] == ELFDATA2MSB);

    if (!is_little_endian && !is_big_endian)
    {
        printf("Endianness du fichier ELF non pris en charge\n");
        fclose(file);
        return;
    }

    Elf32_Shdr *section_headers = malloc(elf_header.e_shentsize * elf_header.e_shnum);
    if (section_headers == NULL)
    {
        printf("Erreur lors de l'allocation de mémoire pour section_headers\n");
        fclose(file);
        return;
    }
    fseek(file, elf_header.e_shoff, SEEK_SET);
    fread(section_headers, elf_header.e_shentsize, elf_header.e_shnum, file);

    int symbol_table_index = -1;

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
    /* Lire la table des symboles */
    Elf32_Shdr symbol_table_header = section_headers[symbol_table_index];
    Elf32_Sym *symbol_table = malloc(symbol_table_header.sh_size);

    fseek(file, symbol_table_header.sh_offset, SEEK_SET);
    fread(symbol_table, symbol_table_header.sh_size, 1, file);

    /* Lire la table des chaînes de caractères */
    int string_table_index = symbol_table_header.sh_link;
    Elf32_Shdr string_table_header = section_headers[string_table_index];
    char *string_table = malloc(string_table_header.sh_size);

    fseek(file, string_table_header.sh_offset, SEEK_SET);
    fread(string_table, string_table_header.sh_size, 1, file);
    /* Afficher la table des symboles */
    printf("Adresse      Type Symbole\n");
    print_symbol_table32(&symbol_table_header, symbol_table, string_table);

    fclose(file);

    free(section_headers);
    free(symbol_table);
    free(string_table);
}