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