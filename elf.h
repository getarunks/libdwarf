/*
 * elf.h Header file for ELF definitions.
 * Copyright (C) 2012  Arun KS<getarunks@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
typedef  unsigned int Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Off;
typedef int Elf32_Sword;
typedef unsigned int Elf32_Word;

#define EI_NIDENT	16
/* header structure */
typedef struct {
#define EI_index_MAG0 	0
#define	EI_index_MAG1	1
#define	EI_index_MAG2	2
#define	EI_index_MAG3	3
#define ELFMAG0	0x7f
#define	ELFMAG1	'E'
#define ELFMAG2	'L'
#define ELFMAG3	'F'
	unsigned char	e_ident[EI_NIDENT];
	Elf32_Half		e_type;
	Elf32_Half		e_machine;
	Elf32_Word		e_version;
	Elf32_Addr		e_entry;
	Elf32_Off		e_phoff;
	Elf32_Off		e_shoff;
	Elf32_Word		e_flags;
	Elf32_Half		e_ehsize;
	Elf32_Half		e_phentsize;
	Elf32_Half		e_phnum;
	Elf32_Half		e_shentsize;
	Elf32_Half		e_shnum;
	Elf32_Half		e_shstrndx;
} Elf32_Ehdr;
/* Section Header */
typedef struct {
	Elf32_Word		sh_name;
	Elf32_Word		sh_type;
	Elf32_Word		sh_flags;
	Elf32_Addr		sh_addr;
	Elf32_Off		sh_offset;
	Elf32_Word		sh_size;
	Elf32_Word		sh_link;
	Elf32_Word		sh_info;
	Elf32_Word		sh_addralign;
	Elf32_Word		sh_entsize;
} Elf32_Shdr;
/*Section types*/

enum section_type{
SHT_NULL,
SHT_PROGBITS,
SHT_SYMTAB,
SHT_STRTAB,
SHT_RELA,
SHT_HASH,
SHT_DYNAMIC,
SHT_NOTE,
SHT_NOBITS,
SHT_REL,
SHT_SHLIB,
SHT_DYNSYM,
SHT_LOPROC = 0x70000000,
SHT_HIPROC = 0x7fffffff,
SHT_LOUSER = 0x80000000,
SHT_HIUSER = 0xffffffff	,
};

typedef struct {
Elf32_Word		st_name;
Elf32_Addr		st_value;
Elf32_Word		st_size;
unsigned char	st_info;
unsigned char	st_other;
Elf32_Half		st_shndx;
}Elf32_Sym;

int copy_section(Elf32_Shdr *shHeaderArray, char *name, int num_sections, enum section_type type, char **dest, int *size);

extern unsigned char* vmlinux_file_path;
extern unsigned char	*section_shStrTable, *section_strTable;
extern unsigned char	*section_symTable;
extern Elf32_Ehdr    	elf_header;
extern Elf32_Shdr		section_header_array[50];
extern unsigned char *mapped_binary_base;
extern int debug_g;