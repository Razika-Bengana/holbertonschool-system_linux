#include "elf_header.h"

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

