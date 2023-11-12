#ifndef HOBJDUMP_H
#define HOBJDUMP_H


/**
#include <string.h>
#include <elf.h>
#include <byteswap.h>
#include <endian.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


/**
#define HAS_RELOC 0x01
#define EXEC_P    0x02
#define HAS_SYMS  0x10
#define D_PAGED   0x100
#define DYNAMIC   0x40

#define BSS_SECTION ".bss"
#define SHSTRTAB_SECTION ".shstrtab"
#define SYMTAB_SECTION ".symtab"
#define STRTAB_SECTION ".strtab"
#define TM_CLONE_TABLE_SECTION ".tm_clone_table"
#define REL_TEXT_SECTION ".rel.text"
#define REL_DATA_SECTION ".rel.data"
#define RELA_EH_FRAME_SECTION ".rela.eh_frame"
#define RELA_TEXT_STARTUP_SECTION ".rela.text.startup"


typedef union
{
    Elf32_Ehdr *elf32;
    Elf64_Ehdr *elf64;
} Elf_Ehdr;
*/


#endif /* HOBJDUMP_H */
