#include "hobjdump.h"

void print_section_content(FILE *file, void *shdr_generic, int is_64, const char *section_name)
{
    if (strcmp(section_name, BSS_SECTION_NAME) == 0 ||
        strcmp(section_name, SHSTRTAB_SECTION_NAME) == 0 ||
        strcmp(section_name, SYMTAB_SECTION_NAME) == 0 ||
        strcmp(section_name, STRTAB_SECTION_NAME) == 0)
    {
        return;
    }

    printf("Contents of section %s:\n", section_name);

    size_t i;

    Elf64_Shdr *shdr64 = is_64 ? (Elf64_Shdr *)shdr_generic : NULL;
    Elf32_Shdr *shdr32 = is_64 ? NULL : (Elf32_Shdr *)shdr_generic;


    size_t size = is_64 ? shdr64->sh_size : shdr32->sh_size;
    uint64_t offset = is_64 ? shdr64->sh_offset : shdr32->sh_offset;
    uint64_t addr = is_64 ? shdr64->sh_addr : shdr32->sh_addr;


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
            printf(" %06lx ", addr + i);
        }


        printf("%02x", buf[i]);


        if (i % 4 == 3)
        {
            printf(" ");
        }


        if ((i % 16 == 15) || (i == size - 1))
        {
            size_t line_start = i - (i % 16);
            size_t line_end = (i % 16 == 15) ? i : size - 1;


            if (i % 16 != 15)
            {
                for (size_t k = 0; k < ((15 - (i % 16)) / 4); k++)
                {
                    printf("         ");
                }
            }


            printf(" ");
            for (size_t j = line_start; j <= line_end; j++)
            {
                printf("%c", isprint(buf[j]) ? buf[j] : '.');
            }

            printf("\n");
        }
    }

    free(buf);
}

