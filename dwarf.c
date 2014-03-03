/*
 * dwarf.c 	Main file for dwarf library
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

#include "elf.h"
#include  "dwarf.h"
#include "dwarf-defines.h"
#include <stdlib.h>
#include <stdio.h>

unsigned char *section_debug_info, *section_debug_abbrev;
unsigned char *section_debug_str, *section_pubnames;
unsigned int info_off, abbrev_off;
int info_section_size, abbrev_section_size, pubnames_section_size;

char *struct_name_g;
unsigned int byte_size_g;
unsigned int type_value_g;
unsigned int type_tag_g;
unsigned int sibling_g;

/* 
	As an example, here is how the unsigned number 624485 gets encoded:
	  10011000011101100101  In raw binary
     010011000011101100101  Padded to a multiple of 7 bits
 0100110  0001110  1100101  Split into 7-bit groups
00100110 10001110 11100101  Add high 1 bits on all but last group to form bytes
    0x26     0x8E     0xE5  In hexadecimal
	0xE5 0x8E 0x26          Output stream */
int decode_ULEB128(unsigned int offset, int *used){
int result = 0;
int shift = 0;
unsigned char byte;

*used = 0;
	while(1)
	{
		byte = *(char *)(offset);//next byte in input;	
		result |= (byte & 0x7F) << shift; //result |= (low order 7 bits of byte << shift);
		*used +=1;	
		if((byte & 0x80) == 0)	//if (high order bit of byte == 0)
			break;
		shift += 7;
		offset++;		
	}
	return result;
}

long decode_leb128(unsigned int offset, int *used)
{
        unsigned char byte;
        int result, shift;
        int num_bits;
        int count;

        result = 0;
        shift = 0;
        count = 0;
		*used = 0;

        while (1) {
                byte = *(char *)(offset);
                offset +=1;
                result |= (byte & 0x7f) << shift;
                *used += 1;
                shift += 7;
                count++;

                if (!(byte & 0x80))
                        break;
        }

        /* The number of bits in a signed integer. */
        num_bits = 8 * sizeof(result);

        if ((shift < num_bits) && (byte & 0x40))		
                result |= (-1 << shift);

        return result;

}
cuabbr_t *cuabbr_head=NULL;

/*
abrev_off is the offset to the abbrev DIE in abbrev section.
This function takes abbrev_off 
and retruns two value,
one is return value ie DIE_END or 0
and used_offset which is used offset by this DIE
cuabbr_t the address of the struct

*/
#define DIE_END 2
cuabbr_t * read_one_DIE_debug_abbrev(int *used_offset, int *die_end){

	cuabbr_t *cuabbr = malloc(sizeof(cuabbr_t));
	int used=0;
	abbr_param_t *temp = NULL;
	
	if(cuabbr == NULL){
		printf(">>>>>>>>>>>error in malloc\n");
		exit(0);
	}	
	memset(cuabbr, 0, sizeof(cuabbr_t));
	cuabbr->index = decode_ULEB128(abbrev_off, &used);
	if(cuabbr->index == 0){
		if(debug_g)printf("End of abbrev die in this CU\n");
		free(cuabbr);
		*die_end =  DIE_END;
		return 0;
	}
	abbrev_off += used;
	
	cuabbr->tag = decode_ULEB128(abbrev_off, &used);
	abbrev_off += used;
	cuabbr->child = decode_ULEB128(abbrev_off, &used);
	abbrev_off += used;
	if(debug_g)printf("tag = %x child = %x",cuabbr->tag, cuabbr->child );
	while(1){
		abbr_param_t *param = malloc(sizeof(abbr_param_t));
		
		if(param == NULL){
			printf(">>>>>>>>>>>>>>>>>>>>>>>>> malloc failed\n");
			exit(0);
		}
			
		param->next = NULL;
		param->at = decode_ULEB128(abbrev_off, &used);
		abbrev_off += used;
	
		param->form = decode_ULEB128(abbrev_off, &used);
		abbrev_off += used;
		
		if(!param->at && !param->form){
			if(debug_g)
				printf("End of a DIE both at and form are zeros\n");
			free(param);
			break;		
		}
		
		if( temp == NULL){
			cuabbr->params = param;
			temp = param;
		} else { /* find the last param and add to the end*/
			temp->next = param;
			temp = param;			
		}
	}
	
	cuabbr->next = NULL;
	*used_offset = used;
	*die_end = 0;
	return cuabbr;	
}
void print_one_die_abbrev(cuabbr_t *cuabbr)
{
	abbr_param_t *abbr_param;
	printf("Index = %d\n", cuabbr->index);
	printf("TAG = 0x%x\n", cuabbr->tag);
	printf("child = %d\n", cuabbr->child);
	abbr_param = cuabbr->params;
	for(; abbr_param != NULL; abbr_param = abbr_param->next)
		printf("attr = 0x%x form = 0x%x\n", abbr_param->at, abbr_param->form);
}
void free_CU_debug_abbrev (cunit_t *cu){
	cuabbr_t *abbrev_entry, *next_abbrev_entry;
	abbr_param_t *next_param, *param;
		
	/* choose one DIE or abbrevation entry */
	for(abbrev_entry = cu->ab; abbrev_entry != NULL; ){
		if(debug_g)
			printf("free index =%d\n", abbrev_entry->index);
		for(param = abbrev_entry->params; param != NULL ; ){
			next_param = param->next;
			free(param);
			param = next_param;
		}
		next_abbrev_entry = abbrev_entry->next;
		free(abbrev_entry);
		abbrev_entry = next_abbrev_entry;
	}
	free(cu);
}

