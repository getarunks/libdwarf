/*
 * dwarf_tag.c This file deals with dwarf tags.
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
#include "dwarf-defines.h"
#include "dwarf.h"
#include <stdlib.h>
extern int debug_g;
extern unsigned int type_value_g;
extern unsigned int sibling_g;
extern char *struct_name_g;
extern int byte_size_g;
extern unsigned int mem_offset_g;

void print_dwarftag(long tag)
{
 	 if(debug_g)printf("===================\n");
	 if(debug_g)printf("[ 0x%x ]", info_off - (unsigned int)section_debug_info - 1/*index*/);
	switch(tag){	
	case DW_TAG_compile_unit:
		if(debug_g)printf("TAG: compile unit\n");
		break;
	case DW_TAG_structure_type:
		if(debug_g)printf("TAG: structure_type\n");
		break;
	case DW_TAG_variant:
		if(debug_g)printf("TAG: variant\n");
		break;
	case DW_TAG_member:
		if(debug_g)printf("TAG: member\n");
		break;
	case DW_TAG_base_type:	
		if(debug_g)printf("TAG: base_type\n");
		break;
	case DW_TAG_subprogram:
		if(debug_g)printf("TAG: subprogram\n");
		break;
	case DW_TAG_variable:
		if(debug_g)printf("TAG: variable\n");
		break;
	case DW_TAG_typedef:
		if(debug_g)printf("TAG: typedef\n");
		break;
	case DW_TAG_array_type:
		if(debug_g)printf("TAG: array_type\n");
		break;
	case DW_TAG_subrange_type:
		if(debug_g)printf("TAG: subrange_type\n");
		break;
	case DW_TAG_pointer_type:
		if(debug_g)printf("TAG: pointer_type\n");
		break;
	case DW_TAG_const_type:
		if(debug_g)printf("TAG: const_type\n");
		break;
	case DW_TAG_subroutine_type:
		if(debug_g)printf("TAG: subroutine_type\n");
		break;
	case DW_TAG_formal_parameter:
		if(debug_g)printf("TAG: formal_parameter\n");
		break;
	case DW_TAG_union_type:
		if(debug_g)printf("TAG: union_type\n");
		break;
	case DW_TAG_enumeration_type:
		if(debug_g)printf("TAG enumeration_type\n");
		break;
	case DW_TAG_enumerator:
		if(debug_g)printf("TAG: enumerator\n");
		break;
	case DW_TAG_volatile_type:
		if(debug_g)printf("TAG: volatile_type\n");
		break;
	case DW_TAG_lexical_block:
		if(debug_g)printf("TAG: lexical_block\n");
		break;
	case DW_TAG_label:
		if(debug_g)printf("TAG: label\n");
		break;
	case DW_TAG_inlined_subroutine:
		if(debug_g)printf("TAG :inlined_subroutine\n");
		break;
	case DW_TAG_unspecified_parameters:
		if(debug_g)printf("unspecified_parameters\n");
		break;
	default:
		printf("TAG: Not defined 0x%x>>>>>>>", tag);
		exit (0);
		break;
	}
}
long curr_attr_g,curr_form_g;
void print_abbrev_attr( long attr)
{
	curr_attr_g = attr;

		switch(attr){
			case DW_AT_producer:
				if(debug_g)printf("producer :\t");
				break;
			case DW_AT_language:
				if(debug_g)printf("language :\t");
				break;
			case DW_AT_name:
				if(debug_g)printf("name :\t");
				break;
			case DW_AT_comp_dir:
				if(debug_g)printf("comp_dir :\t");
				break;
			case DW_AT_low_pc:
				if(debug_g)printf("low_pc :\t");
				break;
			case DW_AT_high_pc:
				if(debug_g)printf("high_pc :\t");
				break;
			case DW_AT_stmt_list:
				if(debug_g)printf("stm_list :\t");
				break;
			case DW_AT_byte_size:
				if(debug_g)printf("byte size:\t");
				break;
			case DW_AT_decl_file:
				if(debug_g)printf("decl file:\t");
				break;
			case DW_AT_decl_line:
				if(debug_g)printf("decl line:\t");
				break;			
			case DW_AT_sibling:
				if(debug_g)printf("sibling:\t\t");				
				break;		
			case DW_AT_type:
				if(debug_g)printf("type:\t\t");
				break;			
			case DW_AT_data_member_location:
				if(debug_g)printf("data_member_location:\t");
				break;				
			case DW_AT_encoding:
				if(debug_g)printf("encoding:\t");
				break;
			case DW_AT_external:
				if(debug_g)printf("External:\t");
				break;
			case DW_AT_prototyped:
				if(debug_g)printf("prototypted:\t");
				break;
			case DW_AT_frame_base:
				if(debug_g)printf("frame base:\t");
				break;
			case DW_AT_location:
				if(debug_g)printf("location:\t");
				break;
			case DW_AT_ranges:
				if(debug_g)printf("Ranges:\t");
				break;
			case DW_AT_upper_bound:
				if(debug_g)printf("upper_bount:\t");
				break;
			case DW_AT_declaration:
				if(debug_g)printf("declaration:\t");
				break;
			case DW_AT_bit_size:
				if(debug_g)printf("bit_size:\t");
				break;
			case DW_AT_bit_offset:
				if(debug_g)printf("bit_offset:\t");
				break;
			case DW_AT_const_value:
				if(debug_g)printf("const_value:\t");
				break;
			case DW_AT_entry_pc:
				if(debug_g)printf("entry_pc:\t");
				break;
			case DW_AT_inline:
				if(debug_g)printf("inline:\t");
				break;
			case DW_AT_abstract_origin:
				if(debug_g)printf("abstract_origin:\t");
				break;
			case DW_AT_call_file:
				if(debug_g)printf("call_file:\t");
				break;
			case DW_AT_call_line:
				if(debug_g)printf("call_line:\t");
				break;
			case DW_AT_artificial:
				if(debug_g)printf("artificial:\t");
				break;
			default:
				printf("not supported attr = 0x%x \n", attr);
				exit(0);;
		}
}

