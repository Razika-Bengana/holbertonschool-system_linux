#include "elf_header.h"


#include <stdint.h>

void swap_bytes(void *value, size_t size)
{
    uint8_t *ptr = value;
    for (size_t i = 0; i < size / 2; i++)
    {
        uint8_t tmp = ptr[i];
        ptr[i] = ptr[size - 1 - i];
        ptr[size - 1 - i] = tmp;
    }
}

int system_is_big_endian()
{
    uint16_t number = 1;
    char *numPtr = (char*)&number;
    return (numPtr[0] == 0);
}

int system_is_little_endian()
{
    return !system_is_big_endian();
}


void read_elf_header(FILE *file, void *header, int is_elf64)
{
    if (is_elf64)
    {
        fread(header, sizeof(Elf64_Ehdr), 1, file);
    }
    else
    {
        fread(header, sizeof(Elf32_Ehdr), 1, file);
    }
}

void print_magic(void *header, int is_elf64)
{
    unsigned char *e_ident = is_elf64 ? ((Elf64_Ehdr *)header)->e_ident : ((Elf32_Ehdr *)header)->e_ident;
    printf("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; ++i)
    {
        printf("%02x ", e_ident[i]);
    }
    printf("\n");
}

void print_class(void *header, int is_elf64)
{
    unsigned char elf_class = is_elf64 ? ((Elf64_Ehdr *)header)->e_ident[EI_CLASS] : ((Elf32_Ehdr *)header)->e_ident[EI_CLASS];
    printf("  Class:                             ");
    switch (elf_class)
    {
        case ELFCLASS32:
            printf("ELF32\n");
            break;
        case ELFCLASS64:
            printf("ELF64\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }
}

void print_data(void *header, int is_elf64)
{
    unsigned char elf_data = is_elf64 ? ((Elf64_Ehdr *)header)->e_ident[EI_DATA] : ((Elf32_Ehdr *)header)->e_ident[EI_DATA];

    if ((system_is_big_endian() && elf_data == ELFDATA2LSB) || (system_is_little_endian() && elf_data == ELFDATA2MSB))
    {
        if (is_elf64)
        {
            Elf64_Ehdr *hdr64 = (Elf64_Ehdr *)header;
            swap_bytes(&hdr64->e_type, sizeof(hdr64->e_type));
            swap_bytes(&hdr64->e_machine, sizeof(hdr64->e_machine));
            swap_bytes(&hdr64->e_version, sizeof(hdr64->e_version));
            swap_bytes(&hdr64->e_entry, sizeof(hdr64->e_entry));
            swap_bytes(&hdr64->e_phoff, sizeof(hdr64->e_phoff));
            swap_bytes(&hdr64->e_shoff, sizeof(hdr64->e_shoff));
            swap_bytes(&hdr64->e_flags, sizeof(hdr64->e_flags));
            swap_bytes(&hdr64->e_ehsize, sizeof(hdr64->e_ehsize));
            swap_bytes(&hdr64->e_phentsize, sizeof(hdr64->e_phentsize));
            swap_bytes(&hdr64->e_phnum, sizeof(hdr64->e_phnum));
            swap_bytes(&hdr64->e_shentsize, sizeof(hdr64->e_shentsize));
            swap_bytes(&hdr64->e_shnum, sizeof(hdr64->e_shnum));
            swap_bytes(&hdr64->e_shstrndx, sizeof(hdr64->e_shstrndx));
        } else {
            Elf32_Ehdr *hdr32 = (Elf32_Ehdr *)header;
            swap_bytes(&hdr32->e_type, sizeof(hdr32->e_type));
            swap_bytes(&hdr32->e_machine, sizeof(hdr32->e_machine));
            swap_bytes(&hdr32->e_version, sizeof(hdr32->e_version));
            swap_bytes(&hdr32->e_entry, sizeof(hdr32->e_entry));
            swap_bytes(&hdr32->e_phoff, sizeof(hdr32->e_phoff));
            swap_bytes(&hdr32->e_shoff, sizeof(hdr32->e_shoff));
            swap_bytes(&hdr32->e_flags, sizeof(hdr32->e_flags));
            swap_bytes(&hdr32->e_ehsize, sizeof(hdr32->e_ehsize));
            swap_bytes(&hdr32->e_phentsize, sizeof(hdr32->e_phentsize));
            swap_bytes(&hdr32->e_phnum, sizeof(hdr32->e_phnum));
            swap_bytes(&hdr32->e_shentsize, sizeof(hdr32->e_shentsize));
            swap_bytes(&hdr32->e_shnum, sizeof(hdr32->e_shnum));
            swap_bytes(&hdr32->e_shstrndx, sizeof(hdr32->e_shstrndx));
        }
    }
    printf("  Data:                              ");
    switch (elf_data)
    {
        case ELFDATA2LSB:
            printf("2's complement, little endian\n");
            break;
        case ELFDATA2MSB:
            printf("2's complement, big endian\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }
}

void print_version(void *header, int is_elf64)
{
    unsigned char elf_version = is_elf64 ? ((Elf64_Ehdr *)header)->e_ident[EI_VERSION] : ((Elf32_Ehdr *)header)->e_ident[EI_VERSION];
    printf("  Version:                           ");
    switch (elf_version)
    {
        case EV_CURRENT:
            printf("1 (current)\n");
            break;
        default:
            printf("Unknown (%d)\n", elf_version);
            break;
    }
}


void print_os_abi(void *header, int is_elf64)
{
    unsigned char elf_osabi = is_elf64 ? ((Elf64_Ehdr *)header)->e_ident[EI_OSABI] : ((Elf32_Ehdr *)header)->e_ident[EI_OSABI];
    printf("  OS/ABI:                            ");
    switch (elf_osabi)
    {
        case ELFOSABI_NONE:
            printf("UNIX - System V\n");
            break;
        case ELFOSABI_LINUX:
            printf("Linux\n");
            break;
        case ELFOSABI_SOLARIS:
            printf("UNIX - Solaris\n");
            break;
        case ELFOSABI_NETBSD:
            printf("UNIX - NetBSD\n");
            break;
        case ELFOSABI_FREEBSD:
            printf("UNIX - FreeBSD\n");
            break;
        default:
            printf("<unknown: %x>\n", elf_osabi);
            break;
    }
}

void print_abi_version(void *header, int is_elf64)
{
    unsigned char elf_abi_version = is_elf64 ? ((Elf64_Ehdr *)header)->e_ident[EI_ABIVERSION] : ((Elf32_Ehdr *)header)->e_ident[EI_ABIVERSION];
    printf("  ABI Version:                       ");
    printf("%d\n", elf_abi_version);
}

void print_type(void *header, int is_elf64)
{
    Elf64_Half elf_type = is_elf64 ? ((Elf64_Ehdr *)header)->e_type : ((Elf32_Ehdr *)header)->e_type;
    printf("  Type:                              ");
    switch (elf_type)
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
            printf("Unknown (%d)\n", elf_type);
            break;
    }
}

void print_machine_type(void *header, int is_elf64)
{
    Elf64_Half machine_type = is_elf64 ? ((Elf64_Ehdr *)header)->e_machine : ((Elf32_Ehdr *)header)->e_machine;
    printf("  Machine:                           ");
    switch (machine_type)
    {
        case EM_386:
            printf("Intel 80386\n");
            break;
        case EM_X86_64:
            printf("Advanced Micro Devices X86-64\n");
            break;
        case EM_ARM:
            printf("ARM\n");
            break;
        case EM_AARCH64:
            printf("ARM AARCH64\n");
            break;
        case EM_MIPS:
            printf("MIPS R3000\n");
            break;
        case EM_SPARC:
            printf("SPARC\n");
            break;
        case EM_IA_64:
            printf("Intel IA-64\n");
            break;
        case EM_S390:
            printf("IBM S/390\n");
            break;
        default:
            printf("Unknown (%d)\n", machine_type);
            break;
    }
}



void print_header_details(void *header, int is_elf64)
{
    if (is_elf64)
    {
        Elf64_Ehdr *hdr = (Elf64_Ehdr *)header;
        printf("  Version:                           0x%x\n", hdr->e_version);
        printf("  Entry point address:               0x%lx\n", hdr->e_entry);
        printf("  Start of program headers:          %ld (bytes into file)\n", hdr->e_phoff);
        printf("  Start of section headers:          %ld (bytes into file)\n", hdr->e_shoff);
        printf("  Flags:                             0x%x\n", hdr->e_flags);
        printf("  Size of this header:               %d (bytes)\n", hdr->e_ehsize);
        printf("  Size of program headers:           %d (bytes)\n", hdr->e_phentsize);
        printf("  Number of program headers:         %d\n", hdr->e_phnum);
        printf("  Size of section headers:           %d (bytes)\n", hdr->e_shentsize);
        printf("  Number of section headers:         %d\n", hdr->e_shnum);
        printf("  Section header string table index: %d\n", hdr->e_shstrndx);
    }
    else
    {
        Elf32_Ehdr *hdr = (Elf32_Ehdr *)header;
        printf("  Version:                           0x%x\n", hdr->e_version);
        printf("  Entry point address:               0x%x\n", hdr->e_entry);
        printf("  Start of program headers:          %d (bytes into file)\n", hdr->e_phoff);
        printf("  Start of section headers:          %d (bytes into file)\n", hdr->e_shoff);
        printf("  Flags:                             0x%x\n", hdr->e_flags);
        printf("  Size of this header:               %d (bytes)\n", hdr->e_ehsize);
        printf("  Size of program headers:           %d (bytes)\n", hdr->e_phentsize);
        printf("  Number of program headers:         %d\n", hdr->e_phnum);
        printf("  Size of section headers:           %d (bytes)\n", hdr->e_shentsize);
        printf("  Number of section headers:         %d\n", hdr->e_shnum);
        printf("  Section header string table index: %d\n", hdr->e_shstrndx);
    }
}

int main(int argc, char *argv[])
{
    void *header = NULL;

    if (argc != 2)
    {
        printf("Usage: %s <ELF_FILE>\n", argv[0]);
        return (1);
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return (1);
    }

    unsigned char e_ident[EI_NIDENT];
    if (fread(e_ident, EI_NIDENT, 1, file) != 1)
    {
        perror("Error reading e_ident");
        fclose(file);
        return 1;
    }

    int is_elf64 = (e_ident[EI_CLASS] == ELFCLASS64);
    header = is_elf64 ? (void *)malloc(sizeof(Elf64_Ehdr)) : (void *)malloc(sizeof(Elf32_Ehdr));

    if (header == NULL)
    {
        perror("Error allocating memory for header");
        fclose(file);
        return 1;
    }

    fseek(file, 0, SEEK_SET);

    read_elf_header(file, header, is_elf64);

    printf("ELF Header:\n");
    print_magic(header, is_elf64);
    print_class(header, is_elf64);
    print_data(header, is_elf64);
    print_version(header, is_elf64);
    print_os_abi(header, is_elf64);
    print_abi_version(header, is_elf64);
    print_type(header, is_elf64);
    print_machine_type(header, is_elf64);
    print_header_details(header, is_elf64);

    free(header);
    fclose(file);

    return (0);
}