cunit_t * read_CU_debug_abbrev(cuhdr_t *cuhdr)
{
int used = 0, ret=0,die_end=0;
cunit_t *cu;
cuabbr_t *cuabbr;

	cu = malloc(sizeof(cunit_t));
	if(NULL == cu){
		printf("malloc failed LINE %d\n",__LINE__);
		exit(0);
	}
	memset(cu , 0 ,sizeof(cunit_t));
	if(debug_g)
		printf("CU len: %d dw_version = %d abbrev_off = 0x%x ptr size = %d\n ", cuhdr->len, cuhdr -> dw_ver, cuhdr ->abb_off, cuhdr->ptr_size);
	abbrev_off = section_debug_abbrev + cuhdr ->abb_off;
	
	cu->abbrev = section_debug_abbrev + cuhdr ->abb_off;
	while(die_end != DIE_END){
		cuabbr = read_one_DIE_debug_abbrev( &used,  &die_end);
		if(die_end == DIE_END){
			if(debug_g)
				printf("CU end LINE no:%d\n", __LINE__);
			break;
		}
		if(cuabbr < 0){
			printf("libdwarf: read retruned error\n");
			exit(0);
		}
		
		if(!cu->ab) /* Save the first DIE in CU */
		{	cu->ab = cuabbr;
			cu->ab_end = cuabbr;
		}else{
			cu->ab_end->next = cuabbr;
			cu->ab_end = cuabbr;
		}
			
		ret = abbrev_off - ((unsigned int)section_debug_abbrev + cuhdr->abb_off);
		if(cuhdr->len < ret){
			if(debug_g)
				printf("end LINE %d\n",__LINE__);
			break;
		}			
	}
	/*print all DIE in a CU */
	if(debug_g)
		for(cuabbr = cu->ab; cuabbr !=NULL; cuabbr = cuabbr->next)
			print_one_die_abbrev(cuabbr);
	return cu;
}

cuabbr_t * find_abbrev_index(int index, cunit_t *cu){

	cuabbr_t *abb_entry;
	if(index == 0){
		if(debug_g)printf("index 0 not permited LINE = %d\n", __LINE__);
		return -1;
	}else if(debug_g)
		printf("index = %d\n", index);	
	
	for(abb_entry = cu->ab; abb_entry!= NULL ; abb_entry = abb_entry->next){
		if(abb_entry->index == index)
			break;
	}
	if(abb_entry == NULL ){
		printf("cannot find abb_entry Line:%d\n",__LINE__);
		return NULL;
	} else
		return abb_entry;
}
/*This function returns the offset of the member into the structure
input arguments 
struct_name - name of the parent structure
member_name - member's name
returns the offset of the member into the structure
*/
int child_depth;
int print_one_cu(cunit_t *cu)
{

	cuabbr_t *abb_entry;
	int index, used, offset = 0;
	abbr_param_t *param;
	int info_off_start = info_off - 7;
	int dummy;

	while(cu->hdr.len > offset){
			index = decode_ULEB128(info_off , &used);
			if(debug_g)
				printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
			info_off +=used;
			
			while(index == 0 && child_depth != 0){
				if(debug_g)
					printf("End of children\n");
				child_depth--;
				offset = info_off - info_off_start;
				if(debug_g)
					printf("hdr.len = %d offset = %d\n",cu->hdr.len, offset );
				if(cu->hdr.len <= offset){
					if(debug_g)
						printf("info_off end for this CU Line = %d\n", __LINE__);
//					break;
					goto out;
				}
				index = decode_ULEB128(info_off , &used);
				if(debug_g)
					printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
				info_off +=used;				
			}
			
			abb_entry = find_abbrev_index(index, cu);
			if(abb_entry == NULL){
				if(debug_g)
					printf("index = %d\n", index);
				if(cu->hdr.len > offset){
					printf("this should not happen LINE %d\n",__LINE__);
					printf("hdr.len = %d offset = %d\n",cu->hdr.len, offset );
					exit(0);
				}
				break;
			}else if(abb_entry == -1)
				return -1;
			
			print_dwarftag(abb_entry->tag);
			if(abb_entry->child)
				child_depth++;
			if(debug_g)
				printf("children = %d child_depth = %d\n", abb_entry->child, child_depth);			
			
			for(param = abb_entry->params ; param != NULL ; param = param->next){
				print_abbrev_attr(param->at);
				used = print_abbrev_form(param->form, param->at, info_off, &dummy);
				info_off +=used;
			}
		offset = info_off - info_off_start;
	}
	
	if(abb_entry == NULL)
		return -1;
out:
	return 0;	
}
void print_all_info()
{
	unsigned int cu_info_start=0;
	cuhdr_t cuhdr;
	cunit_t *cu;
	int ret;
	
	info_off = 0;
	while(info_off < section_debug_info + info_section_size){
		memcpy(&cuhdr, section_debug_info + cu_info_start, sizeof(cuhdr_t));
		cu = read_CU_debug_abbrev(&cuhdr);
		if(cu == NULL){
			printf("All cu's end\n");
			exit(0);
		}
		info_off = section_debug_info + cu_info_start + sizeof(cuhdr);
		cu->hdr = cuhdr;
		ret = print_one_cu(cu);
		if(debug_g)printf("offset into the struct = 0x%x %d\n", ret, ret);
		free_CU_debug_abbrev(cu);

		if(debug_g)
			printf("info_off = 0x%x\n", info_off - (unsigned int)section_debug_info);
		cu_info_start +=  cuhdr.len + 4;
		if(debug_g)
			printf("CU=END============================================================================================================================================\n");
	}
}

