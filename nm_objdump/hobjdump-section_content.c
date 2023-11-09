#include "hobjdump.h"

void print_section_content(FILE *file, void *shdr_generic, int is_64) {
    // Cast the generic pointer to the appropriate type
    Elf64_Shdr *shdr64 = is_64 ? (Elf64_Shdr *)shdr_generic : NULL;
    Elf32_Shdr *shdr32 = is_64 ? NULL : (Elf32_Shdr *)shdr_generic;

    // Determine size and offset based on the ELF class
    size_t size = is_64 ? shdr64->sh_size : shdr32->sh_size;
    uint64_t offset = is_64 ? shdr64->sh_offset : shdr32->sh_offset;
    uint64_t addr = is_64 ? shdr64->sh_addr : shdr32->sh_addr;

    unsigned char *buf = malloc(size);
    if (!buf) {
        perror("Failed to allocate memory for section content");
        return;
    }

    fseek(file, offset, SEEK_SET);
    fread(buf, 1, size, file);

    for (size_t i = 0; i < size; i++) {
        if (i % 16 == 0) {
            printf(" %04lx ", addr + i);
        }

        printf("%02x", buf[i]);

        if (i % 16 == 15) {
            printf("  ");
            for (size_t j = i - 15; j <= i; j++) {
                printf("%c", buf[j] >= 32 && buf[j] <= 126 ? buf[j] : '.');
            }
            printf("\n");
        }
    }

    free(buf);
}
