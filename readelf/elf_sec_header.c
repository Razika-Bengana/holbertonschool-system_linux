#include "elf_header.h"


void print_flags(unsigned long flags, char *buf)
{
    int index = 0;

    if (flags & SHF_WRITE)
    {
        buf[index++] = 'W';
    }

    if (flags & SHF_ALLOC)
    {
        buf[index++] = 'A';
    }

    if (flags & SHF_EXECINSTR)
    {
        buf[index++] = 'X';
    }

    if (flags & SHF_MERGE)
    {
        buf[index++] = 'M';
    }

    if (flags & SHF_STRINGS)
    {
        buf[index++] = 'S';
    }

    if (flags & SHF_INFO_LINK)
    {
        buf[index++] = 'I';
    }

    if (flags & SHF_LINK_ORDER)
    {
        buf[index++] = 'L';
    }

    if (flags & SHF_OS_NONCONFORMING)
    {
        buf[index++] = 'O';
    }

    if (flags & SHF_EXCLUDE)
    {
        buf[index++] = 'E';
    }

    buf[index] = '\0';
}




char *section_type_to_string(uint32_t type)
{
    switch (type)
    {
        case SHT_NULL: return "NULL";
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
        case SHT_PROGBITS: return "PROGBITS";
        case 0x6fffffff: return "VERSYM";
        case 0x6ffffffc: return "VERDEF";
        case 0x6ffffffE: return "VERNEED";
        case 0x6FFFFFF6: return "GNU_HASH";
        case 0x60000000: return "LOOS";
        case 0x60000000 + 0xFFFFFF3: return "LOOS+ffffff3";
        case 0x60000000 + 0xFFFFFF1: return "LOOS+ffffff1";
        case 0x60000001: return "LOUSER";
        default: return "UNKNOWN";
    }
}



