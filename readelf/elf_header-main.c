#include "elf_header.h"

/**
 * initialize_elf_header - program that initializes and read the ELF header
 * from a file
 *
 * this function reads the first few bytes of the file to determine
 * whether it's an ELF32 or ELF64 file;
 * it then dynamically allocates memory for the ELF header and reads it
 * into this memory
 *
 * @file: a pointer to the file to read from
 * @is_elf64_ptr: a pointer to an integer where the determination of
 *                the ELF class (32/64-bit) will be stored
 *
 * Return: a pointer to the dynamically allocated ELF header,
 *         or NULL if the operation fails
 */

void *initialize_elf_header(FILE *file, int *is_elf64_ptr)
{
	unsigned char e_ident[EI_NIDENT];

	if (fread(e_ident, EI_NIDENT, 1, file) != 1)
	{
		perror("Error reading e_ident");
		return (NULL);
	}

	*is_elf64_ptr = (e_ident[EI_CLASS] == ELFCLASS64);
	void *header = *is_elf64_ptr ? (void *)malloc(sizeof(Elf64_Ehdr)) :
		(void *)malloc(sizeof(Elf32_Ehdr));

	if (header == NULL)
	{
		perror("Error allocating memory for header");
	}

	fseek(file, 0, SEEK_SET);
	read_elf_header(file, header, *is_elf64_ptr);
	return (header);
}


/**
 * print_elf_header_details - program that prints the details of the ELF header
 *
 * this function takes a pointer to an ELF header and prints its details,
 * such as the magic number, class, data encoding, etc...;
 * it also determines whether the header is for an ELF32 or ELF64 file
 * and prints the details accordingly
 *
 * @header: a pointer to the ELF header to print
 * @is_elf64: an integer flag that indicates whether the file is ELF32 or ELF64
 *
 * Return: nothing (void)
 */

void print_elf_header_details(void *header, int is_elf64)
{
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
}


/**
 * main - the entry point
 *
 * program that reads and displays the ELF header of an ELF file
 *
 * this function takes the path of an ELF file as a command-line argument;
 * it opens the file, reads the ELF header, and then calls a series of
 * functions to print the details of this header;
 * it supports both ELF32 and ELF64 file formats;
 *
 * @argc: number of command-line arguments.
 * @argv: command-line arguments --> argv[1] should contain the ELF file path
 *
 * Return: 0 on success, 1 on failure
 */

int main(int argc, char *argv[])
{
	FILE *file;
	int is_elf64;

	if (argc != 2)
	{
		printf("Usage: %s <ELF_FILE>\n", argv[0]);
		return (1);
	}

	file = fopen(argv[1], "rb");

	if (file == NULL)
	{
		perror("Error opening file");
		return (1);
	}

	void *header = initialize_elf_header(file, &is_elf64);

	if (header == NULL)
	{
		fclose(file);
		return (1);
	}

	print_elf_header_details(header, is_elf64);

	free(header);
	fclose(file);

	return (0);
}
