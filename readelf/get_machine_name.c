#include "header.h"

/**
 * get_machine_name - Convert machine code to string
 * @e_machine: Machine code
 * Return: String representation of the machine
 */

const char *get_machine_name(Elf64_Half e_machine)
{
	static char buff[64] = {'\0'};

	switch (e_machine)
	{
	case EM_NONE:       return "None";
	case EM_M32:        return "WE32100";
	case EM_SPARC:      return "Sparc";
	case EM_386:        return "Intel 80386";
	case EM_68K:        return "MC68000";
	case EM_88K:        return "MC88000";
	case EM_860:        return "Intel 80860";
	case EM_MIPS:       return "MIPS R3000";
	case EM_S370:       return "IBM System/370";
	case EM_MIPS_RS3_LE:return "MIPS R4000 big-endian";
	case EM_PARISC:     return "HPPA";
	case EM_VPP500:     return "Fujitsu VPP500";
	case EM_SPARC32PLUS:return "Sparc v8+";
	case EM_960:        return "Intel 80960";
	case EM_PPC:        return "PowerPC";
	case EM_PPC64:      return "PowerPC64";
	case EM_S390:       return "IBM S/390";
	case EM_V800:       return "Renesas V850 (using RH850 ABI)";
	case EM_FR20:       return "Fujitsu FR20";
	case EM_RH32:       return "TRW RH32";
	case EM_RCE:        return "Motorola RCE";
	case EM_ARM:        return "ARM";
	case EM_FAKE_ALPHA: return "Digital Alpha";
	case EM_SH:         return "Renesas / SuperH SH";
	case EM_SPARCV9:    return "Sparc v9";
	case EM_TRICORE:    return "Siemens Tricore";
	case EM_ARC:        return "ARC";
	case EM_H8_300:     return "Renesas H8/300";
	case EM_H8_300H:    return "Renesas H8/300H";
	case EM_H8S:        return "Renesas H8S";
	case EM_H8_500:     return "Renesas H8/500";
	case EM_IA_64:      return "Intel IA-64";
	case EM_MIPS_X:     return "Stanford MIPS-X";
	case EM_COLDFIRE:   return "Motorola Coldfire";
	case EM_68HC12:     return "Motorola MC68HC12 Microcontroller";
	case EM_MMA:        return "Fujitsu Multimedia Accelerator";
	case EM_PCP:        return "Siemens PCP";
	case EM_NCPU:       return "Sony nCPU embedded RISC processor";
	case EM_NDR1:       return "Denso NDR1 microprocesspr";
	case EM_STARCORE:   return "Motorola Star*Core processor";
	case EM_ME16:       return "Toyota ME16 processor";
	case EM_ST100:      return "STMicroelectronics ST100 processor";
	case EM_TINYJ:      return "Advanced Logic Corp. TinyJ embedded processor";
	case EM_X86_64:     return "Advanced Micro Devices X86-64";
	case EM_FX66:       return "Siemens FX66 microcontroller";
	case EM_ST7:        return "STMicroelectronics ST7 8-bit microcontroller";
	case EM_68HC16:     return "Motorola MC68HC16 Microcontroller";
	case EM_68HC11:     return "Motorola MC68HC11 Microcontroller";
	case EM_68HC08:     return "Motorola MC68HC08 Microcontroller";
	case EM_68HC05:     return "Motorola MC68HC05 Microcontroller";
	case EM_SVX:        return "Silicon Graphics SVx";
	case EM_ST19:       return "STMicroelectronics ST19 8-bit microcontroller";
	case EM_VAX:        return "Digital VAX";
	case EM_CRIS:       return "Axis Communications 32-bit embedded processor";
	case EM_JAVELIN:    return "Infineon Technologies 32-bit embedded cpu";
	case EM_FIREPATH:   return "Element 14 64-bit DSP processor";
	case EM_ZSP:        return "LSI Logic's 16-bit DSP processor";
	case EM_MMIX:       return "Donald Knuth's educational 64-bit processor";
	case EM_PRISM:      return "Vitesse Prism";
	case EM_AVR:        return "Atmel AVR 8-bit microcontroller";
	case EM_FR30:       return "Fujitsu FR30";
	case EM_D10V:       return "d10v";
	case EM_D30V:       return "d30v";
	case EM_V850:       return "Renesas V850";
	case EM_M32R:       return "Renesas M32R (formerly Mitsubishi M32r)";
	case EM_MN10300:    return "mn10300";
	case EM_MN10200:    return "mn10200";
	case EM_PJ:         return "picoJava";
	case EM_OPENRISC:   return "OpenRISC 32-bit embedded processor";
	case EM_ARC_A5:     return "ARC Cores Tangent-A5";
	case EM_XTENSA:     return "Tensilica Xtensa Processor";
	case EM_AARCH64:    return "AArch64";
	case EM_TILEPRO:    return "Tilera TILEPro multicore architecture family";
	case EM_MICROBLAZE: return "Xilinx MicroBlaze";
	case EM_TILEGX:     return "Tilera TILE-Gx multicore architecture family";

	default:
		snprintf(buff, sizeof(buff), "<unknown>: 0x%x", e_machine);
		return (buff);
	}
}