void print_section_headers(SectionHeaderInfo *info)
{
    int num_headers;
    Elf64_Off offset;
    char flag_str[5];
    int isLittleEndian;

    union {
        uint64_t sh_flags_64;
        uint32_t sh_flags_32;
    } sh_flags_converted;

    if (info->is_64bit)
    {
        Elf64_Ehdr *ehdr = (Elf64_Ehdr *)info->elf_header;
        isLittleEndian = (ehdr->e_ident[EI_DATA] == ELFDATA2LSB);

        num_headers = isLittleEndian ? ehdr->e_shnum : ntohs(ehdr->e_shnum);
        offset = isLittleEndian ? ehdr->e_shoff : ntohl(ehdr->e_shoff);

        printf("There are %d section headers, starting at offset 0x%lx:\n", num_headers, (unsigned long)offset);
        printf("\n");
        printf("Section Headers:\n");
        printf("  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");

        for (int i = 0; i < info->count; i++)
        {
            uint32_t sh_name_converted, sh_type_converted;
            uint64_t sh_addr_converted, sh_offset_converted, sh_size_converted, sh_entsize_converted, sh_addralign_converted;
            uint32_t sh_link_converted, sh_info_converted;

            Elf64_Shdr *section = &((Elf64_Shdr *)info->shdr)[i];

            if (!isLittleEndian)
            {
                sh_name_converted = swap_uint32(section->sh_name);
                sh_type_converted = swap_uint32(section->sh_type);

                sh_addr_converted = swap_uint64(section->sh_addr);
                sh_offset_converted = swap_uint64(section->sh_offset);
                sh_size_converted = swap_uint64(section->sh_size);
                sh_entsize_converted = swap_uint64(section->sh_entsize);

                sh_flags_converted.sh_flags_64 = swap_uint64(section->sh_flags);

                sh_link_converted = swap_uint32(section->sh_link);
                sh_info_converted = swap_uint32(section->sh_info);
                sh_addralign_converted = swap_uint64(section->sh_addralign);
            }
            else
            {
                sh_name_converted = section->sh_name;
                sh_type_converted = section->sh_type;

                sh_addr_converted = section->sh_addr;
                sh_offset_converted = section->sh_offset;
                sh_size_converted = section->sh_size;
                sh_entsize_converted = section->sh_entsize;
                sh_flags_converted.sh_flags_64 = section->sh_flags;
                sh_link_converted = section->sh_link;
                sh_info_converted = section->sh_info;
                sh_addralign_converted = section->sh_addralign;
            }

            print_flags(sh_flags_converted.sh_flags_64, flag_str);

            printf("  [%2d] %-17s %-15s %016lx %06lx %06lx %02lx %3s %2d %3u %2lu\n",
                   i,
                   info->strtab + sh_name_converted,
                   section_type_to_string(sh_type_converted),
                   (unsigned long)sh_addr_converted,
                   (unsigned long)sh_offset_converted,
                   (unsigned long)sh_size_converted,
                   (unsigned long)sh_entsize_converted,
                   flag_str,
                   (int)sh_link_converted,
                   (unsigned int)sh_info_converted,
                   (unsigned long)sh_addralign_converted);
        }

        printf("Key to Flags:\n");
        printf("  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)\n");
        printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
        printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
    }

    else
    {
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)info->elf_header;
        isLittleEndian = (ehdr->e_ident[EI_DATA] == ELFDATA2LSB);

        num_headers = isLittleEndian ? ehdr->e_shnum : ntohs(ehdr->e_shnum);
        offset = isLittleEndian ? ehdr->e_shoff : ntohl(ehdr->e_shoff);

        printf("There are %d section headers, starting at offset 0x%lx:\n", num_headers, offset);
        printf("\n");
        printf("Section Headers:\n");
        printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");

        for (int i = 0; i < info->count; i++)
        {
            uint32_t sh_name_converted, sh_type_converted;
            uint32_t sh_addr_converted, sh_offset_converted, sh_size_converted, sh_entsize_converted, sh_addralign_converted, sh_link_converted, sh_info_converted;

            Elf32_Shdr *section = &((Elf32_Shdr *)info->shdr)[i];

            if (!isLittleEndian)
            {
                sh_name_converted = ntohl(section->sh_name);
                sh_type_converted = ntohl(section->sh_type);

                sh_addr_converted = ntohl(section->sh_addr);
                sh_offset_converted = ntohl(section->sh_offset);
                sh_size_converted = ntohl(section->sh_size);
                sh_entsize_converted = ntohl(section->sh_entsize);
                sh_flags_converted.sh_flags_32 = ntohl(section->sh_flags);
                sh_link_converted = ntohl(section->sh_link);
                sh_info_converted = ntohl(section->sh_info);
                sh_addralign_converted = ntohl(section->sh_addralign);
            }
            else
            {
                sh_name_converted = section->sh_name;
                sh_type_converted = section->sh_type;

                sh_addr_converted = section->sh_addr;
                sh_offset_converted = section->sh_offset;
                sh_size_converted = section->sh_size;
                sh_entsize_converted = section->sh_entsize;
                sh_flags_converted.sh_flags_32 = section->sh_flags;
                sh_link_converted = section->sh_link;
                sh_info_converted = section->sh_info;
                sh_addralign_converted = section->sh_addralign;
            }

            print_flags(sh_flags_converted.sh_flags_32, flag_str);

            printf("  [%2d] %-17s %-15s %08lx %06lx %06lx %02lx %3s %2lu %3lu %2lu\n",
                   i,
                   info->strtab + sh_name_converted,
                   section_type_to_string(sh_type_converted),
                   (unsigned long) sh_addr_converted,
                   (unsigned long) sh_offset_converted,
                   (unsigned long) sh_size_converted,
                   (unsigned long) sh_entsize_converted,
                   flag_str,
                   (unsigned long) sh_link_converted,
                   (unsigned long) sh_info_converted,
                   (unsigned long) sh_addralign_converted);
        }

        printf("Key to Flags:\n");
        printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
        printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
        printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
    }
}


uint16_t swap_uint16(uint16_t val)
{
    return (val << 8) | (val >> 8 );
}

uint32_t swap_uint32(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

uint64_t swap_uint64(uint64_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}


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
        close(fd);
        return (1);
    }

    if (sb.st_size < (off_t)sizeof(Elf64_Ehdr))
    {
        fprintf(stderr, "The file is too small to be a valid ELF file.\n");
        close(fd);
        return (1);
    }

    void *mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mem == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return (1);
    }

    unsigned char *e_ident = ((unsigned char *)mem);
    int file_class = e_ident[EI_CLASS];
    int file_endianness = e_ident[EI_DATA];
    int is_big_endian = (file_endianness == ELFDATA2MSB) ? 1 : 0;

    if (file_class == ELFCLASS32)
    {
        handle_elf(mem, (size_t)sb.st_size, 0, is_big_endian);
    }
    else if (file_class == ELFCLASS64)
    {
        handle_elf(mem, (size_t)sb.st_size, 1, is_big_endian);
    }
    else
    {
        fprintf(stderr, "Unsupported ELF file class.\n");
        munmap(mem, sb.st_size);
        close(fd);
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

