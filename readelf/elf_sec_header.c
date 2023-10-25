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
    switch (type)
    {
        case SHT_NULL: return "NULL";
        case SHT_PROGBITS: return "PROGBITS";
        case SHT_SYMTAB: return "SYMTAB";
        case SHT_STRTAB: return "STRTAB";
        case SHT_RELA: return "RELA";
        case SHT_HASH: return "HASH";
        case SHT_DYNAMIC: return "DYNAMIC";
        case SHT_NOTE: return "NOTE";
        case SHT_NOBITS: return "NOBITS";
        case SHT_REL: return "REL";
        case SHT_SHLIB: return "SHLIB";
        case SHT_DYNSYM: return "DYNSYM";
        case SHT_INIT_ARRAY: return "INIT_ARRAY";
        case SHT_FINI_ARRAY: return "FINI_ARRAY";
        case SHT_PREINIT_ARRAY: return "PREINIT_ARRAY";
        case SHT_GROUP: return "GROUP";
        case SHT_SYMTAB_SHNDX: return "SYMTAB_SHNDX";
        case SHT_NUM: return "NUM";
        default: return "UNKNOWN";
    }
}


void print_section_headers(SectionHeaderInfo *info)
{
    int num_headers;
    Elf64_Off offset;
    char flag_str[4];

    if (info->is_64bit)
    {
        Elf64_Ehdr *ehdr = (Elf64_Ehdr *)info->elf_header;
        num_headers = ehdr->e_shnum;
        offset = ehdr->e_shoff;
    }
    else
    {
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)info->elf_header;
        num_headers = ehdr->e_shnum;
        offset = ehdr->e_shoff;
    }

    printf("There are %d section headers, starting at offset 0x%lx:\n", num_headers, offset);
    printf("\n");
    printf("Section Headers:\n");
    printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");

    for (int i = 0; i < info->count; i++)
    {
        if (info->is_64bit)
        {
            Elf64_Shdr *section = &((Elf64_Shdr *)info->shdr)[i];
            print_flags(section->sh_flags, flag_str);
            printf("  [%2d] %-17s %-15s %08lx %06lx %06lx %02lx  %-3s %3d %3u %2lu\n",
                   i,
                   info->strtab + section->sh_name,
                   section_type_to_string(section->sh_type),
                   section->sh_addr,
                   section->sh_offset,
                   section->sh_size,
                   section->sh_entsize,
                   flag_str,
                   section->sh_link,
                   section->sh_info,
                   section->sh_addralign);
        }
        else
        {
            Elf32_Shdr *section = &((Elf32_Shdr *)info->shdr)[i];
            print_flags(section->sh_flags, flag_str);
            printf("  [%2d] %-17s %-15s %08lx %06lx %06lx %02lx  %-3s %3lu %3lu %2lu\n",
                   i,
                   info->strtab + section->sh_name,
                   section_type_to_string(section->sh_type),
                   (unsigned long) section->sh_addr,
                   (unsigned long) section->sh_offset,
                   (unsigned long) section->sh_size,
                   (unsigned long) section->sh_entsize,
                   flag_str,
                   (unsigned long) section->sh_link,
                   (unsigned long) section->sh_info,
                   (unsigned long) section->sh_addralign);
        }
    }

    printf("Key to Flags:\n");
    printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
    printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
    printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
}


void handle_elf(char *mem, size_t fileSize, int is_64bit)
{
    size_t sh_size = is_64bit ? sizeof(Elf64_Shdr) : sizeof(Elf32_Shdr);
    void *elf_header = (void *) mem;
    void *shdr = mem + (is_64bit ? ((Elf64_Ehdr *)elf_header)->e_shoff : ((Elf32_Ehdr *)elf_header)->e_shoff);
    int count = is_64bit ? ((Elf64_Ehdr *)elf_header)->e_shnum : ((Elf32_Ehdr *)elf_header)->e_shnum;

    if ((size_t)(((char *)shdr - mem) + sh_size * count) > fileSize)
    {
        fprintf(stderr, "Section header table goes past end of file.\n");
        return;
    }

    char *strtab = (char *)(mem + (is_64bit ? ((Elf64_Shdr *)shdr)[((Elf64_Ehdr *)elf_header)->e_shstrndx].sh_offset
                                            : ((Elf32_Shdr *)shdr)[((Elf32_Ehdr *)elf_header)->e_shstrndx].sh_offset));

    SectionHeaderInfo info = {
            .elf_header = elf_header,
            .shdr = shdr,
            .count = count,
            .strtab = strtab,
            .is_64bit = is_64bit
    };

    print_section_headers(&info);
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
    if (ehdr->e_ident[EI_CLASS] == ELFCLASS32) {
        handle_elf((char *)mem, (size_t)sb.st_size, 0);
    } else if (ehdr->e_ident[EI_CLASS] == ELFCLASS64) {
        handle_elf((char *)mem, (size_t)sb.st_size, 1);
    } else {
        fprintf(stderr, "Unsupported ELF file class.\n");
        return (1);
    }

    if (munmap(mem, sb.st_size) == -1)
    {
        perror("munmap");
        return (1);
    }

    close(fd);
    return (0);
}

