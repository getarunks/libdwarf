/*
 * dwarf-main.c Entry point for dwarf library.
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
#include "dll.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "elf.h"
#include "dwarf.h"

/* Make it 1 for more debug prints */
int debug_g=0;

unsigned char* vmlinux_file_path;
unsigned char	*section_shStrTable, *section_strTable;
unsigned char	*section_symTable;
Elf32_Ehdr    	elf_header;
Elf32_Shdr		section_header_array[50];

int section_header_index(enum section_type type, char *name){
	int i;
	char section_name[50];
	
	for(i = 0 ; i < elf_header.e_shnum; i++){
		if((section_header_array[i].sh_type == type) &&
			strcmp(name, section_header_array[i].sh_name + section_shStrTable)== 0)
			return i;
	}
}

int verify_elf_header(Elf32_Ehdr *elf_header){
	
	int ret = 0;
	
    if((elf_header->e_ident[EI_index_MAG0] == ELFMAG0 )&& 
							   ( elf_header->e_ident[EI_index_MAG1] == ELFMAG1 )&& 
                              ( elf_header->e_ident[EI_index_MAG2] == ELFMAG2) && 
                              ( elf_header->e_ident[EI_index_MAG3] == ELFMAG3))
              ret = 1;
    else {
		      return 0;
	}
    if(debug_g)printf("e_type = %d\n", elf_header->e_type );
	if(debug_g)printf("e_machine = %d\n", elf_header->e_machine);
	if(debug_g)printf("e_version = %d\n", elf_header->e_version);
	if(debug_g)printf("e_entry = 0x%x\n", elf_header->e_entry);
	if(debug_g)printf("e_phoff = %d 0x%x\n", elf_header->e_phoff, elf_header->e_phoff);
	if(debug_g)printf("e_shoff = %d 0x%x\n", elf_header->e_shoff, elf_header->e_shoff);
	if(debug_g)printf("e_flags = 0x%x\n", elf_header->e_flags, elf_header->e_flags);
	if(debug_g)printf("e_ehsize = %d bytes\n", elf_header->e_ehsize);
	if(debug_g)printf("e_phentsize = %d bytes\n", elf_header->e_phentsize);
	if(debug_g)printf("e_phnum = %d entries\n", elf_header->e_phnum);
	if(debug_g)printf("e_shentsize = %d bytes\n", elf_header->e_shentsize);
	if(debug_g)printf("e_shnum = %d entries\n", elf_header->e_shnum);
	if(debug_g)printf("e_shstrndx = %x\n", elf_header->e_shentsize);

	return ret;
}

FILE *fp;
  
int find_and_copy_section_shstrtable(Elf32_Shdr *shHeaderArray, char *name){
	 int i, ret;
	 
	 for ( i = 0 ; i < 40 ; i ++)
	 	if(shHeaderArray[i].sh_type == SHT_STRTAB){
			if(debug_g)printf("i = %d\n", i);
	 	 	section_shStrTable = malloc(shHeaderArray[i].sh_size);
			if(section_shStrTable == NULL){
				printf("libdwarf: malloc failed Line :%d\n", __LINE__ );
				exit(0);
			}
	 
		if( fseek(fp, shHeaderArray[i].sh_offset, 0)!= 0){
			printf("libdwarf: fseek failed Line no %d\n", __LINE__ );
			exit(1);
		}
	 
		ret = fread(section_shStrTable, 1, shHeaderArray[i].sh_size, fp);
		if(ret < 0){
			printf("libdwarf: fread error Line %d\n", __LINE__);
			exit(0);
		}
	 	if(strcmp(name, section_shStrTable + shHeaderArray[i].sh_name) == 0){
		    if(debug_g)printf("%s %d\n", section_shStrTable + shHeaderArray[i].sh_name, i);
			break;
		}else{
			printf("libdwarf: shStrTable name mismatch\n");
			free(section_shStrTable);
			exit(0);
		}
	} 
	 return 1;
}

