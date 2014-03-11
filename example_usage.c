/*
 * example_usage.c Example showing how to use dwarf ibrary.
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
//#include "dll.h"
#include <windows.h>
#include <stdio.h>

typedef unsigned int (CALLBACK* dwarf_get_member_offset_fn)(char *str, int *buffer, char *file);
dwarf_get_member_offset_fn dwarf_get_member_offset;

typedef INT (CALLBACK* dwarf_get_array_count_fn)(char *str, int *count, char *file);
dwarf_get_array_count_fn dwarf_get_array_count;

typedef INT (CALLBACK* dwarf_get_member_offset_from_typename_fn)(char *str_name, int *buffer, char *file);
dwarf_get_member_offset_from_typename_fn dwarf_get_member_offset_from_typename;
HINSTANCE Hdll; /* handle to dll */

typedef void (CALLBACK* dwarf_debug_enable_fn)(int enable);
dwarf_debug_enable_fn dwarf_debug_enable;

typedef int (CALLBACK* dwarf_get_symbols_count_fn)(char *file);
dwarf_get_symbols_count_fn dwarf_get_symbols_count;

struct elf_symbol{
char 			name[75];
unsigned int 	addr;
};

typedef int (CALLBACK *  dwarf_get_elf_symbols_fn)(struct elf_symbol *elf_symbol);
dwarf_get_elf_symbols_fn dwarf_get_elf_symbols;

typedef int (CALLBACK* dwarf_get_sizeof_type_fn)(char *name, char *file);
dwarf_get_sizeof_type_fn dwarf_get_sizeof_type;

typedef int (CALLBACK* dwarf_get_addr_fn)(char *variable, unsigned int *addr, char *file);
dwarf_get_addr_fn dwarf_get_addr;