int decode_OP(form, attr)
{
	unsigned int loc, used, op;
	unsigned int len;// = decode_ULEB128(info_off , &used);

	switch(form){
	case DW_FORM_block1:
		len = *(unsigned char*) info_off;
		info_off += 1;
		break;
	case DW_FORM_block2:
		len = *(unsigned short *) info_off;
		info_off += 2;
		break;
	}

	if(debug_g)printf("op len is = %d\n", len);
	if(attr != DW_AT_data_member_location){/* we go further only if current att is data_member_location */
		info_off += len;
		return 0;/* Do we need to go further as of now? arun */
	}

	op = *(unsigned char *)info_off;
	info_off +=1;
	
	if(debug_g)printf("op = 0x%x\n", op);
	switch(op){
	case DW_OP_plus_uconst:
		loc = decode_ULEB128(info_off , &used);
		info_off +=used;
		break;
	case DW_OP_addr:/*constant address (size target specific) arun */
		loc = *(unsigned int *)info_off;
		info_off += 4;
		break;
	case DW_OP_fbreg:
		//printf("[ 0x%x ]", info_off - (unsigned int)section_debug_info);
		loc = decode_leb128(info_off , &used);
		info_off += used;
		//printf("[ 0x%x ]", info_off - (unsigned int)section_debug_info);
		//printf("used = %d\n", used);
		break;
		
	/* DW_OP_breg0, DW_OP_breg1, ..., DW_OP_breg31 
	The single operand of the DW_OP_bregn operations provides a 
	signed LEB128 offset from the specified register.*/
	case DW_OP_breg0:
	case DW_OP_breg1:
	case DW_OP_breg2:
	case DW_OP_breg3:
	case DW_OP_breg4:
	case DW_OP_breg5:
	case DW_OP_breg6:
	case DW_OP_breg7:
	case DW_OP_breg8:
	case DW_OP_breg9:
	case DW_OP_breg10:
	case DW_OP_breg11:
	case DW_OP_breg12:
	case DW_OP_breg13:
	case DW_OP_breg14:
	case DW_OP_breg15:
	case DW_OP_breg16:
	case DW_OP_breg17:
	case DW_OP_breg18:
	case DW_OP_breg19:
	case DW_OP_breg20:
	case DW_OP_breg21:
	case DW_OP_breg22:
	case DW_OP_breg23:
	case DW_OP_breg24:
	case DW_OP_breg25:
	case DW_OP_breg26:
	case DW_OP_breg27:
	case DW_OP_breg28:
	case DW_OP_breg29:
	case DW_OP_breg30:
	case DW_OP_breg31:	
		loc = decode_leb128(info_off , &used);
		info_off += used;
		break;

	case DW_OP_reg0:
	case DW_OP_reg1:
	case DW_OP_reg2:
	case DW_OP_reg3:
	case DW_OP_reg4:
	case DW_OP_reg5:
	case DW_OP_reg6:
	case DW_OP_reg7:
	case DW_OP_reg8:
	case DW_OP_reg9:
	case DW_OP_reg10:
	case DW_OP_reg11:
	case DW_OP_reg12:
	case DW_OP_reg13:
	case DW_OP_reg14:
	case DW_OP_reg15:
	case DW_OP_reg16:
	case DW_OP_reg17:
	case DW_OP_reg18:
	case DW_OP_reg19:
	case DW_OP_reg20:
	case DW_OP_reg21:
	case DW_OP_reg22:
	case DW_OP_reg23:
	case DW_OP_reg24:
	case DW_OP_reg25:
	case DW_OP_reg26:
	case DW_OP_reg27:
	case DW_OP_reg28:
	case DW_OP_reg29:
	case DW_OP_reg30:
	case DW_OP_reg31:
		break;/* ignore as of now since we are not interested */
	default:
		printf("unknown ops = 0x%x\n", op);
		exit(0);
		break;
	}
	if(debug_g)printf("loc = %d \n", loc);
//	mem_offset_g = loc;
	return loc;
}