int copy_section(Elf32_Shdr *shHeaderArray, char *name, int num_sections, enum section_type type, char **dest, int *size){

	int i;
	
	for(i=0 ; i < num_sections ; i++)
		if(shHeaderArray[i].sh_type == type)		
			if(strcmp(name, section_shStrTable + shHeaderArray[i].sh_name) == 0)
				break;
				
	if(i == num_sections){
		printf("DWARF LIB error: cannot find section LINE %d\n", __LINE__);
		exit(0);
	}
	
	*dest = malloc(shHeaderArray[i].sh_size);
	if(*dest == NULL){
		printf("DWARF LIB error: malloc failed Line %d\n", __LINE__);
		exit(0);
	}
	if( fseek(fp, shHeaderArray[i].sh_offset, 0)!= 0){
		printf("libdwarf: fseek failed Line %d\n",__LINE__);
		exit(0);
	}
	if(fread(*dest, 1, shHeaderArray[i].sh_size, fp) < 0){
		printf("libdwarf: fread failed Line %d\n", __LINE__);
		exit(0);
	}
		
	if(size !=NULL)
		*size = shHeaderArray[i].sh_size;	
		
	return 0;
}

int print_all_section_names(Elf32_Shdr *shHeaderArray, int num_sections)
{
 	int i;

	for (i =0 ; i < num_sections ; i++){
			printf("===================================================================\n");
			printf("%s size = %d or 0x%x bytes %d KB\n", shHeaderArray[i].sh_name + section_shStrTable,shHeaderArray[i].sh_size, shHeaderArray[i].sh_size,shHeaderArray[i].sh_size/1024);
			printf("session offset = %d %x\n", shHeaderArray[i].sh_offset, shHeaderArray[i].sh_offset);
			printf("===================================================================\n");
	}
}

int find_symbol(char *str){
	Elf32_Sym *symbol, *last_symbol;

	int index = section_header_index(SHT_SYMTAB, ".symtab");
	last_symbol = section_symTable +section_header_array[index].sh_size;
 
	for (symbol = section_symTable; symbol < last_symbol ; symbol++){
		if(strcmp(str, symbol->st_name + section_strTable) == 0 )
			break;
	} 
	if(symbol >= last_symbol){
		printf("cannot find symbol\n");
		return -1;
	}
	if(debug_g)printf("Got symbol %s at %d symbol size = %d value = %x\n",symbol->st_name + section_strTable , symbol, symbol->st_size, symbol->st_value);
	if(debug_g)printf("shndx = %d\n", symbol->st_shndx);
	return symbol->st_value;
}
#if 0//arun deleteme
int find_nth_symbol(int index, char *sym_name, unsigned int *sym_val){
	Elf32_Sym *symbol, *last_symbol;
	
    int symtab_index = section_header_index(SHT_SYMTAB, ".symtab");
    last_symbol = section_symTable + section_header_array[symtab_index].sh_size;

    symbol = section_symTable;
	symbol += index;

    if(symbol > last_symbol)
        return -1;

    strcpy(sym_name, symbol->st_name + section_strTable);
    *sym_val = symbol->st_value;
    return 0;
} 
#endif

FILE * open_n_copy_sections(char *file_path)
{
 	int ret;
	
    if(file_path == NULL){
        printf("filename is NULL Line No: %d", __LINE__);
		return 0;
	}
                             
    if(debug_g)printf("%s\n", file_path );
    fp = fopen(file_path, "rb");
    if(!fp){
        printf("fopen failed \n");
        return 0;                                
    }
               
    ret = fread(&elf_header, 1, sizeof(elf_header), fp);
	if(ret < 0){
		printf("libdwarf: fread error LINE %d\n",__LINE__);
		return -1;
	}
    if(debug_g)printf("size of elfheader %d\n", sizeof(elf_header));
    if(debug_g)printf("%x %c%c%c\n", elf_header.e_ident[0], elf_header.e_ident[1],elf_header.e_ident[2],elf_header.e_ident[3]);
    
    if(verify_elf_header(&elf_header)){
		if(debug_g)printf("ELF image header correct\n");    
	}else {
	    printf("Wrong header in elf\n");
 		return -1;
	}
    
	/* print the section headers */
	ret =  fseek(fp, elf_header.e_shoff, 0);
	if(ret != 0){
		printf("libdwarf: Error in fseek LINE %d\n", __LINE__);
		return -1;
	}
	ret = fread(section_header_array, 1, elf_header.e_shentsize * elf_header.e_shnum, fp);
	if(ret < 0){
		printf("libdwarf: fread error LINE %d\n",__LINE__);
		return -1;
	}
	if(debug_g)printf("section headers read size = %d\n", ret);
	if(debug_g)printf("%x %d\n", section_header_array[1].sh_name,section_header_array[1].sh_name);
	/* Find and copy section string table */
	ret = find_and_copy_section_shstrtable(section_header_array, ".shstrtab");
	if(ret < 0){
		printf("libdwarf: error in find_and_copy_section_shstrtable LINE = %d\n", __LINE__ );
		exit(0);
	}
	if(debug_g)
		ret = print_all_section_names(section_header_array, elf_header.e_shnum);
	copy_section(section_header_array, ".symtab", elf_header.e_shnum, SHT_SYMTAB, &section_symTable, NULL);
	copy_section(section_header_array, ".strtab", elf_header.e_shnum, SHT_STRTAB, &section_strTable, NULL);
	//find_symbol("__log_buf");
	//find_symbol("init_task");
	
	read_n_copy_dwarf_sections();
	return fp;
}

