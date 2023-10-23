#include "elf_header.h"

void read_elf_header(int fd, Elf64_Ehdr *header);
void print_magic(Elf64_Ehdr *header);
void print_class(Elf64_Ehdr *header);
void print_data(Elf64_Ehdr *header);
void print_version(Elf64_Ehdr *header);
void print_os_abi(Elf64_Ehdr *header);
void print_abi_version(Elf64_Ehdr *header);
void print_type(Elf64_Ehdr *header);
void print_machine_type(Elf64_Ehdr *header);
void print_header_details(Elf64_Ehdr *header);

int main(int argc, char **argv)
{
    int fd;
    Elf64_Ehdr header;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    read_elf_header(fd, &header);

    printf("ELF Header:\n");
    print_magic(&header);
    print_class(&header);
    print_data(&header);
    print_version(&header);
    print_os_abi(&header);
    print_abi_version(&header);
    print_type(&header);
    print_machine_type(&header);
    print_header_details(&header);

    close(fd);
    return 0;
}

void read_elf_header(int fd, Elf64_Ehdr *header)
{
    if (read(fd, header, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr))
    {
        perror("Error reading ELF header");
        exit(EXIT_FAILURE);
    }
}

void print_magic(Elf64_Ehdr *header)
{
    int i;
    printf("  Magic:   ");
    for (i = 0; i < EI_NIDENT; i++)
    {
        printf("%02x ", header->e_ident[i]);
    }
    printf("\n");
}

void print_class(Elf64_Ehdr *header)
{
    printf("  Class:                             ELF%d\n",
           header->e_ident[EI_CLASS] == ELFCLASS32 ? 32 : 64);
}

void print_data(Elf64_Ehdr *header)
{
    printf("  Data:                              %s\n",
           header->e_ident[EI_DATA] == ELFDATA2LSB ? "2's complement, little endian" : "Invalid");
}

void print_version(Elf64_Ehdr *header)
{
    printf("  Version:                           %d (current)\n", header->e_ident[EI_VERSION]);
}

void print_os_abi(Elf64_Ehdr *header)
{
    printf("  OS/ABI:                            ");
    switch (header->e_ident[EI_OSABI])
    {
        case ELFOSABI_NONE:
            printf("UNIX - System V\n");
            break;
        case ELFOSABI_HPUX:
            printf("UNIX - HP-UX\n");
            break;
        case ELFOSABI_NETBSD:
            printf("UNIX - NetBSD\n");
            break;
        case ELFOSABI_LINUX:
            printf("UNIX - Linux\n");
            break;
        case ELFOSABI_SOLARIS:
            printf("UNIX - Solaris\n");
            break;
        case ELFOSABI_IRIX:
            printf("UNIX - IRIX\n");
            break;
        case ELFOSABI_FREEBSD:
            printf("UNIX - FreeBSD\n");
            break;
        case ELFOSABI_TRU64:
            printf("UNIX - TRU64\n");
            break;
        default:
            printf("Unknown\n");
    }
}


void print_abi_version(Elf64_Ehdr *header)
{
    printf("  ABI Version:                       %d\n", header->e_ident[EI_ABIVERSION]);
}

void print_type(Elf64_Ehdr *header)
{
    printf("  Type:                              ");
    switch (header->e_type)
    {
        case ET_NONE:
            printf("NONE (No file type)\n");
            break;
        case ET_REL:
            printf("REL (Relocatable file)\n");
            break;
        case ET_EXEC:
            printf("EXEC (Executable file)\n");
            break;
        case ET_DYN:
            printf("DYN (Shared object file)\n");
            break;
        case ET_CORE:
            printf("CORE (Core file)\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }
}

void print_machine_type(Elf64_Ehdr *header)
{
    printf("  Machine:                           ");
    switch (header->e_machine)
    {
        case EM_NONE:
            printf("No machine\n");
            break;
        case EM_M32:
            printf("AT&T WE 32100\n");
            break;
        case EM_SPARC:
            printf("SPARC\n");
            break;
        case EM_386:
            printf("Intel 80386\n");
            break;
        case EM_68K:
            printf("Motorola 68000\n");
            break;
        case EM_88K:
            printf("Motorola 88000\n");
            break;
        case EM_860:
            printf("Intel 80860\n");
            break;
        case EM_MIPS:
            printf("MIPS RS3000 Big-Endian\n");
            break;
        case EM_PARISC:
            printf("Hewlett-Packard PA-RISC\n");
            break;
        case EM_VPP500:
            printf("Fujitsu VPP500\n");
            break;
        case EM_SPARC32PLUS:
            printf("SPARC with enhanced instruction set\n");
            break;
        case EM_PPC:
            printf("PowerPC\n");
            break;
        case EM_PPC64:
            printf("PowerPC 64-bit\n");
            break;
        case EM_S390:
            printf("IBM S390\n");
            break;
        case EM_ARM:
            printf("ARM\n");
            break;
        case EM_SH:
            printf("Renesas SH\n");
            break;
        case EM_SPARCV9:
            printf("SPARC V9 64-bit\n");
            break;
        case EM_IA_64:
            printf("Intel Itanium\n");
            break;
        case EM_X86_64:
            printf("Advanced Micro Devices X86-64\n");
            break;
        case EM_AARCH64:
            printf("ARM64\n");
            break;
        default:
            printf("Unknown\n");
    }
}


void print_header_details(Elf64_Ehdr *header)
{
    printf("  Version:                           0x%x\n", header->e_version);
    printf("  Entry point address:               0x%lx\n", header->e_entry);
    printf("  Start of program headers:          %ld (bytes into file)\n", header->e_phoff);
    printf("  Start of section headers:          %ld (bytes into file)\n", header->e_shoff);
    printf("  Flags:                             0x%x\n", header->e_flags);
    printf("  Size of this header:               %d (bytes)\n", header->e_ehsize);
    printf("  Size of program headers:           %d (bytes)\n", header->e_phentsize);
    printf("  Number of program headers:         %d\n", header->e_phnum);
    printf("  Size of section headers:           %d (bytes)\n", header->e_shentsize);
    printf("  Number of section headers:         %d\n", header->e_shnum);
    printf("  Section header string table index: %d\n", header->e_shstrndx);
}
