#include "hobjdump.h"

uint32_t convert_endianness32(uint32_t value, int is_little_endian, int is_big_endian)
{

    if (is_big_endian && (__BYTE_ORDER != __BIG_ENDIAN))
    {
        return (__builtin_bswap32(value));
    }

    else if (is_little_endian && (__BYTE_ORDER != __LITTLE_ENDIAN))
    {
        return (__builtin_bswap32(value));
    }
    /* Aucune conversion nécessaire si l'endianness du fichier correspond à celle de la machine hôte */
    return (value);
}


uint64_t convert_endianness64(uint64_t value, int is_little_endian, int is_big_endian)
{
    if (is_big_endian && (__BYTE_ORDER != __BIG_ENDIAN))
    {
        return (__builtin_bswap64(value));
    }
    else if (is_little_endian && (__BYTE_ORDER != __LITTLE_ENDIAN))
    {
        return (__builtin_bswap64(value));
    }
    /* Pareil */
    return (value);
}
