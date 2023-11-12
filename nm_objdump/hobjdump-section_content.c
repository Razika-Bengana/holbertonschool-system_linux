#include "hobjdump.h"

void print_section_content(FILE *file, void *shdr_generic, int is_64, const char *section_name, int is_little_endian, int is_big_endian)
{
    size_t i, j;
    int space_to_fill;

    if (strcmp(section_name, BSS_SECTION_NAME) == 0 ||
        strcmp(section_name, SHSTRTAB_SECTION_NAME) == 0 ||
        strcmp(section_name, SYMTAB_SECTION_NAME) == 0 ||
        strcmp(section_name, STRTAB_SECTION_NAME) == 0)
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

    for (i = 0; i < size; i++)
    {
        if (i % 16 == 0)
        {
            printf(" ");
            printf("%04lx ", addr + i);
        }

        printf("%02x", buf[i]);

        if (i % 4 == 3)
        {
            printf(" ");
        }

        if ((i % 16 == 15) || (i == size - 1))
        {
            printf(" ");
            space_to_fill = ((16 - (i % 16)) - 1) * 2 + ((16 - (i % 16)) / 4);
            if (i % 16 != 15)
            {
                printf("%*s", space_to_fill, "");
            }
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
                printf("%*s", (int)(16 - (i % 16)), "");
            }

            printf("\n");
        }
    }
    free(buf);
}