void save_abb_n_info_as_bin()
{
	FILE *fp;
	#if 0
	fp = fopen("abbrev.out", "wb+");
	if(fp == NULL){
		printf("error in fopen\n");
		return -1;
	}
	
	fwrite(section_debug_abbrev, 1, abbrev_section_size , fp );
	fclose(fp);
	
	fp = fopen("info.out", "wb+");
	fwrite(section_debug_info, 1, info_section_size, fp );
	fclose(fp);
	#endif
	
	fp = fopen("pubnames.out", "wb+");
	fwrite(section_pubnames, 1, pubnames_section_size, fp);
	fclose(fp);
}

void read_n_copy_dwarf_sections(){
	int ret;
	unsigned int ULEB128;
	debug_info_hdr debug_info_hdr;
	debug_pubnames_hdr pubnames_hdr;
	int used,i;
	
	if(debug_g)
		printf(">>>>>>>>>>%d\n", sizeof(debug_info_hdr));
		
	copy_section(section_header_array, ".debug_info", elf_header.e_shnum, SHT_PROGBITS, &section_debug_info, &info_section_size);
	memcpy(&debug_info_hdr, section_debug_info, sizeof(debug_info_hdr));
	copy_section(section_header_array, ".debug_abbrev", elf_header.e_shnum, SHT_PROGBITS, &section_debug_abbrev, &abbrev_section_size);
	copy_section(section_header_array, ".debug_str", elf_header.e_shnum, SHT_PROGBITS, &section_debug_str, NULL);
	copy_section(section_header_array, ".debug_pubnames", elf_header.e_shnum, SHT_PROGBITS, &section_pubnames, &pubnames_section_size);
	
	#if 0
	/* This was done to scurtinize the debu_info and debug_abbrev sections
	   using hexedit. */
	save_abb_n_info_as_bin();
	#endif

	if(debug_g){
		printf("DWARF ver: %d\n", debug_info_hdr.DWARF_version);
		printf("CU len: %d abbrev_off = %x pointer_size = %d\n", debug_info_hdr.CU_len, debug_info_hdr.abbrev_off, debug_info_hdr.pointer_size);
		printf("section_debug_info = %x %d\n",section_debug_info,section_debug_info);
		printf("info_off = 0x%x\n", info_off - (unsigned int)section_debug_info);
	}
	//debug_g = 1;	
	if(debug_g)
		print_all_info();
	//debug_g = 0;
		
	return 0;

}

unsigned int cu_start_offset_g, cu_len;
/* This function returns the offset in debu_info section 
  Uses pubnames section */