int main(int argc, char *argv[])
{
	UINT offset;
	int buffer[10],i, ret, count;
	char name[100];
	unsigned int address;
 	//get dll func address
	Hdll = LoadLibrary("dwarf");
    if(!Hdll){
        printf("Missing DLL: dwarf.dll");return FALSE;
    }

	dwarf_get_sizeof_type = (dwarf_get_sizeof_type_fn)GetProcAddress(Hdll , "dwarf_get_sizeof_type");
	if(!dwarf_get_sizeof_type){
		printf("Function dwarf_get_sizeof_type doesnt exist\n");
		exit(0);
	}

	dwarf_get_member_offset = (dwarf_get_member_offset_fn)GetProcAddress(Hdll , "dwarf_get_member_offset");
	if(!dwarf_get_member_offset){
		printf("Function dwarf_get_member_offset doesnt exist\n");
		exit(0);
	}

	dwarf_debug_enable = (dwarf_debug_enable_fn) GetProcAddress(Hdll, "dwarf_debug_enable");
		if(!dwarf_debug_enable){
		printf("Function dwarf_debug_enable doesnt exist\n");
		exit(0);
	}

	dwarf_get_symbols_count = (dwarf_get_symbols_count_fn) GetProcAddress(Hdll, "dwarf_get_symbols_count");
		if(!dwarf_get_symbols_count){
		printf("Function dwarf_get_symbols_count_fn doesnt exist\n");
		exit(0);
	}

	dwarf_get_elf_symbols = (dwarf_get_elf_symbols_fn) GetProcAddress(Hdll, "dwarf_get_elf_symbols");
		if(!dwarf_get_elf_symbols){
		printf("Function dwarf_get_elf_symbols doesnt exist\n");
		exit(0);
	}

	dwarf_get_member_offset_from_typename = (dwarf_get_member_offset_from_typename_fn)GetProcAddress(Hdll , "dwarf_get_member_offset_from_typename");
	if(!dwarf_get_member_offset_from_typename){
		printf("No such func name dwarf_get_member_offset_from_typename\n");
		exit(0);
	}

	dwarf_get_array_count = (dwarf_get_array_count_fn)GetProcAddress(Hdll , "dwarf_get_array_count");
	if(!dwarf_get_array_count){
		printf("No such func name dwarf_get_array_coun\n");
		exit(0);
	}
	dwarf_get_addr = (dwarf_get_addr_fn)GetProcAddress(Hdll , "dwarf_get_addr");
	if(!dwarf_get_addr){
		printf("No such func name dwarf_get_array_coun\n");
		exit(0);
	}

	/* importing funciton from lib over */
#if 1

	count = dwarf_get_symbols_count("vmlinux");
	if(count < 0){
			   printf("error getting elf symbol count");
 			   exit(-1);
    }
    printf("count = %d\n", count);

	{
		struct elf_symbol *elf_symbol;
		elf_symbol = malloc(sizeof( struct elf_symbol) * count);
		if(elf_symbol == NULL){
			printf("connot allocate memmory\n");
			exit(0);
		}
		if(dwarf_get_elf_symbols(elf_symbol)<0){
			printf("error\n");
			exit(0);
		}
	printf("reach back\n");

	//printf("%s %x %s %x\n", elf_symbol[0]->name, elf_symbol[0]->addr, elf_symbol[100]->name, elf_symbol[100]->addr);
		printf("%s %x %s %x\n", elf_symbol[0].name, elf_symbol[0].addr, elf_symbol[100].name, elf_symbol[100].addr);
	}
		return 0;

	offset = dwarf_get_member_offset("init_task.mm.mmap_cache.vm_flags", buffer, "vmlinux");
	for(i = 0 ; i < 3 ; i++){
		printf("APP dwarf_get_member_offset(init_task.mm.mmap_cache.vm_flags) %d %d\n", i, buffer[i]);
		buffer[i] = 0;
	}

	if (dwarf_get_member_offset("init_task.comm", buffer, "vmlinux")) {
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
    }

	printf("APP dwarf_get_member_offset(init_task.comm) value = %d\n", buffer[0]);

	ret = dwarf_get_sizeof_type("task_struct", "vmlinux");
	printf("APP = %d\n", ret);

	if (dwarf_get_member_offset_from_typename("pglist_data.nr_zones", buffer, "vmlinux")) {
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP dwarf_get_member_offset_from_typename(pglist_data.nr_zones)	value %d \n", buffer[0]);

	if(dwarf_get_member_offset("contig_page_data.nr_zones", buffer, "vmlinux")) {
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP dwarf_get_member_offset(contig_page_data.nr_zones)	value %d \n", buffer[0]);

	if (dwarf_get_member_offset("contig_page_data.node_zones.name", buffer, "vmlinux")) {
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
    }
	printf("APP dwarf_get_member_offset(contig_page_data.node_zones.name) %d %d\n", buffer[0], buffer[1]);

	if (dwarf_get_member_offset_from_typename("pglist_data.node_zones.name", buffer, "vmlinux")) {
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
    }
	printf("APP dwarf_get_member_offset_from_typename(pglist_data.node_zones.name) %d %d\n", buffer[0], buffer[1]);

	if (dwarf_get_member_offset_from_typename("task_struct.mm", buffer, "vmlinux")) {
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
    }
	printf("APP dwarf_get_member_offset_from_typename(task_struct.mm) %d\n", buffer[0]);



	if(dwarf_get_array_count("pglist_data.node_zones", buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP dwarf_get_array_count(pglist_data.node_zones)%d\n", buffer[0]);
	//buffer[0] = buffer[1] = 0;


	if(dwarf_get_array_count("pglist_data.node_zones.lowmem_reserve", buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP dwarf_get_array_count(pglist_data.node_zones.lowmem_reserve) %d\n", buffer[0]);

	if(dwarf_get_array_count("pglist_data.node_zones.lowmem_reserve", buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP dwarf_get_array_count(pglist_data.node_zones.lowmem_reserve) %d\n", buffer[0]);
#endif
#if 1
	printf("calling the curr fn\n");
	//dwarf_debug_enable(1);
	//if(dwarf_get_array_count("zone.free_area.free_list", buffer, "vmlinux")){
	if(dwarf_get_array_count("zone.free_area.free_list\0", buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("return succesfully\n");
	printf("APP dwarf_get_array_count(zone.free_area.free_list) %d\n", buffer[0]);
#endif
	#if 1
	if(dwarf_get_array_count("vm_stat", buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP %d\n", buffer[0]);
	#endif
	if(dwarf_get_array_count("irq_desc", buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP %d\n", buffer[0]);

	if(dwarf_get_array_count("task_struct.mm.rss_stat.count", buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP0 %d\n", buffer[0]);
	//dwarf_debug_enable(1);
	if(dwarf_get_addr("swapper_pg_dir",  buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP 0x%x\n", buffer[0]);

	if(dwarf_get_member_offset_from_typename("irq_desc.action.name", buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP %d %d\n", buffer[0], buffer[1]);

	if(dwarf_get_member_offset_from_typename("kmem_list3.slabs_full", buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP %d\n", buffer[0]);

	//if(dwarf_get_member_offset_from_typename("slab.inuse", buffer, "vmlinux")){
	if(dwarf_get_member_offset_from_typename("zone.name", buffer, "vmlinux")){
		fprintf(stderr,"Error:%d\n",__LINE__);
		return -1;
	}
	printf("APP zone.name %d 0x%x\n", buffer[0], buffer[0]);
	return 0;

}