extern void free_dwarf_dynamic_memory(void);
void free_dynamic_memory()
{
	free(section_shStrTable);
	free(section_symTable);
	free(section_strTable);
	free_dwarf_dynamic_memory();
}
void test_fun(void);

/*eg str: "init_task.mm.mmap_cache.vm_flags"
 * where init_task is a variable */
DLLIMPORT UINT dwarf_get_member_offset (char *str, int *buffer, char *file)
{
    int c, ret, i,j; 
  	char string_table_section_buff[50];
	int die_off, struct_type_die_offset, member_location, what_type, ptr_type;
	char delims[] = ".";
	char *variable_name, *member_name;
	char *string_ptr;
//#define debug_g 1
	string_ptr =  malloc(strlen(str) + 1);
	if(string_ptr == NULL){
		printf("malloc failed LINE %d\n", __LINE__);
		exit(0);
	}
	strcpy(string_ptr, str);

	fp = open_n_copy_sections(file);
	if(!fp){
        printf("fopen failed \n");
        return -1;
    }
	
	variable_name = strtok(string_ptr, delims);
	if(debug_g)printf("variable_name = %s\n", variable_name);
	
	member_name = strtok(NULL, delims);
	if(debug_g) printf("member_name = %s\n", member_name);
	die_off = get_global_variable_die_offset(variable_name);
	if(debug_g)
		printf("die_off = %d for %s \n", die_off, variable_name);
	if(die_off < 0){
		printf("invalid variable name Line %d\n", __LINE__);
		return -1;
	}
	struct_type_die_offset = find_struct_type_die_offset(die_off, &what_type);
	i = 0;
	
	do {
		if(debug_g)printf("struct type offset = %d 0x%x what_type = %x\n", struct_type_die_offset, struct_type_die_offset, what_type);
		if(debug_g)printf("what_type = %x member_name = %s\n", what_type, member_name);
	
		ptr_type = reach_struct_defintion(struct_type_die_offset);
		if(debug_g)printf("called reach_struct_defintion i/p = 0x%x o/p = 0x%x\n", what_type, ptr_type);
		if(debug_g)printf("ret = 0x%x\n", ptr_type);
		if(ptr_type > 0)
			struct_type_die_offset = ptr_type;		

		member_location = find_data_member_location(struct_type_die_offset, member_name, &what_type);
		if(debug_g)printf("member name = %s member offset = %d 0x%x what_type = %x\n",member_name, member_location, member_location, what_type);	
		buffer[i] = member_location;
		if(member_location < 0)
			return -1;
		i++;
		struct_type_die_offset = what_type;
		member_name = strtok(NULL, delims);
		
	}while(member_name != NULL);

	if(debug_g)
		for (j = 0; j < i ; j++)
			printf("%d = %d\n", j, buffer[j]);
		
    fclose(fp);
	free(string_ptr);
	free_dynamic_memory();
	return 0;
	#undef debug_g
}