unsigned int get_global_variable_die_offset(char *var)
{
	debug_pubnames_hdr pubnames_hdr;
	int size = 0, total_size = 0;
	unsigned int off_val;
	
	while(total_size < pubnames_section_size){
	
		memcpy(&pubnames_hdr, section_pubnames + total_size, sizeof(pubnames_hdr));
		
		if(debug_g){
			printf("len = %d 0x%x\n", pubnames_hdr.set_len, pubnames_hdr.set_len);
			printf("version = %d\n", pubnames_hdr.lookup_version);
			printf("CU offset = %d size = 0x%x\n", pubnames_hdr.debug_info_off, pubnames_hdr.debug_info_len);
		}
		cu_start_offset_g = pubnames_hdr.debug_info_off;
		if(debug_g)printf("CU offset = %d size = 0x%x\n", pubnames_hdr.debug_info_off, pubnames_hdr.debug_info_len);
		cu_len = pubnames_hdr.debug_info_len;
		
		for(size = sizeof(pubnames_hdr); size < pubnames_hdr.set_len; ){
			/* off_val is the offset into the debu_info for this CU */
			off_val = *(unsigned int *)(section_pubnames + total_size + size);
			size += 4;
			if(debug_g)
				printf("off_val = %d %s cu_start_offset_g = %d\n", off_val + cu_start_offset_g, section_pubnames + total_size + size, cu_start_offset_g );
			if(!strcmp(var, section_pubnames + total_size + size)){
				if(debug_g)
					printf("found a match in pubnames sections\n");
				return (off_val + cu_start_offset_g);
			}
			size += strlen(section_pubnames + total_size + size) + 1;
			if(debug_g)
				printf("size = %d pubnames_hdr.set_len = %d\n", size, pubnames_hdr.set_len);
		}
					
		size += 4;/*Every set is terminated with a unsigned int 0 */
		total_size += size;
		size = 0;
	}	
	
	printf("cannot fine variable in pubnames LINE no %d\n",__LINE__);
	return -1;
	
}

int get_tag_value(cunit_t *cu)
{
	cuabbr_t *abb_entry;
	int index, used, offset = 0;
	abbr_param_t *param;

	if(cu == NULL){
		printf("cu cannot be NULL LINE %d\n", __LINE__);
		exit(0);
	}

	index = decode_ULEB128(info_off , &used);
	if(debug_g)
		printf("index = %d info_off = 0x%x \n", index, info_off-(unsigned int)section_debug_info);
	info_off +=used;

	abb_entry = find_abbrev_index(index, cu);
	if(abb_entry == NULL){
		printf("abb entry is NULL index = %d\n", index);
	}else if(abb_entry == -1)
			return -1;
				
	return abb_entry->tag;

}

int get_attr_value(int attr, cunit_t *cu, void **value)
{
	cuabbr_t *abb_entry;
	int index, used, offset = 0;
	abbr_param_t *param;
	void *ptr;


	if(debug_g)printf("index = %d info_off = 0x%x \n", index, info_off-(unsigned int)section_debug_info);
	index = decode_ULEB128(info_off , &used);
	if(debug_g)
		printf("index = %d info_off = 0x%x \n", index, info_off-(unsigned int)section_debug_info);
	info_off +=used;	

	abb_entry = find_abbrev_index(index, cu);
	if(abb_entry == NULL){
		if(debug_g)
			printf("index = %d\n", index);
		if(cu->hdr.len > offset){
			printf("this should not happen LINE %d\n",__LINE__);
			printf("hdr.len = %d offset = %d\n",cu->hdr.len, offset );
			exit(0);
		}	
	}else if(abb_entry == -1)
			return -1;
				
	if(debug_g)
		printf("children = %d\n", abb_entry->child);
			
	for(param = abb_entry->params ; param != NULL ; param = param->next){
		/* ptr can be string, U32, U16 or any time accourding to its attr */
		used = print_abbrev_form(param->form, param->at, info_off, &ptr);
		info_off +=used;
		
		if(param->at == attr){
			*value = ptr;
			if(debug_g)printf("ptr = %x\n", ptr);
			return 0;
		}
	}

	return -1;

}
cunit_t *cu_g;/*this is used by 
find_struct_type_die_offset and 
find_member_offset funcitons */
unsigned int member_die_off;
unsigned int find_struct_type_die_offset(unsigned int die_off, unsigned int *what_type)
{	
	unsigned int cu_info_start=0;
	cuhdr_t cuhdr;	
	unsigned int type_offset;
	int ret;
	int tag, type_value;

	memcpy(&cuhdr, section_debug_info + cu_start_offset_g, sizeof(cuhdr_t));
	/* This creates linked list of all the abbrev entries for this CU */
	cu_g = read_CU_debug_abbrev(&cuhdr);
	if(cu_g == NULL){
		printf("CU doesn't exist LINE %d\n",__LINE__);
		return -1;
	}

	cu_g->hdr = cuhdr;
	info_off = section_debug_info + die_off;
	tag = get_tag_value(cu_g);
	*what_type = tag;
	
	if(tag == DW_TAG_structure_type)
		return die_off;
	
	info_off = section_debug_info + die_off;
	if(get_attr_value(DW_AT_type, cu_g, &type_value) != 0){
		printf("cannot fine attr Line %d\n", __LINE__);
		printf("More info tag = 0x%x\n",tag);
		exit(0);
	}
	return (type_value + cu_start_offset_g);
}

