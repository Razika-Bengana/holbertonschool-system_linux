#include "../hobjdump.h"

void print_section_content(FILE *file, void *shdr_generic, int is_64, const char *section_name, int is_little_endian, int is_big_endian)
{
    size_t i, j;
    int space_to_fill;
    int bytes_missing;
    int is_last_byte_of_line;
    int is_last_byte_of_data;
    int is_complete_line;
    int padding_for_alignment;

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
            printf("%04x ", address_width);
        }
        printf("%02x", buf[i]);

        if (i % 4 == 3)
        {
            printf(" ");
        }

        if ((i % 16 == 15) || (i == size - 1))
        {
            bytes_missing = 16 - ((i % 16) + 1);
            space_to_fill = bytes_missing * 2 + bytes_missing / 4;

            is_last_byte_of_line = (i % 16 == 15);
            is_last_byte_of_data = (i == size - 1);
            is_complete_line = (is_last_byte_of_line && is_last_byte_of_data);

            if (!is_complete_line)
            {
                space_to_fill++; // Add an extra space for incomplete addresses.
            }

            // Adjust space_to_fill if it's the last byte of data and we have a complete line.
            printf("%*s", is_complete_line ? space_to_fill - 1 : space_to_fill, "");

            if (!is_last_byte_of_line || is_last_byte_of_data) {
                printf(" "); // Add a space if it's not the end of a line or it is the last byte of data.
            }

            for (j = i - (i % 16); j <= i; j++) {
                if (j < size) {
                    printf("%c", isprint(buf[j]) ? buf[j] : '.');
                }
            }

            // Add padding for alignment if not the end of a line
            if (!is_last_byte_of_line) {
                padding_for_alignment = (16 - (i % 16)) - 1;
                printf("%*s", padding_for_alignment, "");
            }
            printf("\n");
        }

    }

    free(buf);
}