int print_abbrev_form(long form, long attr, char * info_off, void **ptr)
{
 	 int used=0, ret, len;
		switch(form){
			case DW_FORM_strp:
				if(debug_g)printf("%s\n", section_debug_str + *(unsigned int *)info_off);
				*ptr = section_debug_str + *(unsigned int *)info_off;
				used = 4;
				break;
			case DW_FORM_data1:
				if(debug_g)printf("%d\n",  *(unsigned char *)info_off);
				used = 1;
//				if(attr == DW_AT_byte_size)
				*ptr = *(unsigned char *)info_off;				
				break;
			case DW_FORM_data2:
				if(debug_g)printf("%d\n", *(unsigned short *)info_off);
				used = 2;
//				if(attr == DW_AT_byte_size)
				*ptr = *(unsigned short *)info_off;
				break;
			case DW_FORM_data4:
				if(debug_g)printf("%d\n", *(unsigned int *)info_off);
				used = 4;
				break;
			case DW_FORM_data8:
				if(debug_g)printf("%llx\n", *(unsigned long long *)info_off);
				used = 8;
				break;
			case DW_FORM_string:
				if(debug_g)printf("%s\n", (unsigned char *)info_off);
				*ptr = info_off;
				used = strlen((unsigned char *)info_off) + 1;
				break;
			case DW_FORM_addr:
				if(debug_g)printf("0x%x\n", *(unsigned int *)info_off);
				used = 4;
				break;
			case DW_FORM_ref4:
				if(DW_AT_type == attr)
//					type_value_g = *(unsigned int *)info_off;
					*ptr = *(unsigned int *)info_off;
				else if(DW_AT_sibling == attr)
//					sibling_g = *(unsigned int *)info_off;
					*ptr = *(unsigned int *)info_off;
				if(debug_g)printf("ref4 0x%x\n", *(unsigned int *)info_off);
				used = 4;
				break;
			case DW_FORM_block1:			
				if(DW_AT_const_value == attr){
					//len = decode_ULEB128(info_off , &used);
					len = *(unsigned char *)info_off;
					used += 1;
					used += len;
				} else {
					ret = decode_OP(form, attr);// arun
					*ptr = ret;
					used = 0;
				}
				break;
			case DW_FORM_block2:
				if(DW_AT_const_value == attr){
					len = *(unsigned short *)info_off;
					used += 2;
					used += len;
					//printf("len = %d\n", len);
				} else {
					ret = decode_OP(form, attr);// arun
					*ptr = ret;
					used = 0;
				}
				break;
			case DW_FORM_flag:
				if(debug_g)printf("0x%x\n", *(unsigned char *)info_off);
				used +=1;
				info_off +=1;
				break;
			case DW_FORM_sdata:				
				ret = decode_ULEB128(info_off , &used);
				if(debug_g)printf("%d\n", ret);
				info_off +=used;
				break;
			default:
				printf("not supported form = 0x%x\n", form);
				exit(0);
		}		
		
		return used;
}