/* struct_type_die_offset is offset in debu_info to the parent structure */
unsigned int find_data_member_die_offset(unsigned int struct_type_die_offset,char * member_name, unsigned int *what_type)
{
	cuabbr_t *abb_entry;
	int index, used, offset = 0;
	abbr_param_t *param;
	int info_off_start;
	int child_depth = 0, name_match = 0;
	void *ptr;
	int sibling;
	int die_offset;
	
	info_off_start = info_off = section_debug_info + struct_type_die_offset;
	if(get_attr_value(DW_AT_sibling, cu_g, &sibling) != 0 ){
		printf("cannot find sibling LINE %d\n", __LINE__);
		exit(0);
        }

	if(debug_g)printf("sibling = %x\n", sibling);

	while(sibling > offset){
	
			if(name_match)
				return die_offset;
				
			die_offset = info_off - (unsigned int)section_debug_info;
			
			index = decode_ULEB128(info_off , &used);
			if(debug_g)
				printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
			info_off +=used;
			
			while(index == 0 && child_depth != 0){
				if(debug_g)
					printf("End of children\n");
				child_depth--;
				offset = info_off - info_off_start;
				if(debug_g)
					printf("hdr.len = %d offset = %d\n",cu_g->hdr.len, offset );
				if(sibling <= offset){
					if(debug_g)
						printf("info_off end for this CU Line = %d\n", __LINE__);
//					break;
					goto out;
				}
				index = decode_ULEB128(info_off , &used);
				if(debug_g)
					printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
				info_off +=used;				
			}
			
			abb_entry = find_abbrev_index(index, cu_g);
			if(abb_entry == NULL){
				if(debug_g)
					printf("index = %d\n", index);
				if(cu_g->hdr.len > offset){
					printf("this should not happen LINE %d\n",__LINE__);
					printf("hdr.len = %d offset = %d\n",cu_g->hdr.len, offset );
					exit(0);
				}
				break;
			}else if(abb_entry == -1)
				return -1;
				
			if(abb_entry->child)
				child_depth++;
			if(debug_g)
				printf("children = %d child_depth = %d\n", abb_entry->child, child_depth);			
			
			for(param = abb_entry->params ; param != NULL ; param = param->next){
				
				/* ptr can be string, U32, U16 or any time accourding to its attr */
				used = print_abbrev_form(param->form, param->at, info_off, &ptr);
				info_off +=used;
				
				if(param->at == DW_AT_name){
					if(!strcmp(ptr, member_name)){
						name_match = 1;
						if(debug_g)printf("name match %s %s", ptr, member_name);
					}else{
						name_match = 0;
						if(debug_g) printf("unset_name_match\n");
					}
				}
				if(param->at == DW_AT_type)
					*what_type = ptr + cu_start_offset_g;
			}
		offset = info_off - info_off_start;
		if(debug_g)
			printf("sibling = %d offset = %d\n", sibling, offset);
	}
out:
	return 0;
}
/* struct_type_die_offset is offset in debu_info to the parent structure */
unsigned int find_data_member_location(unsigned int struct_type_die_offset,char * member_name, unsigned int *what_type)
{
	cuabbr_t *abb_entry;
	int index, used, offset = 0;
	abbr_param_t *param;
	int info_off_start;
	int child_depth = 0, name_match = 0;
	void *ptr;
	int sibling, ret, die_off, type, declaration;
	char *name;
	
	info_off_start = info_off = section_debug_info + struct_type_die_offset;
	if(get_attr_value(DW_AT_sibling, cu_g, &sibling) != 0 ){

		info_off = info_off_start;
		ret = get_attr_value(DW_AT_name, cu_g, &name);
		info_off = info_off_start;
		ret = get_attr_value(DW_AT_declaration, cu_g, &declaration);
		free_CU_debug_abbrev(cu_g);
		die_off = find_die_offset_from_name(name);

		info_off_start = info_off = die_off + section_debug_info;

		if(get_attr_value(DW_AT_sibling, cu_g, &sibling) != 0 ){
			printf("cannot find sibling LINE %d\n", __LINE__);
			exit(0);
		}

    }

	if(debug_g)printf("sibling = %x\n", sibling);

	while(sibling > offset){
			index = decode_ULEB128(info_off , &used);
			if(debug_g)
				printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
			info_off +=used;
			
			while(index == 0 && child_depth != 0){
				if(debug_g)
					printf("End of children\n");
				child_depth--;
				offset = info_off - info_off_start;
				if(debug_g)
					printf("hdr.len = %d offset = %d\n",cu_g->hdr.len, offset );
				if(sibling <= offset){
					if(debug_g)
						printf("info_off end for this CU Line = %d\n", __LINE__);
//					break;		
					goto out;
				}
				index = decode_ULEB128(info_off , &used);
				if(debug_g)
					printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
				info_off +=used;				
			}
			
			abb_entry = find_abbrev_index(index, cu_g);
			if(abb_entry == NULL){
				if(debug_g)
					printf("index = %d\n", index);
				if(cu_g->hdr.len > offset){
					printf("this should not happen LINE %d\n",__LINE__);
					printf("hdr.len = %d offset = %d\n",cu_g->hdr.len, offset );
					exit(0);
				}
				break;
			}else if(abb_entry == -1)
				return -1;
				
			if(abb_entry->child)
				child_depth++;
			if(debug_g)
				printf("children = %d child_depth = %d\n", abb_entry->child, child_depth);			
			
			for(param = abb_entry->params ; param != NULL ; param = param->next){
				
				/* ptr can be string, U32, U16 or any time accourding to its attr */
				used = print_abbrev_form(param->form, param->at, info_off, &ptr);
				info_off +=used;
				
				if(param->at == DW_AT_name){
					if(!strcmp(ptr, member_name)){
						name_match = 1;
						if(debug_g)printf("name match %s %s", ptr, member_name);
					}else
						name_match = 0;
				}
				if(param->at == DW_AT_type)
					*what_type = ptr + cu_start_offset_g;

				if(name_match){
					if(param->at == DW_AT_data_member_location){
						return ptr;
					}
				}
			}
		offset = info_off - info_off_start;
		if(debug_g)
			printf("sibling = %d offset = %d\n", sibling, offset);
	}
out:
	return 0;
}

