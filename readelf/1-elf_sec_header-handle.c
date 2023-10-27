#include "elf_header.h"

void handle_elf(char *mem, size_t fileSize, int is_64bit, int is_big_endian)
{
    void *elf_header = (void *)mem;

    if (is_64bit && fileSize < sizeof(Elf64_Ehdr))
    {
        fprintf(stderr, "The file is too small to be a valid ELF-64 file.\n");
        return;
    }
    if (!is_64bit && fileSize < sizeof(Elf32_Ehdr))
    {
        fprintf(stderr, "The file is too small to be a valid ELF-32 file.\n");
        return;
    }

    uint64_t e_shoff = is_64bit ? ((Elf64_Ehdr *)elf_header)->e_shoff : ((Elf32_Ehdr *)elf_header)->e_shoff;
    uint16_t count = is_64bit ? ((Elf64_Ehdr *)elf_header)->e_shnum : ((Elf32_Ehdr *)elf_header)->e_shnum;

    if (is_big_endian)
    {
        e_shoff = is_64bit ? swap_uint64(e_shoff) : swap_uint32((uint32_t)e_shoff);
        count = swap_uint16(count);
    }

    size_t sh_size = is_64bit ? sizeof(Elf64_Shdr) : sizeof(Elf32_Shdr);
    if (e_shoff == 0 || count == 0 || e_shoff + sh_size * count > fileSize)
    {
        fprintf(stderr, "Section header table goes past end of file.\n");
        return;
    }

    void *shdr = mem + e_shoff;

    uint16_t e_shstrndx = is_64bit ? ((Elf64_Ehdr *)elf_header)->e_shstrndx : ((Elf32_Ehdr *)elf_header)->e_shstrndx;

    if (is_big_endian)
    {
        e_shstrndx = swap_uint16(e_shstrndx);
    }

    if (e_shstrndx >= count)
    {
        fprintf(stderr, "Invalid string table index.\n");
        return;
    }

    uint64_t strtab_offset = is_64bit ?
                             ((Elf64_Shdr *)((char *)shdr + e_shstrndx * sh_size))->sh_offset :
                             ((Elf32_Shdr *)((char *)shdr + e_shstrndx * sh_size))->sh_offset;

    if (is_big_endian)
    {
        strtab_offset = is_64bit ? swap_uint64(strtab_offset) : swap_uint32((uint32_t)strtab_offset);
    }

    if (strtab_offset == 0 || strtab_offset >= fileSize)
    {
        fprintf(stderr, "Invalid string table offset.\n");
        return;
    }

    char *strtab = (char *)(mem + strtab_offset);

    SectionHeaderInfo info = {
            .elf_header = elf_header,
            .shdr = shdr,
            .count = count,
            .strtab = strtab,
            .is_64bit = is_64bit
    };

    print_section_headers(&info);
}