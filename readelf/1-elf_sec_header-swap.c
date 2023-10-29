#include "elf_header.h"

/**
 * swap_uint16 - program that swaps the byte order of a 16-bit unsigned integer
 *
 * this function takes a 16-bit unsigned integer and returns it with
 * its byte order reversed;
 * for example, the value 0x1234 would be returned as 0x3412;
 *
 * @val: a 16-bit unsigned integer whose bytes are to be swapped
 *
 * Return: the 16-bit unsigned integer with its bytes swapped
 */

uint16_t swap_uint16(uint16_t val)
{
	return ((val << 8) | (val >> 8));
}


/**
 * swap_uint32 - program that swaps the byte order of a 32-bit unsigned integer
 *
 * this function takes a 32-bit unsigned integer and returns it with
 * its byte order reversed;
 * it uses bitwise shifts and OR operations to achieve the swap;
 *
 * @val: a 32-bit unsigned integer whose bytes are to be swapped
 *
 * Return: the 32-bit unsigned integer with its bytes swapped
 */

uint32_t swap_uint32(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return ((val << 16) | (val >> 16));
}

/**
 * swap_uint64 - program that swaps the byte order of a 64-bit unsigned integer
 *
 * this function takes a 64-bit unsigned integer and returns it with
 * its byte order reversed;
 * it uses bitwise shifts and OR operations to achieve the swap;
 *
 * @val: a 64-bit unsigned integer whose bytes are to be swapped
 *
 * Return: the 64-bit unsigned integer with its bytes swapped
 */

uint64_t swap_uint64(uint64_t val)
{
	val = ((val << 8) & 0xFF00FF00FF00FF00ULL) |
		((val >> 8) & 0x00FF00FF00FF00FFULL);
	val = ((val << 16) & 0xFFFF0000FFFF0000ULL) |
		((val >> 16) & 0x0000FFFF0000FFFFULL);
	return ((val << 32) | (val >> 32));
}
