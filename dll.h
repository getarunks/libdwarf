/*
 * dll.h Header file for dwarf library.
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
#ifndef _DLL_H_
#define _DLL_H_

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */

/* get member offset
 * str example "init_task.mm.rss_stat"
 * returns the data_member_location in buffer[]
 * returns -1 in case of error
 * otherwise the nume of entries in buf.
 */
DLLIMPORT unsigned int dwarf_get_member_offset (char *str, int *buffer, char *file);
/* i/p name of a structure type
return size of structure*/
DLLIMPORT int dwarf_get_sizeof_type(char *name, char *file);
/* get kernel virtual address corresponding to the variable. Same as in System.map */
DLLIMPORT int dwarf_get_addr(char *variable, unsigned int *addr, char *file);
/* get the no of elements in an array
 * an example of str "task_struct.mm.rss_stat.count". Find elements of arrary count
 */
DLLIMPORT int dwarf_get_array_count (char *str, int *count, char *file);
/* get a member offset
 i/p struct type name
 eg: dwarf_get_member_offset_from_typename("task_struct.mm.mmap_cache.vm_flags", buffer, "vmlinux");;
 */
DLLIMPORT int dwarf_get_member_offset_from_typename(char *str_name, int *buffer, char *file);

/* Not implemented */
DLLIMPORT int dwarf_get_array_mem_names(char *array_var_name, char **buf);

/* This function returns symbols
return 0 on success and -1 when symbols ends
*/
DLLIMPORT int elf_find_all_symbols(int count, char *sym_name, unsigned int *sym_val, char *file);

/*Enable/disable dwarf lib debugging */
DLLIMPORT void dwarf_debug_enable(int enable);

#endif /* _DLL_H_ */