/* Is the tag of this die is a array_type or not
   if it is a pointer it returns the die info offset of pointer type_offset
   else 0*/
int is_die_a_typedef_type(unsigned int what_type)
{
	int tag_type, type_value;

	info_off = section_debug_info + what_type;
	tag_type = get_tag_value(cu_g);
	if(tag_type == DW_TAG_typedef){
		if(debug_g)printf("tag say its typedef LINE %d\n", __LINE__);
		info_off = section_debug_info + what_type;
		if(get_attr_value(DW_AT_type, cu_g, &type_value) != 0){
			printf("cannot find this Attr in this DIE\n");
			exit(0);
		}
		return type_value + cu_start_offset_g;
	}else
		return 0;
}

/* Is the tag of this die is a array_type or not
   if it is a pointer it returns the die info offset of pointer type_offset
   else 0*/
int is_die_a_array_type(unsigned int what_type)
{
	int tag_type, type_value;

	info_off = section_debug_info + what_type;
	tag_type = get_tag_value(cu_g);
	if(tag_type == DW_TAG_array_type){
		if(debug_g)printf("tag say its array type LINE %d\n", __LINE__);
		info_off = section_debug_info + what_type;
		if(get_attr_value(DW_AT_type, cu_g, &type_value) != 0){
			printf("cannot find this Attr in this DIE\n");
			exit(0);
		}
		return type_value + cu_start_offset_g;
	}else
		return 0;
}
/* Is the tag of this die is a ptr_type or not
   if it is a pointer it returns the die info offset of pointer type_offset
   else 0*/
int is_die_a_ptr_type(unsigned int what_type)
{
	int tag_type, type_value;

	info_off = section_debug_info + what_type/* + cu_start_offset_g*/;
	tag_type = get_tag_value(cu_g);
	if(tag_type == DW_TAG_pointer_type){
		if(debug_g)printf("tag say its pointer type LINE %d\n", __LINE__);
		info_off = section_debug_info + what_type;
		if(get_attr_value(DW_AT_type, cu_g, &type_value) != 0){
			printf("cannot find this Attr in this DIE\n");
			exit(0);
		}
		return type_value + cu_start_offset_g;
	}else
		return 0;
}

int search_name_in_cu(cunit_t *cu, char *name)
{
	cuabbr_t *abb_entry;
	int index, used, offset = 0;
	abbr_param_t *param;
	int info_off_start = info_off - 7;
	int die_off, ret, name_match = 0;
	void *ptr;

	while(cu->hdr.len > offset){
			index = decode_ULEB128(info_off , &used);
			if(debug_g)
				printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
			die_off = info_off;
			info_off +=used;
			
			while(index == 0 && child_depth != 0){
				if(debug_g)
					printf("End of children\n");
				child_depth--;
				offset = info_off - info_off_start;
				if(debug_g)
					printf("hdr.len = %d offset = %d\n",cu->hdr.len, offset );
				if(cu->hdr.len <= offset){
					if(debug_g)
						printf("info_off end for this CU Line = %d\n", __LINE__);
					goto out;
				}
				index = decode_ULEB128(info_off , &used);
				if(debug_g)
					printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
				info_off +=used;				
				die_off += used;
			}
			
			abb_entry = find_abbrev_index(index, cu);
			if(abb_entry == NULL){
				if(debug_g)
					printf("index = %d\n", index);
				if(cu->hdr.len > offset){
					printf("this should not happen LINE %d\n",__LINE__);
					printf("hdr.len = %d offset = %d\n",cu->hdr.len, offset );
					exit(0);
				}
				break;
			}else if(abb_entry == -1)
				return -1;
			
			if(debug_g)print_dwarftag(abb_entry->tag);
			
			if(abb_entry->child)
				child_depth++;
			if(debug_g)
				printf("children = %d child_depth = %d\n", abb_entry->child, child_depth);
			
			for(param = abb_entry->params ; param != NULL ; param = param->next){
				used = print_abbrev_form(param->form, param->at, info_off, &ptr);
				info_off +=used;
				
				if(abb_entry->tag == DW_TAG_structure_type && param->at == DW_AT_name)
						if(!strcmp(ptr, name)){
							if(debug_g)printf("struct_name = %s name = %s\n", ptr, name);
							name_match = 1;/*but we need to continue if it is declaration*/
							ret = die_off - (unsigned int)section_debug_info;
						}
						if(name_match && param->at == DW_AT_declaration)
								name_match = 0;
			}
			if(name_match == 1)
				return ret;
		offset = info_off - info_off_start;
	}
	
	if(abb_entry == NULL)
		return -1;
out:		
	return 0;/*this may be problematic if the struct name is in cu 0 arun*/
}

