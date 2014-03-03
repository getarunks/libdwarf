/*
 * dwarf.h Header file for dwarf library.
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
void read_n_copy_dwarf_sections();
/*
- CU length : 32bits
- DWARF version : 16bits
- abbrev offset : 32bits
- pointer size : 8bits*/
#pragma pack(1)
typedef struct {
	unsigned int CU_len;
	unsigned short DWARF_version;
	unsigned	int	abbrev_off;
	unsigned char	pointer_size;
}debug_info_hdr;
#pragma pack(0)

#pragma pack(1)
typedef struct {
	unsigned int set_len;
	unsigned short lookup_version;
	unsigned int	debug_info_off;
	unsigned int	debug_info_len;
}debug_pubnames_hdr;
#pragma pack(0)

typedef struct	val2text_s
{
  char		*text;
  long		val;
}		val2text_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

/* structure for compilation unit's header				*/
#pragma pack(1)
typedef struct	cuhdr_s
{
  uint32_t	len;			/* length of compilation unit	*/
  uint16_t	dw_ver;			/* dwarf version (2 for me)	*/
  uint32_t	abb_off;		/* offset in .debug_abbrev	*/
  uint8_t	ptr_size;		/* pointer size on target arch	*/
}cuhdr_t;
#pragma pack(0)

/* structure for stocking parameters of abbreviations entry		*/
typedef struct		abbr_param_s
{
  long			at;
  long			form;
  struct abbr_param_s	*next;
}abbr_param_t;

/* structure for abbreviation table entries for one compilation unit	*/
typedef struct		cuabbr_s
{
  long			index;
  long			tag;
  uint8_t		child;		/* flag : does this have child?	*/
  abbr_param_t		*params;	/* linked list of parameters	*/
  abbr_param_t		*last_param;
  struct cuabbr_s	*next;
}cuabbr_t;

/* structure for one compilation unit					*/
typedef struct	cuni_s
{
  int		depth;			/* current depth in CU		*/
  char		*filename;		/* source file of CU		*/
  char		*base;			/* base address of current CU	*/
  char		*sec_base;		/* current section base offset	*/
  cuhdr_t	hdr;			/* compilation unit header	*/
  char		*abbrev;		/* base abbrev address		*/
  cuabbr_t	*ab;			/* linked list of abbrev entries*/
  cuabbr_t	*ab_end;		/* last element of linked list	*/
 // type_t	*types;			/* types linked list		*/
 // stabs_t	*funcs;			/* functions linked list (CU)	*/
  struct cuni_s	*next;			/* next element of linked list	*/
} cunit_t;

extern void print_dwarftag(long tag);
extern void  print_abbrev_attr( long attr);
extern int print_abbrev_form(long form, long attr, char * info_off, void **ptr);
extern unsigned int info_off, abbrev_off;
extern unsigned char *section_debug_str;
extern unsigned char *section_debug_info;
extern long decode_leb128(unsigned int offset, int *used);