int reach_struct_defintion(int what_type){
	int type;

	while(1){
		
		type = is_die_a_array_type(what_type);
		if(type)
			goto next;
		type = is_die_a_typedef_type(what_type);
		if(type)
			goto next;
		type =  is_die_a_ptr_type(what_type);
		if(type)
			goto next;
		/* not any of these(array, typedef, ptr). So lets break */
		break;
		next:
			what_type = type;
	}
return what_type;
}

struct name_list{
	char name[200];
	int die_offset;
	struct name_list *next;
};
/* "task_struct.mm.rss_stat.count" */
DLLIMPORT INT dwarf_get_array_count (char *str, int *count, char *file)
{
	int off, what_type ,ptr_type,array_type, struct_type_die_off,member_die_off;
	char delims[] = ".";
	char *this_str;
	struct name_list *head, *temp, *parse;
	int type, element_size, member_size, no_of_elements = 0, die_off, ret;
	char array_name[50];	
	char *string_ptr = malloc(strlen(str) + 1);
		
	if(string_ptr == NULL){
		printf("malloc failed LINE %d\n", __LINE__);
		exit(0);
	}
	strncpy(string_ptr, str, strlen(str));
	string_ptr[strlen(str)] = '\0';

	if(debug_g)printf("\nstring_ptr = %s\n", string_ptr);

	fp = open_n_copy_sections(file);
	if(!fp){
        	printf("fopen failed \n");
	        return -1;                                
    	}

	this_str = strtok(string_ptr, delims);
	if(this_str == NULL){
		printf("libdwarf: invalid name passed LINE %d\n", __LINE__);
		return -1;
	}
	no_of_elements++;
    temp = malloc(sizeof(struct name_list));
    if(temp == NULL){
		printf("libdwarf: malloc failed LINE %d\n", __LINE__);
        exit(0);
	}

        strcpy(temp->name, this_str);
        temp->next = NULL;
        head = temp;
        do {
                this_str = strtok(NULL, delims);
                if(this_str){
                        temp = malloc(sizeof(struct name_list));
                        strcpy(temp->name, this_str);
                        temp->next=NULL;
                        for(parse = head; parse->next != NULL ; parse = parse->next);
                        parse->next = temp;
						no_of_elements++;
                }

        }while(this_str != NULL);
		
		/* if no of elements  is one then it is a variable
		 * so we have to search in pubnames section
		 */
		if(no_of_elements == 1){
			die_off = get_global_variable_die_offset(head->name);
			if(die_off < 0 ){
				printf("invalid variable name %s LINE %d\n", head->name, __LINE__);
				return -1;
			}
			die_off = find_struct_type_die_offset(die_off, &what_type);
			if(debug_g)printf("die_off = %x what_type = %x\n", die_off, what_type);
			/* [    6a]    array_type
             * type                 [    25]
             * sibling              [    7a]
			 *  [    73]     subrange_type
			 *  			 type                 [    7a]
			 *				 upper_bound          9
			 *	we have [6a]. now find upper_bound to get the no of elements in array
			 */
			ret = find_upper_bound(die_off);
			if(ret < 0)
				return ret;//error
			else {	
				*count = (ret + 1);//no of elements
				goto out;
			}			
		}

        for(parse = head; parse != NULL ; parse = parse->next){
			if(debug_g)printf("parse->name = %s\n", parse->name);
			if(parse->next == NULL)
				strcpy(array_name , parse->name);
		}

	struct_type_die_off = find_die_offset_from_name(head->name);
	if(struct_type_die_off < 0 ){
		printf("cannot find offset LINE %d\n", __LINE__);
		exit(0);
	}
	head->die_offset = struct_type_die_off;
	if(debug_g)	printf("DIEOFFSET1 = %x\n", struct_type_die_off);

	/*eg: start from mm */
	for(parse = head->next ; parse != NULL ; parse = parse->next){

		member_die_off = struct_type_die_off;

		ptr_type = reach_struct_defintion(struct_type_die_off);
		if(debug_g)printf("ptr_type = 0x%x\n", ptr_type);
		
		if(ptr_type > 0)
			struct_type_die_off = ptr_type;

		member_die_off = find_data_member_die_offset(struct_type_die_off, parse->name, &what_type);
		if(debug_g)printf("============================\n");
		if(debug_g)printf("member_die_off = %x what_type = 0x%x\n", member_die_off, what_type);
		parse->die_offset = member_die_off;
		struct_type_die_off = what_type;
		
	}
	/* now member_die_off will be pointing to count in "struct_task.mm.rss_stat.count" */
	if(debug_g)printf("member_die_off = %x what_type = %x\n", member_die_off, what_type);
	
	what_type = reach_struct_defintion(struct_type_die_off);
	if(what_type > 0 )
			struct_type_die_off = what_type;
	if(debug_g)	printf("struct_type_die_off = 0x%x\n", what_type);
	element_size = get_structure_size(what_type);
	if(debug_g)printf("element_size = %d\n", element_size);
	
	/* parse till rss_stat now */
	for(parse = head; parse->next->next != NULL; parse = parse->next){
		if(debug_g)printf("name = %s offset =0x%x\n", parse->name, parse->die_offset);
	}
	/* parse has rss_stat now */
	if(debug_g)printf("got offset = 0x%x name = %s\n", parse->die_offset, parse->name);

	struct_type_die_off = find_struct_type_die_offset(parse->die_offset, &what_type);

	what_type = reach_struct_defintion(struct_type_die_off);
	if(what_type > 0)
			struct_type_die_off = what_type;
	if(debug_g)printf("struct_type_die_off = 0x%x\n", what_type);

	if(debug_g)printf("array name = %s\n", array_name);
	member_size = get_member_size(what_type, array_name);
	if(debug_g)printf("member_size = %d\n", member_size);
	if(debug_g)printf("member_size/element_size = %d\n", member_size/element_size);

	if(element_size == 0){
		printf("libdwarf error Line %d\n", __LINE__);
		exit(0);
	}
	*count = member_size/element_size;
out:
	free_dynamic_memory();
	fclose(fp);
	free(string_ptr);
	return 0;

}
/* get a member offset 
 i/p struct type name 
 eg: get_member_offset_from_typename("task_struct.mm.mmap_cache.vm_flags", buffer, "vmlinux");
 where buffer can be an array or integer pointer.
 */