int find_die_offset_from_name(char *struct_name)
{
	unsigned int cu_info_start=0;
	cuhdr_t cuhdr;
	int ret;
	
	info_off = 0;
	if(debug_g)printf("searching struc_name = %s\n", struct_name);
	
	/* parse throught all the dies and break when the name matches*/
	while(info_off < section_debug_info + info_section_size){
		
		memcpy(&cuhdr, section_debug_info + cu_info_start, sizeof(cuhdr_t));
		cu_g = read_CU_debug_abbrev(&cuhdr);
		if(cu_g == NULL){
			printf("All cu's end\n");
			exit(0);
		}
		info_off = section_debug_info + cu_info_start + sizeof(cuhdr);
		cu_g->hdr = cuhdr;
		if(debug_g)printf("info_off = 0x%x\n", info_off);
		ret = search_name_in_cu(cu_g, struct_name);
		if(debug_g)printf("offset into the struct = 0x%x %d LINE no: %d\n", ret, ret, __LINE__);
		if(ret > 0){
			/* search_name_in_cu returns 0 if not found 
			   and  offset if found */
			cu_start_offset_g = cu_info_start;
//			printf("Arun Cu offset = %d\n", cu_info_start);
			return ret;
		}else if(ret < 0){
			printf("error while search fn = %s\n", __func__);
			exit(0);
		}
		free_CU_debug_abbrev(cu_g);
		cu_g = NULL;
		
		if(debug_g)
			printf("info_off = 0x%x\n", info_off - (unsigned int)section_debug_info);
		
		cu_info_start +=  cuhdr.len + 4;
		if(debug_g)
			printf("CU=END============================================================================================================================================\n");
	}
	return -1;
}

int get_structure_size(int off)
{
	int byte_size;

	info_off = section_debug_info + off;
	if(get_attr_value(DW_AT_byte_size, cu_g, &byte_size ) != 0 ){
		printf("cannot find byte_size attr Line %d\n", __LINE__);
		exit(0);
	}
	return byte_size;
}

