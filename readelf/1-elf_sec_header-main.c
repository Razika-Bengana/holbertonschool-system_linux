#include "elf_header.h"

/**
 * open_file - program that opens a file and returns its file descriptor
 *
 * @filename: the name of the file to open
 *
 * Return: the file descriptor if successful, or -1 if an error occurs
 */

int open_file(const char *filename)
{
	int fd = open(filename, O_RDONLY);

	if (fd == -1)
	{
		perror("open");
	}
	return (fd);
}


/**
 * check_file_stat - program that checks the file status using fstat
 *
 * @fd: the file descriptor of the file
 * @sb: a pointer to struct stat where file status will be stored
 *
 * Return: 0 on success, or -1 if an error occurs
 */

int check_file_stat(int fd, struct stat *sb)
{
	if (fstat(fd, sb) == -1)
	{
		perror("fstat");
		return (-1);
	}
	return (0);
}


/**
 * map_file - program that maps the file into memory
 *
 * @fd: the file descriptor of the file
 * @sb: a pointer to struct stat containing file status
 *
 * Return: a pointer to the mapped memory if successful,
 *         or MAP_FAILED if an error occurs
 */

void *map_file(int fd, struct stat *sb)
{
	void *mem = mmap(NULL, sb->st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if (mem == MAP_FAILED)
	{
		perror("mmap");
	}
	return (mem);
}


/**
 * process_elf_file - program that handles the mapped ELF file
 *
 * this function determines whether the file is ELF32 or ELF64 and calls
 * the corresponding function to handle it further;
 * it also extracts file endianness;
 *
 * @mem: a pointer to the mapped memory of the file
 * @sb: a pointer to struct stat containing file status
 *
 * Return: nothing (void)
 */

void process_elf_file(void *mem, struct stat *sb)
{
	unsigned char *e_ident = ((unsigned char *)mem);
	int file_class = e_ident[EI_CLASS];
	int file_endianness = e_ident[EI_DATA];
	int is_big_endian = (file_endianness == ELFDATA2MSB) ? 1 : 0;

	if (file_class == ELFCLASS32)
	{
		handle_elf(mem, (size_t)sb->st_size, 0, is_big_endian);
	}
	else if (file_class == ELFCLASS64)
	{
		handle_elf(mem, (size_t)sb->st_size, 1, is_big_endian);
	}
	else
	{
		fprintf(stderr, "Unsupported ELF file class.\n");
	}
}


/**
 * main - the entry point
 *
 * the main entry point of the ELF file processor
 *
 * this function serves as the orchestrator for the entire process of reading
 * and analyzing an ELF (Executable and Linkable Format) file
 *
 * Steps are as follows:
 *
 * 1. checks if the filename has been provided as a command-line argument;
 *    if not, exits with an error
 * 2. opens the specified file and obtains its file descriptor;
 *    if the file cannot be opened, exits with an error
 * 3. retrieves file metadata using fstat and validates it;
 *    if this step fails, closes the file and exits with an error
 * 4. maps the file into memory for easier manipulation;
 *    if the mapping fails, closes the file and exits with an error
 * 5. delegates the handling of the ELF file based on its type
 *    (32-bit or 64-bit) and endianness
 * 6. unmaps the file from memory and closes the file descriptor
 *
 * @argc: the number of command-line arguments
 * @argv: an array of command-line arguments
 *
 * Return: 0 upon successful execution,
 *         and if an error occurs at any stage, returns 1
 */

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
		return (1);
	}
	int fd = open_file(argv[1]);

	if (fd == -1)
		return (1);

	struct stat sb;

	if (check_file_stat(fd, &sb) == -1)
	{
		close(fd);
		return (1);
	}
	if (sb.st_size < (off_t)sizeof(Elf64_Ehdr))
	{
		fprintf(stderr, "The file is too small to be a valid ELF file.\n");
		close(fd);
		return (1);
	}
	void *mem = map_file(fd, &sb);

	if (mem == MAP_FAILED)
	{
		close(fd);
		return (1);
	}

	process_elf_file(mem, &sb);

	if (munmap(mem, sb.st_size) == -1)
	{
		perror("munmap");
		return (1);
	}
	close(fd);
	return (0);
}