DLLIMPORT INT dwarf_get_member_offset_from_typename(char *str_name, int *buffer, char *file)
{    int c, ret, i,j;
  	char string_table_section_buff[50];
	int die_off, struct_type_die_offset, member_location, what_type, ptr_type;
	char delims[] = ".";
	char *variable_name, *member_name;
	char *string_ptr = malloc(strlen(str_name) + 1);
//#define debug_g 1
	if(string_ptr == NULL){
		printf("malloc failed LINE %d\n", __LINE__);
		exit(0);
	}
	strncpy(string_ptr, str_name, strlen(str_name));
	string_ptr[strlen(str_name)] = '\0';
	
	if(debug_g)printf("str_name = %s\n", str_name);
	
	fp = open_n_copy_sections(file);
	if(!fp){
        printf("fopen failed \n");
        return -1;
    	}

	variable_name = strtok(string_ptr, delims);
	if(debug_g)printf("variable_name = %s\n", variable_name);
	
	member_name = strtok(NULL, delims);
	if(debug_g) printf("member_name = %s\n", member_name);

	struct_type_die_offset = find_die_offset_from_name(variable_name);
	if(struct_type_die_offset < 0 ){
		printf("cannot find offset LINE %d\n", __LINE__);
		return -1;
	}
	if(debug_g)printf(" LINE %d DIEOFFSET = %x\n", __LINE__, struct_type_die_offset);
	
	i = 0;
	do {
		if(debug_g)printf("struct type offset = %d 0x%x what_type = %x\n", struct_type_die_offset, struct_type_die_offset, what_type);
		if(debug_g)printf("what_type = %x member_name = %s\n", what_type, member_name);
	
		ptr_type = reach_struct_defintion(struct_type_die_offset);
		if(debug_g)	printf("ptr_type = 0x%x\n", ptr_type);

		if(ptr_type > 0)
			struct_type_die_offset = ptr_type;
			
		member_location = find_data_member_location(struct_type_die_offset, member_name, &what_type);
		if(debug_g)	printf("member offset = %d 0x%x what_type = %x\n", member_location, member_location, what_type);	
		buffer[i] = member_location;
		if(member_location < 0)
			return -1;
		i++;
		struct_type_die_offset = what_type;
		member_name = strtok(NULL, delims);
		
	}while(member_name != NULL);
	
	if(debug_g)
		for (j = 0; j < i ; j++)
			printf("%d = %d\n", j, buffer[j]);
			
    fclose(fp);
	free(string_ptr);
	free_dynamic_memory();
	return 0;
	#undef debug_g

}