int get_member_size(int struct_type_die_offset,char *member_name)
{
	cuabbr_t *abb_entry;
	int index, used, offset = 0;
	abbr_param_t *param;
	int info_off_start = info_off - 7;
	int child_depth = 0, name_match = 0;
	void *ptr;
	int sibling;
	int struct_size = get_structure_size(struct_type_die_offset);
	int once_matched = 0, data_mem_loc, data_mem_loc_next;

	if(debug_g) printf("struct_size = %d\n", struct_size);
	
	info_off_start = info_off = section_debug_info + struct_type_die_offset;

	if(get_attr_value(DW_AT_sibling, cu_g, &sibling) != 0 ){
		printf("cannot find sibling LINE %d\n", __LINE__);
		exit(0);
    	}
	offset = struct_type_die_offset;	
	sibling += cu_start_offset_g;

	if(debug_g)printf("sibling = %x\n", sibling);

	info_off = info_off_start;
	while(sibling > offset){
			if(debug_g)printf("Start sibling = %x offset = %x\n", sibling, offset);
		
			index = decode_ULEB128(info_off , &used);
			if(debug_g)
				printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
			info_off +=used;
			
			while(index == 0 && child_depth != 0){
				if(debug_g)
					printf("End of children\n");
				child_depth--;
				offset = info_off - info_off_start + struct_type_die_offset;
				if(debug_g)
					printf("hdr.len = %d offset = 0x%x\n",cu_g->hdr.len, offset );
				if(sibling <= offset){
					if(debug_g)
						printf("info_off end for this CU Line = %d\n", __LINE__);
						goto out;
				}
				index = decode_ULEB128(info_off , &used);
				if(debug_g)
					printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
				info_off +=used;				
			}
			
			abb_entry = find_abbrev_index(index, cu_g);
			if(abb_entry == NULL){
				if(debug_g)
					printf("index = %d\n", index);
				if(cu_g->hdr.len > offset){
					printf("this should not happen LINE %d\n",__LINE__);
					printf("hdr.len = %d offset = %d\n",cu_g->hdr.len, offset );
					exit(0);
				}
				break;
			}else if(abb_entry == -1)
				return -1;
				
			if(abb_entry->child)
				child_depth++;
			if(debug_g)
				printf("children = %d child_depth = %d\n", abb_entry->child, child_depth);			
			
			for(param = abb_entry->params ; param != NULL ; param = param->next){
				
				/* ptr can be string, U32, U16 or any time accourding to its attr */
				used = print_abbrev_form(param->form, param->at, info_off, &ptr);
				info_off +=used;
				
				if(param->at == DW_AT_name){
				if(debug_g)
				printf("#########member_name = %s ptr = %s\n", member_name, ptr);
				
					if(!strcmp(ptr, member_name)){
						name_match = 1;
						once_matched = 1;
						if(debug_g)printf("name match %s %s", ptr, member_name);
					}else 
						name_match = 0;

				}

				if(param->at == DW_AT_data_member_location){
					if(name_match){
						data_mem_loc =  ptr;
						if(debug_g)	printf("data_mem_loc = %d\n", data_mem_loc);
					}else
						data_mem_loc_next = ptr;
				}
			}
			if(once_matched & (!name_match)){
				return data_mem_loc_next - data_mem_loc;
			}
		offset = info_off - info_off_start + struct_type_die_offset;
		if(debug_g)
			printf("sibling = %x offset = %x\n", sibling, offset);

	}
out:	
	return struct_size - data_mem_loc;
}

int find_upper_bound(int die_off)
{
	cuabbr_t *abb_entry;
	int index, used, offset = 0;
	abbr_param_t *param;
	int info_off_start = info_off - 7;
	int child_depth = 0, name_match = 0;
	void *ptr;
	int sibling;
	
	info_off_start = info_off = section_debug_info + die_off;
	if(get_attr_value(DW_AT_sibling, cu_g, &sibling) != 0 ){
		printf("cannot find sibling LINE %d\n", __LINE__);
		exit(0);
        }

	if(debug_g)printf("sibling = %x\n", sibling + cu_start_offset_g);
	while(sibling > offset){			
			index = decode_ULEB128(info_off , &used);
			if(debug_g)
				printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
			info_off +=used;
			
			while(index == 0 && child_depth != 0){
				if(debug_g)
					printf("End of children\n");
				child_depth--;
				offset = info_off - info_off_start;
				if(debug_g)
					printf("hdr.len = %d offset = %d\n",cu_g->hdr.len, offset );
				if(sibling <= offset){
					if(debug_g)
						printf("info_off end for this CU Line = %d\n", __LINE__);
//					break;		
					goto out;
				}
				index = decode_ULEB128(info_off , &used);
				if(debug_g)
					printf("index = %d info_off = 0x%x child_depth = %d\n", index, info_off-(unsigned int)section_debug_info, child_depth);
				info_off +=used;				
			}
			abb_entry = find_abbrev_index(index, cu_g);
			if(abb_entry == NULL){
				if(debug_g)
					printf("index = %d\n", index);
				if(cu_g->hdr.len > offset){
					printf("this should not happen LINE %d\n",__LINE__);
					printf("hdr.len = %d offset = %d\n",cu_g->hdr.len, offset );
					exit(0);
				}
				break;
			}else if(abb_entry == -1)
				return -1;
				
			if(abb_entry->child)
				child_depth++;
			if(debug_g)
				printf("children = %d child_depth = %d\n", abb_entry->child, child_depth);			
			
			for(param = abb_entry->params ; param != NULL ; param = param->next){
				/* ptr can be string, U32, U16 or any time accourding to its attr */
				used = print_abbrev_form(param->form, param->at, info_off, &ptr);
				info_off +=used;
				if(param->at == DW_AT_upper_bound)
					return ptr;				
			}
			
		offset = info_off - info_off_start;
		if(debug_g)
			printf("sibling = %d offset = %d\n", sibling, offset);
	}
out:
	return 0;
	
}

void free_dwarf_dynamic_memory(void)
{
	free(section_debug_info);section_debug_info=NULL;
	free(section_debug_abbrev);section_debug_abbrev=NULL;
	free(section_debug_str);section_debug_str=NULL;
	free(section_pubnames);section_pubnames=NULL;
	cu_start_offset_g = 0;
	if(cu_g != NULL){
		free_CU_debug_abbrev(cu_g);
		cu_g = NULL;
		}
}



