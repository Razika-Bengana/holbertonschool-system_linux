#include "hobjdump.h"

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2)
    {
        fprintf(stderr, "hobjdump: %s [objfile ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++)
    {
        process_elf_file(argv[i]);
    }

    return (EXIT_SUCCESS);
}

void process_elf_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("File opening failed");
        return;
    }

    unsigned char e_ident[EI_NIDENT];
    if (fread(e_ident, 1, EI_NIDENT, file) != EI_NIDENT)
    {
        perror("Reading ELF identification failed");
        fclose(file);
        return;
    }

    if (memcmp(e_ident, ELFMAG, SELFMAG) != 0)
    {
        fprintf(stderr, "%s is not an ELF file.\n", filename);
        fclose(file);
        return;
    }

    if (e_ident[EI_CLASS] == ELFCLASS32)
    {
        process_elf_header32(file, filename);
    }
    else if (e_ident[EI_CLASS] == ELFCLASS64)
    {
        process_elf_header64(file, filename);
    }
    else
    {
        fprintf(stderr, "Unknown ELF class.\n");
        fclose(file);
    }
}

void process_elf_header32(FILE *file, const char *filename)
{
    Elf32_Ehdr header;

    rewind(file);

    if (fread(&header, 1, sizeof(header), file) != sizeof(header))
    {
        perror("Reading ELF header failed");
        fclose(file);
        return;
    }

    Elf32_Shdr *sections = malloc(header.e_shnum * sizeof(Elf32_Shdr));

    if (!sections)
    {
        perror("Failed to allocate memory for section headers");
        fclose(file);
        return;
    }

    fseek(file, header.e_shoff, SEEK_SET);

    if (fread(sections, sizeof(Elf32_Shdr), header.e_shnum, file) != header.e_shnum)
    {
        perror("Reading section headers failed");
        free(sections);
        fclose(file);
        return;
    }

    print_elf_info32(header, filename, sections);
    read_and_process_sections32(file, header);

    free(sections);
    fclose(file);
}


void process_elf_header64(FILE *file, const char *filename)
{
    Elf64_Ehdr header;

    rewind(file);

    if (fread(&header, 1, sizeof(header), file) != sizeof(header))
    {
        perror("Reading ELF header failed");
        fclose(file);
        return;
    }

    Elf64_Shdr *sections = malloc(header.e_shnum * sizeof(Elf64_Shdr));

    if (!sections)
    {
        perror("Failed to allocate memory for section headers");
        fclose(file);
        return;
    }

    fseek(file, header.e_shoff, SEEK_SET);

    if (fread(sections, sizeof(Elf64_Shdr), header.e_shnum, file) != header.e_shnum)
    {
        perror("Reading section headers failed");
        free(sections);
        fclose(file);
        return;
    }

    print_elf_info64(header, filename, sections);
    read_and_process_sections64(file, header);

    free(sections);
    fclose(file);
}


void print_elf_info32(Elf32_Ehdr header, const char *filename, Elf32_Shdr *sections)
{
    int is_little_endian = header.e_ident[EI_DATA] == ELFDATA2LSB;
    int is_big_endian = header.e_ident[EI_DATA] == ELFDATA2MSB;
    uint32_t flags = convert_endianness32(header.e_flags, is_little_endian, is_big_endian);
    uint32_t entry_point = convert_endianness32(header.e_entry, is_little_endian, is_big_endian);

    printf("%s:     file format elf32-i386\n", filename);

    printf("architecture: ");

    switch(header.e_machine)
    {
        case EM_386:
            printf("i386, ");
            break;
            /* Plus de cas après le checker */
        default:
            printf("unknown, ");
            break;
    }
    Elf_Ehdr header_union;
    header_union.elf32 = &header;
    flags = setFileFlags(header_union, sections, 0);
    print_flags(flags);
    printf("start address 0x%08x\n\n", entry_point);
}

void print_elf_info64(Elf64_Ehdr header, const char *filename, Elf64_Shdr *sections)
{
    int is_little_endian = header.e_ident[EI_DATA] == ELFDATA2LSB;
    int is_big_endian = header.e_ident[EI_DATA] == ELFDATA2MSB;
    uint32_t flags = convert_endianness64(header.e_flags, is_little_endian, is_big_endian);
    uint64_t entry_point = convert_endianness64(header.e_entry, is_little_endian, is_big_endian);

    printf("%s:     file format elf64-x86-64\n", filename);

    printf("architecture: ");

    switch(header.e_machine)
    {
        case EM_X86_64:
            printf("i386:x86-64, ");
            break;
            /* Plus de cas après le checker */
        default:
            printf("unknown, ");
            break;
    }
    Elf_Ehdr header_union;
    header_union.elf64 = &header;
    flags = setFileFlags(header_union, sections, 1);
    print_flags(flags);
    printf("start address 0x%016lx\n\n", entry_point);
}



void read_and_process_sections64(FILE *file, Elf64_Ehdr header)
{
    int i;

    Elf64_Shdr *sections = malloc(header.e_shnum * sizeof(Elf64_Shdr));

    if (!sections)
    {
        perror("Failed to allocate memory for section headers");
        return;
    }

    fseek(file, header.e_shoff, SEEK_SET);

    if (fread(sections, sizeof(Elf64_Shdr), header.e_shnum, file) != header.e_shnum)
    {
        perror("Reading section headers failed");
        free(sections);
        return;
    }

    Elf64_Shdr strtab = sections[header.e_shstrndx];

    char *section_names = malloc(strtab.sh_size);

    if (!section_names)
    {
        perror("Failed to allocate memory for section names");
        free(sections);
        return;
    }
    fseek(file, strtab.sh_offset, SEEK_SET);

    if (fread(section_names, 1, strtab.sh_size, file) != strtab.sh_size)
    {
        perror("Reading section name string table failed");
        free(section_names);
        free(sections);
        return;
    }

    for (i = 0; i < header.e_shnum; i++)
    {
        if (i == 0 || sections[i].sh_name == SHN_UNDEF)
        {
            continue;
        }
        char *section_name = &section_names[sections[i].sh_name];

        printf("Contents of section %s:\n", section_name);
        print_section_content(file, &sections[i], 1);
    }

    free(section_names);
    free(sections);
}

void read_and_process_sections32(FILE *file, Elf32_Ehdr header)
{
    int i;

    Elf32_Shdr *sections = malloc(header.e_shnum * sizeof(Elf32_Shdr));
    if (!sections)
    {
        perror("Failed to allocate memory for section headers");
        return;
    }

    fseek(file, header.e_shoff, SEEK_SET);
    if (fread(sections, sizeof(Elf32_Shdr), header.e_shnum, file) != header.e_shnum)
    {
        perror("Reading section headers failed");
        free(sections);
        return;
    }

    Elf32_Shdr strtab = sections[header.e_shstrndx];
    char *section_names = malloc(strtab.sh_size);
    if (!section_names)
    {
        perror("Failed to allocate memory for section names");
        free(sections);
        return;
    }
    fseek(file, strtab.sh_offset, SEEK_SET);
    if (fread(section_names, 1, strtab.sh_size, file) != strtab.sh_size)
    {
        perror("Reading section name string table failed");
        free(section_names);
        free(sections);
        return;
    }

    for (i = 0; i < header.e_shnum; i++)
    {
        if (i == 0 || sections[i].sh_name == SHN_UNDEF)
        {
            continue;
        }

        char *section_name = &section_names[sections[i].sh_name];

        printf("Contents of section %s:\n", section_name);
        print_section_content(file, &sections[i], 0);
    }

    free(section_names);
    free(sections);
}
