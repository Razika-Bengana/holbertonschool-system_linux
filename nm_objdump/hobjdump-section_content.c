#include "hobjdump.h"

void print_section_content(FILE *file, void *shdr_generic, int is_64, const char *section_name, int is_little_endian, int is_big_endian)
{
    size_t i, j;
    int space_to_fill;

    if (strcmp(section_name, BSS_SECTION) == 0 ||
        strcmp(section_name, SHSTRTAB_SECTION) == 0 ||
        strcmp(section_name, SYMTAB_SECTION) == 0 ||
        strcmp(section_name, STRTAB_SECTION) == 0 ||

        strcmp(section_name, TM_CLONE_TABLE_SECTION) == 0 ||
        strcmp(section_name, REL_TEXT_SECTION) == 0 ||
        strcmp(section_name, REL_DATA_SECTION) == 0 ||
        strcmp(section_name, RELA_EH_FRAME_SECTION) == 0 ||
        strcmp(section_name, RELA_TEXT_STARTUP_SECTION) == 0)
    {
        return;
    }

    printf("Contents of section %s:\n", section_name);

    size_t size = is_64 ? convert_endianness64(((Elf64_Shdr *) shdr_generic)->sh_size, is_little_endian, is_big_endian)
                        : convert_endianness32(((Elf32_Shdr *) shdr_generic)->sh_size, is_little_endian, is_big_endian);

    uint64_t offset = is_64 ? convert_endianness64(((Elf64_Shdr *) shdr_generic)->sh_offset, is_little_endian, is_big_endian)
                            : convert_endianness32(((Elf32_Shdr *) shdr_generic)->sh_offset, is_little_endian, is_big_endian);

    uint64_t addr = is_64 ? convert_endianness64(((Elf64_Shdr *) shdr_generic)->sh_addr, is_little_endian, is_big_endian)
                          : convert_endianness32(((Elf32_Shdr *) shdr_generic)->sh_addr, is_little_endian, is_big_endian);


    if (size == 0)
    {
        printf("Section %s is empty or size is zero.\n", section_name);
        return;
    }

    unsigned char *buf = malloc(size);
    if (!buf)
    {
        perror("Failed to allocate memory for section content");
        return;
    }

    fseek(file, offset, SEEK_SET);
    if (fread(buf, 1, size, file) != size)
    {
        perror("Failed to read section content");
        free(buf);
        return;
    }

    char address_buffer[20];
    sprintf(address_buffer, "%04lx", addr + size - 1);
    int address_width = strlen(address_buffer);

    for (i = 0; i < size; i++)
    {
        if (i % 16 == 0)
        {
            printf(" ");
            printf("%0*lx ", address_width, addr + i);
        }
        printf("%02x", buf[i]);

        if (i % 4 == 3)
        {
            printf(" ");
        }

        if ((i % 16 == 15) || (i == size - 1))
        {
            /* Combien d'octets sont manquants pour compléter une ligne de 16 octets */
            int bytes_missing = 16 - ((i % 16) + 1);
            /* Chaque octet manquant correspond à 2 caractères hexadécimaux. */
            space_to_fill = bytes_missing * 2;
            /* Ajouter un espace supplémentaire pour chaque groupe de 4 octets dans les octets manquants */
            space_to_fill += bytes_missing / 4;


                int remaining = 16 - (i % 16) - 1;

                /* Si c'est la fin d'une ligne complète, ou la fin des données */
                if (remaining == 0)
                {
                    /* Pas besoin d'espace supplémentaire */
                    space_to_fill = space_to_fill;
                }
                else if (i % 16 != 15)
                {
                    space_to_fill += 1; /* Ajouter un espace pour les adresses incomplètes */
                }
                printf("%*s", space_to_fill, "");

            printf(" ");


            for (j = i - (i % 16); j <= i; j++)
            {
                if (j < size)
                {
                    printf("%c", isprint(buf[j]) ? buf[j] : '.');
                }
            }
            if (i % 16 != 15)
            {
                int padding_for_alignment = (16 - (i % 16)) - 1;
                printf("%*s", padding_for_alignment, "");
            }
            printf("\n");
        }
    }
    free(buf);
}