DLLIMPORT INT dwarf_get_addr(char *variable, unsigned int *addr, char *file)
{
	int ret = 0;
	
	fp = open_n_copy_sections(file);
	*addr = find_symbol(variable);
	if(*addr < 0){
		printf("cannot find symbol\n");
		ret = -1;
	}
	free_dynamic_memory();
	fclose(fp);
	return ret;
}
struct elf_symbol{
char 			name[75];
unsigned int 	addr;
};
int symbol_count;
DLLIMPORT INT dwarf_get_symbols_count(char *file)
{
	int count = 0;
	fp = open_n_copy_sections(file);
	
	Elf32_Sym *symbol, *last_symbol;
	
    int symtab_index = section_header_index(SHT_SYMTAB, ".symtab");
    last_symbol = section_symTable + section_header_array[symtab_index].sh_size;
	
	for(symbol = section_symTable ; symbol < last_symbol ; symbol++){

		if(strncmp(symbol->st_name + section_strTable, "$a", 2) == 0)
			continue;
		if(strncmp(symbol->st_name + section_strTable, "$d", 2) == 0)
			continue;
		if(strncmp(symbol->st_name + section_strTable, "$t", 2) == 0)
			continue;		
		if(strncmp(symbol->st_name + section_strTable, "__crc_", 6) == 0)
			continue;
		if(symbol->st_name  == 0)
			continue;
		count++;
	}
	symbol_count = count;
	return count;
}

DLLIMPORT int dwarf_get_elf_symbols(struct elf_symbol *elf_symbol)
{
	int ret = 0;	
	Elf32_Sym *symbol, *last_symbol;
	
    int symtab_index = section_header_index(SHT_SYMTAB, ".symtab");
    last_symbol = section_symTable + section_header_array[symtab_index].sh_size;
	
	for(symbol = section_symTable ; symbol < last_symbol ; symbol++){
	
		if(strncmp(symbol->st_name + section_strTable, "$a", 2) == 0)
			continue;
		if(strncmp(symbol->st_name + section_strTable, "$d", 2) == 0)
			continue;
		if(strncmp(symbol->st_name + section_strTable, "$t", 2) == 0)
			continue;		
		if(strncmp(symbol->st_name + section_strTable, "__crc_", 6) == 0)
			continue;
		if(symbol->st_name  == 0)
			continue;

		strncpy(elf_symbol->name, symbol->st_name + section_strTable, strlen(symbol->st_name + section_strTable));

		elf_symbol->addr = symbol->st_value;
		elf_symbol++;
	}	
	free_dynamic_memory();
	fclose(fp);
	return symbol_count;
}

/* i/p name of a structure type
return size of structure*/
DLLIMPORT INT dwarf_get_sizeof_type(char *str_name, char *file)
{
	int off,  what_type, size;
	
	fp = open_n_copy_sections(file);
    if(!fp){
        printf("fopen failed \n");
        return -1;                                
    }

	off = find_die_offset_from_name(str_name);
	if(off < 0 ){
		printf("cannot find offset LINE %d\n", __LINE__);
		return -1;
	}
	if(debug_g)printf("LINE %d DIEOFFSET = %x\n",__LINE__, off);
	
	size = get_structure_size(off);
	fclose(fp);
	free_dynamic_memory();

	return size;
}

void dwarf_debug_enable(int enable)
{
	debug_g = enable;
}

BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{ int i;
    switch (reason)
    {
      case DLL_PROCESS_ATTACH:
	 	break;

      case DLL_PROCESS_DETACH:
	    break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
    }

    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}
