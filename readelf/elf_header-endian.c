#include "elf_header.h"

/**
 * swap_bytes - program that swaps the bytes of a given value
 *
 * this function swaps the bytes of the given value in-place, effectively
 * changing its endianness
 *
 * @value: a pointer to the value whose bytes are to be swapped
 * @size: the size of the value in bytes
 *
 * Return: nothing (void)
 */

void swap_bytes(void *value, size_t size)
{
	size_t i;
	uint8_t *ptr = value;

	for (i = 0; i < size / 2; i++)
	{
		uint8_t tmp = ptr[i];

		ptr[i] = ptr[size - 1 - i];
		ptr[size - 1 - i] = tmp;
	}
}

/**
 * system_is_big_endian - program that determines if the system is big-endian
 *
 * this function checks if the current system is big-endian
 *
 * Return: 1 if the system is big-endian, 0 otherwise
 */

int system_is_big_endian(void)
{
	uint16_t number = 1;
	char *numPtr = (char *)&number;

	return (numPtr[0] == 0);
}

/**
 * system_is_little_endian - program that determines if the system is
 * little-endian
 *
 * this function checks if the current system is little-endian;
 * it essentially calls system_is_big_endian and negates the result;
 *
 * Return: 1 if the system is little-endian, 0 otherwise
 */

int system_is_little_endian(void)
{
	return (!system_is_big_endian());
}
