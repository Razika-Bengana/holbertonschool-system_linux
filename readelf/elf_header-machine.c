#include "elf_header.h"

/**
 * print_machine_type - program that prints the machine type specified
 * in the ELF header
 *
 * this function takes a pointer to an ELF header and a flag indicating
 * whether the ELF is 64-bit or 32-bit;
 * it then prints the machine architecture for which the ELF file was built;
 * for example, it could print "Intel 80386" for an ELF built for an
 * x86 machine or "Advanced Micro Devices X86-64" for an ELF64 file
 *
 * @header: a pointer to the ELF header, which could be either Elf64_Ehdr
 *          or Elf32_Ehdr
 * @is_elf64: a flag to specify whether the ELF file is 64-bit (1)
 *            or 32-bit (0)
 *
 * Return: nothing (void)
 */

void print_machine_type(void *header, int is_elf64)
{
	Elf64_Half machine_type = is_elf64 ?
		((Elf64_Ehdr *)header)->e_machine :
		((Elf32_Ehdr *)header)->e_machine;

	printf("  Machine:                           ");
	switch (machine_type)
	{
	case EM_386:
		printf("Intel 80386\n");
		break;
	case EM_X86_64:
		printf("Advanced Micro Devices X86-64\n");
		break;
	case EM_ARM:
		printf("ARM\n");
		break;
	case EM_AARCH64:
		printf("ARM AARCH64\n");
		break;
	case EM_MIPS:
		printf("MIPS R3000\n");
		break;
	case EM_SPARC:
		printf("Sparc\n");
		break;
	case EM_IA_64:
		printf("Intel IA-64\n");
		break;
	case EM_S390:
		printf("IBM S/390\n");
		break;
	default:
		printf("Unknown (%d)\n", machine_type);
		break;
	}
}
