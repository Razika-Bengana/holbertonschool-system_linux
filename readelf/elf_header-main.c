#include "elf_header.h"

/**
 * main - the entry point
 *
 * program that reads and displays the ELF header of an ELF file
 *
 * this function takes an ELF file as an argument and reads its ELF header
 * to display various fields such as magic number, class, data encoding,
 * version, OS ABI, ABI version, file type, machine type, etc...;
 * it supports both ELF32 and ELF64 file formats;
 *
 * @argc: the number of command-line arguments
 * @argv: the command-line arguments --> argv[1] should be the ELF file path
 *
 * Return: 0 on success, 1 on failure
 */

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