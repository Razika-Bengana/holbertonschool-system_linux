#include "elf_header.h"

void print_section_headers(SectionHeaderInfo *info)
{
    int num_headers;
    Elf64_Off offset;
    char flag_str[5];
    int isLittleEndian;

    union {
        uint64_t sh_flags_64;
        uint32_t sh_flags_32;
    } sh_flags_converted;

    if (info->is_64bit)
    {
        Elf64_Ehdr *ehdr = (Elf64_Ehdr *)info->elf_header;
        isLittleEndian = (ehdr->e_ident[EI_DATA] == ELFDATA2LSB);

        num_headers = isLittleEndian ? ehdr->e_shnum : ntohs(ehdr->e_shnum);
        offset = isLittleEndian ? ehdr->e_shoff : ntohl(ehdr->e_shoff);

        printf("There are %d section headers, starting at offset 0x%lx:\n", num_headers, (unsigned long)offset);
        printf("\n");
        printf("Section Headers:\n");
        printf("  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");

        for (int i = 0; i < info->count; i++)
        {
            uint32_t sh_name_converted, sh_type_converted;
            uint64_t sh_addr_converted, sh_offset_converted, sh_size_converted, sh_entsize_converted, sh_addralign_converted;
            uint32_t sh_link_converted, sh_info_converted;

            Elf64_Shdr *section = &((Elf64_Shdr *)info->shdr)[i];

            if (!isLittleEndian)
            {
                sh_name_converted = swap_uint32(section->sh_name);
                sh_type_converted = swap_uint32(section->sh_type);

                sh_addr_converted = swap_uint64(section->sh_addr);
                sh_offset_converted = swap_uint64(section->sh_offset);
                sh_size_converted = swap_uint64(section->sh_size);
                sh_entsize_converted = swap_uint64(section->sh_entsize);

                sh_flags_converted.sh_flags_64 = swap_uint64(section->sh_flags);

                sh_link_converted = swap_uint32(section->sh_link);
                sh_info_converted = swap_uint32(section->sh_info);
                sh_addralign_converted = swap_uint64(section->sh_addralign);
            }
            else
            {
                sh_name_converted = section->sh_name;
                sh_type_converted = section->sh_type;

                sh_addr_converted = section->sh_addr;
                sh_offset_converted = section->sh_offset;
                sh_size_converted = section->sh_size;
                sh_entsize_converted = section->sh_entsize;
                sh_flags_converted.sh_flags_64 = section->sh_flags;
                sh_link_converted = section->sh_link;
                sh_info_converted = section->sh_info;
                sh_addralign_converted = section->sh_addralign;
            }

            print_flags(sh_flags_converted.sh_flags_64, flag_str);

            printf("  [%2d] %-17s %-15s %016lx %06lx %06lx %02lx %3s %2d %3u %2lu\n",
                   i,
                   info->strtab + sh_name_converted,
                   section_type_to_string(sh_type_converted),
                   (unsigned long)sh_addr_converted,
                   (unsigned long)sh_offset_converted,
                   (unsigned long)sh_size_converted,
                   (unsigned long)sh_entsize_converted,
                   flag_str,
                   (int)sh_link_converted,
                   (unsigned int)sh_info_converted,
                   (unsigned long)sh_addralign_converted);
        }

        printf("Key to Flags:\n");
        printf("  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)\n");
        printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
        printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
    }

    else
    {
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)info->elf_header;
        isLittleEndian = (ehdr->e_ident[EI_DATA] == ELFDATA2LSB);

        num_headers = isLittleEndian ? ehdr->e_shnum : ntohs(ehdr->e_shnum);
        offset = isLittleEndian ? ehdr->e_shoff : ntohl(ehdr->e_shoff);

        printf("There are %d section headers, starting at offset 0x%lx:\n", num_headers, offset);
        printf("\n");
        printf("Section Headers:\n");
        printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");

        for (int i = 0; i < info->count; i++)
        {
            uint32_t sh_name_converted, sh_type_converted;
            uint32_t sh_addr_converted, sh_offset_converted, sh_size_converted, sh_entsize_converted, sh_addralign_converted, sh_link_converted, sh_info_converted;

            Elf32_Shdr *section = &((Elf32_Shdr *)info->shdr)[i];

            if (!isLittleEndian)
            {
                sh_name_converted = ntohl(section->sh_name);
                sh_type_converted = ntohl(section->sh_type);

                sh_addr_converted = ntohl(section->sh_addr);
                sh_offset_converted = ntohl(section->sh_offset);
                sh_size_converted = ntohl(section->sh_size);
                sh_entsize_converted = ntohl(section->sh_entsize);
                sh_flags_converted.sh_flags_32 = ntohl(section->sh_flags);
                sh_link_converted = ntohl(section->sh_link);
                sh_info_converted = ntohl(section->sh_info);
                sh_addralign_converted = ntohl(section->sh_addralign);
            }
            else
            {
                sh_name_converted = section->sh_name;
                sh_type_converted = section->sh_type;

                sh_addr_converted = section->sh_addr;
                sh_offset_converted = section->sh_offset;
                sh_size_converted = section->sh_size;
                sh_entsize_converted = section->sh_entsize;
                sh_flags_converted.sh_flags_32 = section->sh_flags;
                sh_link_converted = section->sh_link;
                sh_info_converted = section->sh_info;
                sh_addralign_converted = section->sh_addralign;
            }

            print_flags(sh_flags_converted.sh_flags_32, flag_str);

            printf("  [%2d] %-17s %-15s %08lx %06lx %06lx %02lx %3s %2lu %3lu %2lu\n",
                   i,
                   info->strtab + sh_name_converted,
                   section_type_to_string(sh_type_converted),
                   (unsigned long) sh_addr_converted,
                   (unsigned long) sh_offset_converted,
                   (unsigned long) sh_size_converted,
                   (unsigned long) sh_entsize_converted,
                   flag_str,
                   (unsigned long) sh_link_converted,
                   (unsigned long) sh_info_converted,
                   (unsigned long) sh_addralign_converted);
        }

        printf("Key to Flags:\n");
        printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
        printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
        printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
    }
}
