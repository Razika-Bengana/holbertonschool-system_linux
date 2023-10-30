#include "elf_header.h"

int is_little_endian()
{
    uint32_t x = 1;
    unsigned char *c = (unsigned char*)&x;
    return *c == 1;
}

uint32_t convert_endianness_32(uint32_t val, unsigned char endianness)
{
    if (is_little_endian() == (endianness == ELFDATA2LSB))
    {
        /* Si la machine a le même boutisme que le fichier, alors aucune conversion */
        return (val);
    }
    else
    {
        return ((val & 0xFF000000) >> 24) |
               ((val & 0x00FF0000) >> 8)  |
               ((val & 0x0000FF00) << 8)  |
               ((val & 0x000000FF) << 24);
    }
}

uint64_t convert_endianness_64(uint64_t val, unsigned char endianness)
{
    if (is_little_endian() == (endianness == ELFDATA2LSB))
    {
        /* pareil */
        return (val);
    }
    else
    {
        return ((val & 0xFF00000000000000ULL) >> 56) |
               ((val & 0x00FF000000000000ULL) >> 40) |
               ((val & 0x0000FF0000000000ULL) >> 24) |
               ((val & 0x000000FF00000000ULL) >> 8)  |
               ((val & 0x00000000FF000000ULL) << 8)  |
               ((val & 0x0000000000FF0000ULL) << 24) |
               ((val & 0x000000000000FF00ULL) << 40) |
               ((val & 0x00000000000000FFULL) << 56);
    }
}

uint16_t convert_endianness_16(uint16_t val, unsigned char endianness)
{
    if (is_little_endian() == (endianness == ELFDATA2LSB))
    {
        /* pareil */
        return (val);
    }
    else
    {
        return ((val & 0xFF00) >> 8) |
               ((val & 0x00FF) << 8);
    }
}
