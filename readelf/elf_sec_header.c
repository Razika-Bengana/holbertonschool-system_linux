#include "elf_header.h"


void print_flags(unsigned long flags, char *buf)
{
    buf[0] = (flags & SHF_WRITE) ? 'W' : ' ';
    buf[1] = (flags & SHF_ALLOC) ? 'A' : ' ';
    buf[2] = (flags & SHF_EXECINSTR) ? 'X' : ' ';
    buf[3] = '\0';
}

char* section_type_to_string(uint32_t type)
{
    switch(type)
    {
        case SHT_NULL: return "NULL";
        case SHT_PROGBITS: return "PROGBITS";
        case SHT_SYMTAB: return "SYMTAB";
        case SHT_STRTAB: return "STRTAB";
        case SHT_RELA: return "RELA";
        case SHT_HASH: return "HASH";
        case SHT_DYNAMIC: return "DYNAMIC";
        default: return "UNKNOWN";
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        return (1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return (1);
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        perror("fstat");
        return (1);
    }

    void *mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mem == MAP_FAILED)
    {
        perror("mmap");
        return (1);
    }

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) mem;
    Elf64_Shdr *shdr = (Elf64_Shdr *) ((char *)mem + ehdr->e_shoff);
    char *strtab = (char *) ((char *)mem + shdr[ehdr->e_shstrndx].sh_offset);

    printf("There are %d section headers, starting at offset 0x%lx:\n\n", ehdr->e_shnum, ehdr->e_shoff);
    char flag_str[4];
    printf("Section Headers:\n");
    printf("  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");
    for (int i = 0; i < ehdr->e_shnum; i++)
    {
        Elf64_Shdr *section = &shdr[i];
        print_flags(section->sh_flags, flag_str);

        printf("  [%2d] %-17s ", i, &strtab[section->sh_name]);
        printf("%-15s ", section_type_to_string(section->sh_type));
        printf("%016lx ", section->sh_addr);
        printf("%06lx ", section->sh_offset);
        printf("%06lx ", section->sh_size);
        printf("%02lx ", section->sh_entsize);
        printf("%s ", flag_str);
        printf("%3d ", section->sh_link);
        printf("%3d ", section->sh_info);
        printf("%2lx\n", section->sh_addralign);
    }

    printf("Key to Flags:\n");
    printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
    printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
    printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");

    if (munmap(mem, sb.st_size) == -1)
    {
        perror("munmap");
        return (1);
    }

    if (close(fd) == -1)
    {
        perror("close");
        return (1);
    }

    return (0);
}
