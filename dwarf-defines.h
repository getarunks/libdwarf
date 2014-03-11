/*
 * dwarf-defines.h
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
#define DW_TAG_array_type               0x01
#define DW_TAG_class_type               0x02
#define DW_TAG_entry_point              0x03
#define DW_TAG_enumeration_type         0x04
#define DW_TAG_formal_parameter         0x05
#define DW_TAG_imported_declaration     0x08
#define DW_TAG_label                    0x0a
#define DW_TAG_lexical_block            0x0b
#define DW_TAG_member                   0x0d
#define DW_TAG_pointer_type             0x0f
#define DW_TAG_reference_type           0x10
#define DW_TAG_compile_unit             0x11
#define DW_TAG_string_type              0x12
#define DW_TAG_structure_type           0x13
#define DW_TAG_subroutine_type          0x15
#define DW_TAG_typedef                  0x16
#define DW_TAG_union_type               0x17
#define DW_TAG_unspecified_parameters   0x18
#define DW_TAG_variant                  0x19
#define DW_TAG_common_block             0x1a
#define DW_TAG_common_inclusion         0x1b
#define DW_TAG_inheritance              0x1c
#define DW_TAG_inlined_subroutine       0x1d
#define DW_TAG_module                   0x1e
#define DW_TAG_ptr_to_member_type       0x1f
#define DW_TAG_set_type                 0x20
#define DW_TAG_subrange_type            0x21
#define DW_TAG_with_stmt                0x22
#define DW_TAG_access_declaration       0x23
#define DW_TAG_base_type                0x24
#define DW_TAG_catch_block              0x25
#define DW_TAG_const_type               0x26
#define DW_TAG_constant                 0x27
#define DW_TAG_enumerator               0x28
#define DW_TAG_file_type                0x29
#define DW_TAG_friend                   0x2a
#define DW_TAG_namelist                 0x2b
#define DW_TAG_namelist_item            0x2c /* DWARF3/2 spelling */
#define DW_TAG_namelist_items           0x2c /* SGI misspelling/typo */
#define DW_TAG_packed_type              0x2d
#define DW_TAG_subprogram               0x2e
#define DW_TAG_template_type_parameter  0x2f /* DWARF3/2 spelling*/
#define DW_TAG_template_type_param      0x2f /* DWARF2   spelling*/
#define DW_TAG_template_value_parameter 0x30 /* DWARF3/2 spelling*/
#define DW_TAG_template_value_param     0x30 /* DWARF2   spelling*/
#define DW_TAG_thrown_type              0x31
#define DW_TAG_try_block                0x32
#define DW_TAG_variant_part             0x33
#define DW_TAG_variable                 0x34
#define DW_TAG_volatile_type            0x35
#define DW_TAG_dwarf_procedure          0x36  /* DWARF3 */
#define DW_TAG_restrict_type            0x37  /* DWARF3 */
#define DW_TAG_interface_type           0x38  /* DWARF3 */
#define DW_TAG_namespace                0x39  /* DWARF3 */
#define DW_TAG_imported_module          0x3a  /* DWARF3 */
#define DW_TAG_unspecified_type         0x3b  /* DWARF3 */
#define DW_TAG_partial_unit             0x3c  /* DWARF3 */
#define DW_TAG_imported_unit            0x3d  /* DWARF3 */
#define DW_TAG_mutable_type		0x3e
#define DW_TAG_condition                0x3f  /* DWARF3f */
#define DW_TAG_shared_type              0x40  /* DWARF3f */
#define DW_TAG_lo_user                  0x4080
#define DW_TAG_MIPS_loop                0x4081
#define DW_TAG_HP_array_descriptor      0x4090 /* HP */
#define DW_TAG_format_label             0x4101 /* GNU. Fortran. */
#define DW_TAG_function_template        0x4102 /* GNU. For C++ */
#define DW_TAG_class_template           0x4103 /* GNU. For C++ */
#define DW_TAG_GNU_BINCL                0x4104 /* GNU */
#define DW_TAG_GNU_EINCL                0x4105 /* GNU */
#define DW_TAG_ALTIUM_circ_type         0x5101 /* ALTIUM */
#define DW_TAG_ALTIUM_mwa_circ_type     0x5102 /* ALTIUM */
#define DW_TAG_ALTIUM_rev_carry_type    0x5103 /* ALTIUM */
#define DW_TAG_ALTIUM_rom               0x5111 /* ALTIUM */
#define DW_TAG_upc_shared_type          0x8765 /* UPC */
#define DW_TAG_upc_strict_type          0x8766 /* UPC */
#define DW_TAG_upc_relaxed_type         0x8767 /* UPC */
#define DW_TAG_PGI_kanji_type           0xa000 /* PGI */
#define DW_TAG_PGI_interface_block      0xa020 /* PGI */
#define DW_TAG_hi_user                  0xffff

#define DW_children_no                  0
#define DW_children_yes                 1

#define DW_FORM_addr                    0x01
#define DW_FORM_block2                  0x03
#define DW_FORM_block4                  0x04
#define DW_FORM_data2                   0x05
#define DW_FORM_data4                   0x06
#define DW_FORM_data8                   0x07
#define DW_FORM_string                  0x08
#define DW_FORM_block                   0x09
#define DW_FORM_block1                  0x0a
#define DW_FORM_data1                   0x0b
#define DW_FORM_flag                    0x0c
#define DW_FORM_sdata                   0x0d
#define DW_FORM_strp                    0x0e
#define DW_FORM_udata                   0x0f
#define DW_FORM_ref_addr                0x10
#define DW_FORM_ref1                    0x11
#define DW_FORM_ref2                    0x12
#define DW_FORM_ref4                    0x13
#define DW_FORM_ref8                    0x14
#define DW_FORM_ref_udata               0x15
#define DW_FORM_indirect                0x16

#define DW_AT_sibling                           0x01
#define DW_AT_location                          0x02
#define DW_AT_name                              0x03
#define DW_AT_ordering                          0x09
#define DW_AT_subscr_data                       0x0a
#define DW_AT_byte_size                         0x0b
#define DW_AT_bit_offset                        0x0c
#define DW_AT_bit_size                          0x0d
#define DW_AT_element_list                      0x0f
#define DW_AT_stmt_list                         0x10
#define DW_AT_low_pc                            0x11
#define DW_AT_high_pc                           0x12
#define DW_AT_language                          0x13
#define DW_AT_member                            0x14
#define DW_AT_discr                             0x15
#define DW_AT_discr_value                       0x16
#define DW_AT_visibility                        0x17
#define DW_AT_import                            0x18
#define DW_AT_string_length                     0x19
#define DW_AT_common_reference                  0x1a
#define DW_AT_comp_dir                          0x1b
#define DW_AT_const_value                       0x1c
#define DW_AT_containing_type                   0x1d
#define DW_AT_default_value                     0x1e
#define DW_AT_inline                            0x20
#define DW_AT_is_optional                       0x21
#define DW_AT_lower_bound                       0x22
#define DW_AT_producer                          0x25
#define DW_AT_prototyped                        0x27
#define DW_AT_return_addr                       0x2a
#define DW_AT_start_scope                       0x2c
#define DW_AT_bit_stride                        0x2e /* DWARF3 name */
#define DW_AT_stride_size                       0x2e /* DWARF2 name */
#define DW_AT_upper_bound                       0x2f
#define DW_AT_abstract_origin                   0x31
#define DW_AT_accessibility                     0x32
#define DW_AT_address_class                     0x33
#define DW_AT_artificial                        0x34
#define DW_AT_base_types                        0x35
#define DW_AT_calling_convention                0x36
#define DW_AT_count                             0x37
#define DW_AT_data_member_location              0x38
#define DW_AT_decl_column                       0x39
#define DW_AT_decl_file                         0x3a
#define DW_AT_decl_line                         0x3b
#define DW_AT_declaration                       0x3c
#define DW_AT_discr_list                        0x3d
#define DW_AT_encoding                          0x3e
#define DW_AT_external                          0x3f
#define DW_AT_frame_base                        0x40
#define DW_AT_friend                            0x41
#define DW_AT_identifier_case                   0x42
#define DW_AT_macro_info                        0x43
#define DW_AT_namelist_item                     0x44
#define DW_AT_priority                          0x45
#define DW_AT_segment                           0x46
#define DW_AT_specification                     0x47
#define DW_AT_static_link                       0x48
#define DW_AT_type                              0x49
#define DW_AT_use_location                      0x4a
#define DW_AT_variable_parameter                0x4b
#define DW_AT_virtuality                        0x4c
#define DW_AT_vtable_elem_location              0x4d
#define DW_AT_allocated                         0x4e /* DWARF3 */
#define DW_AT_associated                        0x4f /* DWARF3 */
#define DW_AT_data_location                     0x50 /* DWARF3 */
#define DW_AT_byte_stride                       0x51 /* DWARF3f */
#define DW_AT_stride                            0x51 /* DWARF3 (do not use) */
#define DW_AT_entry_pc                          0x52 /* DWARF3 */
#define DW_AT_use_UTF8                          0x53 /* DWARF3 */
#define DW_AT_extension                         0x54 /* DWARF3 */
#define DW_AT_ranges                            0x55 /* DWARF3 */
#define DW_AT_trampoline                        0x56 /* DWARF3 */
#define DW_AT_call_column                       0x57 /* DWARF3 */
#define DW_AT_call_file                         0x58 /* DWARF3 */
#define DW_AT_call_line                         0x59 /* DWARF3 */
#define DW_AT_description                       0x5a /* DWARF3 */
#define DW_AT_binary_scale                      0x5b /* DWARF3f */
#define DW_AT_decimal_scale                     0x5c /* DWARF3f */
#define DW_AT_small                             0x5d /* DWARF3f */
#define DW_AT_decimal_sign                      0x5e /* DWARF3f */
#define DW_AT_digit_count                       0x5f /* DWARF3f */
#define DW_AT_picture_string                    0x60 /* DWARF3f */
#define DW_AT_mutable                           0x61 /* DWARF3f */
#define DW_AT_threads_scaled                    0x62 /* DWARF3f */
#define DW_AT_explicit                          0x63 /* DWARF3f */
#define DW_AT_object_pointer                    0x64 /* DWARF3f */
#define DW_AT_endianity                         0x65 /* DWARF3f */
#define DW_AT_elemental                         0x66 /* DWARF3f */
#define DW_AT_pure                              0x67 /* DWARF3f */
#define DW_AT_recursive                         0x68 /* DWARF3f */
#define DW_AT_lo_user                           0x2000
#define DW_AT_HP_block_index                    0x2000  /* HP */
#define DW_AT_MIPS_fde                          0x2001 /* MIPS/SGI */
#define DW_AT_MIPS_loop_begin                   0x2002 /* MIPS/SGI */
#define DW_AT_MIPS_tail_loop_begin              0x2003 /* MIPS/SGI */
#define DW_AT_MIPS_epilog_begin                 0x2004 /* MIPS/SGI */
#define DW_AT_MIPS_loop_unroll_factor           0x2005 /* MIPS/SGI */
#define DW_AT_MIPS_software_pipeline_depth      0x2006 /* MIPS/SGI */
#define DW_AT_MIPS_linkage_name                 0x2007 /* MIPS/SGI */
#define DW_AT_MIPS_stride                       0x2008 /* MIPS/SGI */
#define DW_AT_MIPS_abstract_name                0x2009 /* MIPS/SGI */
#define DW_AT_MIPS_clone_origin                 0x200a /* MIPS/SGI */
#define DW_AT_MIPS_has_inlines                  0x200b /* MIPS/SGI */
#define DW_AT_MIPS_stride_byte                  0x200c /* MIPS/SGI */
#define DW_AT_MIPS_stride_elem                  0x200d /* MIPS/SGI */
#define DW_AT_MIPS_ptr_dopetype                 0x200e /* MIPS/SGI */
#define DW_AT_MIPS_allocatable_dopetype         0x200f /* MIPS/SGI */
#define DW_AT_MIPS_assumed_shape_dopetype       0x2010 /* MIPS/SGI */
#define DW_AT_MIPS_assumed_size                 0x2011 /* MIPS/SGI */
#define DW_AT_HP_raw_data_ptr                   0x2012 /* HP */
#define DW_AT_HP_pass_by_reference              0x2013 /* HP */
#define DW_AT_HP_opt_level                      0x2014 /* HP */
#define DW_AT_HP_prof_version_id                0x2015 /* HP */
#define DW_AT_HP_opt_flags                      0x2016 /* HP */
#define DW_AT_HP_cold_region_low_pc             0x2017 /* HP */
#define DW_AT_HP_cold_region_high_pc            0x2018 /* HP */
#define DW_AT_HP_all_variables_modifiable       0x2019 /* HP */
#define DW_AT_HP_linkage_name                   0x201a /* HP */
#define DW_AT_HP_prof_flags                     0x201b /* HP */
#define DW_AT_sf_names                          0x2101 /* GNU */
#define DW_AT_src_info                          0x2102 /* GNU */
#define DW_AT_mac_info                          0x2103 /* GNU */
#define DW_AT_src_coords                        0x2104 /* GNU */
#define DW_AT_body_begin                        0x2105 /* GNU */
#define DW_AT_body_end                          0x2106 /* GNU */
#define DW_AT_GNU_vector                        0x2107 /* GNU */
#define DW_AT_VMS_rtnbeg_pd_address             0x2201 /* VMS */
#define DW_AT_PGI_lbase                         0x3a00 /* PGI */
#define DW_AT_PGI_soffset                       0x3a01 /* PGI */
#define DW_AT_PGI_lstride                       0x3a02 /* PGI */
#define DW_AT_upc_threads_scaled                0x3210 /* UPC */
#define DW_AT_hi_user                           0x3fff

#define DW_LANG_C89                     0x0001
#define DW_LANG_C                       0x0002
#define DW_LANG_Ada83                   0x0003
#define DW_LANG_C_plus_plus             0x0004
#define DW_LANG_Cobol74                 0x0005
#define DW_LANG_Cobol85                 0x0006
#define DW_LANG_Fortran77               0x0007
#define DW_LANG_Fortran90               0x0008
#define DW_LANG_Pascal83                0x0009
#define DW_LANG_Modula2                 0x000a
#define DW_LANG_Java                    0x000b /* DWARF3 */
#define DW_LANG_C99                     0x000c /* DWARF3 */
#define DW_LANG_Ada95                   0x000d /* DWARF3 */
#define DW_LANG_Fortran95               0x000e /* DWARF3 */
#define DW_LANG_PLI                     0x000f /* DWARF3 */
#define DW_LANG_ObjC                    0x0010 /* DWARF3f */
#define DW_LANG_ObjC_plus_plus          0x0011 /* DWARF3f */
#define DW_LANG_UPC                     0x0012 /* DWARF3f */
#define DW_LANG_D                       0x0013 /* DWARF3f */
#define DW_LANG_lo_user                 0x8000
#define DW_LANG_Mips_Assembler          0x8001 /* MIPS   */
#define DW_LANG_Upc                     0x8765
#define DW_LANG_ALTIUM_Assembler        0x9101  /* ALTIUM */
#define DW_LANG_hi_user                 0xffff

#define DW_ID_case_sensitive            0x00
#define DW_ID_up_case                   0x01
#define DW_ID_down_case                 0x02
#define DW_ID_case_insensitive          0x03

/* DWARF location operation encodings.  */
enum
  {
    DW_OP_addr = 0x03,		/* Constant address.  */
    DW_OP_deref = 0x06,
    DW_OP_const1u = 0x08,	/* Unsigned 1-byte constant.  */
    DW_OP_const1s = 0x09,	/* Signed 1-byte constant.  */
    DW_OP_const2u = 0x0a,	/* Unsigned 2-byte constant.  */
    DW_OP_const2s = 0x0b,	/* Signed 2-byte constant.  */
    DW_OP_const4u = 0x0c,	/* Unsigned 4-byte constant.  */
    DW_OP_const4s = 0x0d,	/* Signed 4-byte constant.  */
    DW_OP_const8u = 0x0e,	/* Unsigned 8-byte constant.  */
    DW_OP_const8s = 0x0f,	/* Signed 8-byte constant.  */
    DW_OP_constu = 0x10,	/* Unsigned LEB128 constant.  */
    DW_OP_consts = 0x11,	/* Signed LEB128 constant.  */
    DW_OP_dup = 0x12,
    DW_OP_drop = 0x13,
    DW_OP_over = 0x14,
    DW_OP_pick = 0x15,		/* 1-byte stack index.  */
    DW_OP_swap = 0x16,
    DW_OP_rot = 0x17,
    DW_OP_xderef = 0x18,
    DW_OP_abs = 0x19,
    DW_OP_and = 0x1a,
    DW_OP_div = 0x1b,
    DW_OP_minus = 0x1c,
    DW_OP_mod = 0x1d,
    DW_OP_mul = 0x1e,
    DW_OP_neg = 0x1f,
    DW_OP_not = 0x20,
    DW_OP_or = 0x21,
    DW_OP_plus = 0x22,
    DW_OP_plus_uconst = 0x23,	/* Unsigned LEB128 addend.  */
    DW_OP_shl = 0x24,
    DW_OP_shr = 0x25,
    DW_OP_shra = 0x26,
    DW_OP_xor = 0x27,
    DW_OP_bra = 0x28,		/* Signed 2-byte constant.  */
    DW_OP_eq = 0x29,
    DW_OP_ge = 0x2a,
    DW_OP_gt = 0x2b,
    DW_OP_le = 0x2c,
    DW_OP_lt = 0x2d,
    DW_OP_ne = 0x2e,
    DW_OP_skip = 0x2f,		/* Signed 2-byte constant.  */
    DW_OP_lit0 = 0x30,		/* Literal 0.  */
    DW_OP_lit1 = 0x31,		/* Literal 1.  */
    DW_OP_lit2 = 0x32,		/* Literal 2.  */
    DW_OP_lit3 = 0x33,		/* Literal 3.  */
    DW_OP_lit4 = 0x34,		/* Literal 4.  */
    DW_OP_lit5 = 0x35,		/* Literal 5.  */
    DW_OP_lit6 = 0x36,		/* Literal 6.  */
    DW_OP_lit7 = 0x37,		/* Literal 7.  */
    DW_OP_lit8 = 0x38,		/* Literal 8.  */
    DW_OP_lit9 = 0x39,		/* Literal 9.  */
    DW_OP_lit10 = 0x3a,		/* Literal 10.  */
    DW_OP_lit11 = 0x3b,		/* Literal 11.  */
    DW_OP_lit12 = 0x3c,		/* Literal 12.  */
    DW_OP_lit13 = 0x3d,		/* Literal 13.  */
    DW_OP_lit14 = 0x3e,		/* Literal 14.  */
    DW_OP_lit15 = 0x3f,		/* Literal 15.  */
    DW_OP_lit16 = 0x40,		/* Literal 16.  */
    DW_OP_lit17 = 0x41,		/* Literal 17.  */
    DW_OP_lit18 = 0x42,		/* Literal 18.  */
    DW_OP_lit19 = 0x43,		/* Literal 19.  */
    DW_OP_lit20 = 0x44,		/* Literal 20.  */
    DW_OP_lit21 = 0x45,		/* Literal 21.  */
    DW_OP_lit22 = 0x46,		/* Literal 22.  */
    DW_OP_lit23 = 0x47,		/* Literal 23.  */
    DW_OP_lit24 = 0x48,		/* Literal 24.  */
    DW_OP_lit25 = 0x49,		/* Literal 25.  */
    DW_OP_lit26 = 0x4a,		/* Literal 26.  */
    DW_OP_lit27 = 0x4b,		/* Literal 27.  */
    DW_OP_lit28 = 0x4c,		/* Literal 28.  */
    DW_OP_lit29 = 0x4d,		/* Literal 29.  */
    DW_OP_lit30 = 0x4e,		/* Literal 30.  */
    DW_OP_lit31 = 0x4f,		/* Literal 31.  */
    DW_OP_reg0 = 0x50,		/* Register 0.  */
    DW_OP_reg1 = 0x51,		/* Register 1.  */
    DW_OP_reg2 = 0x52,		/* Register 2.  */
    DW_OP_reg3 = 0x53,		/* Register 3.  */
    DW_OP_reg4 = 0x54,		/* Register 4.  */
    DW_OP_reg5 = 0x55,		/* Register 5.  */
    DW_OP_reg6 = 0x56,		/* Register 6.  */
    DW_OP_reg7 = 0x57,		/* Register 7.  */
    DW_OP_reg8 = 0x58,		/* Register 8.  */
    DW_OP_reg9 = 0x59,		/* Register 9.  */
    DW_OP_reg10 = 0x5a,		/* Register 10.  */
    DW_OP_reg11 = 0x5b,		/* Register 11.  */
    DW_OP_reg12 = 0x5c,		/* Register 12.  */
    DW_OP_reg13 = 0x5d,		/* Register 13.  */
    DW_OP_reg14 = 0x5e,		/* Register 14.  */
    DW_OP_reg15 = 0x5f,		/* Register 15.  */
    DW_OP_reg16 = 0x60,		/* Register 16.  */
    DW_OP_reg17 = 0x61,		/* Register 17.  */
    DW_OP_reg18 = 0x62,		/* Register 18.  */
    DW_OP_reg19 = 0x63,		/* Register 19.  */
    DW_OP_reg20 = 0x64,		/* Register 20.  */
    DW_OP_reg21 = 0x65,		/* Register 21.  */
    DW_OP_reg22 = 0x66,		/* Register 22.  */
    DW_OP_reg23 = 0x67,		/* Register 24.  */
    DW_OP_reg24 = 0x68,		/* Register 24.  */
    DW_OP_reg25 = 0x69,		/* Register 25.  */
    DW_OP_reg26 = 0x6a,		/* Register 26.  */
    DW_OP_reg27 = 0x6b,		/* Register 27.  */
    DW_OP_reg28 = 0x6c,		/* Register 28.  */
    DW_OP_reg29 = 0x6d,		/* Register 29.  */
    DW_OP_reg30 = 0x6e,		/* Register 30.  */
    DW_OP_reg31 = 0x6f,		/* Register 31.  */
    DW_OP_breg0 = 0x70,		/* Base register 0.  */
    DW_OP_breg1 = 0x71,		/* Base register 1.  */
    DW_OP_breg2 = 0x72,		/* Base register 2.  */
    DW_OP_breg3 = 0x73,		/* Base register 3.  */
    DW_OP_breg4 = 0x74,		/* Base register 4.  */
    DW_OP_breg5 = 0x75,		/* Base register 5.  */
    DW_OP_breg6 = 0x76,		/* Base register 6.  */
    DW_OP_breg7 = 0x77,		/* Base register 7.  */
    DW_OP_breg8 = 0x78,		/* Base register 8.  */
    DW_OP_breg9 = 0x79,		/* Base register 9.  */
    DW_OP_breg10 = 0x7a,	/* Base register 10.  */
    DW_OP_breg11 = 0x7b,	/* Base register 11.  */
    DW_OP_breg12 = 0x7c,	/* Base register 12.  */
    DW_OP_breg13 = 0x7d,	/* Base register 13.  */
    DW_OP_breg14 = 0x7e,	/* Base register 14.  */
    DW_OP_breg15 = 0x7f,	/* Base register 15.  */
    DW_OP_breg16 = 0x80,	/* Base register 16.  */
    DW_OP_breg17 = 0x81,	/* Base register 17.  */
    DW_OP_breg18 = 0x82,	/* Base register 18.  */
    DW_OP_breg19 = 0x83,	/* Base register 19.  */
    DW_OP_breg20 = 0x84,	/* Base register 20.  */
    DW_OP_breg21 = 0x85,	/* Base register 21.  */
    DW_OP_breg22 = 0x86,	/* Base register 22.  */
    DW_OP_breg23 = 0x87,	/* Base register 23.  */
    DW_OP_breg24 = 0x88,	/* Base register 24.  */
    DW_OP_breg25 = 0x89,	/* Base register 25.  */
    DW_OP_breg26 = 0x8a,	/* Base register 26.  */
    DW_OP_breg27 = 0x8b,	/* Base register 27.  */
    DW_OP_breg28 = 0x8c,	/* Base register 28.  */
    DW_OP_breg29 = 0x8d,	/* Base register 29.  */
    DW_OP_breg30 = 0x8e,	/* Base register 30.  */
    DW_OP_breg31 = 0x8f,	/* Base register 31.  */
    DW_OP_regx = 0x90,		/* Unsigned LEB128 register.  */
    DW_OP_fbreg = 0x91,		/* Signed LEB128 offset.  */
    DW_OP_bregx = 0x92,		/* ULEB128 register followed by SLEB128 off. */
    DW_OP_piece = 0x93,		/* ULEB128 size of piece addressed. */
    DW_OP_deref_size = 0x94,	/* 1-byte size of data retrieved.  */
    DW_OP_xderef_size = 0x95,	/* 1-byte size of data retrieved.  */
    DW_OP_nop = 0x96,
    DW_OP_push_object_address = 0x97,
    DW_OP_call2 = 0x98,
    DW_OP_call4 = 0x99,
    DW_OP_call_ref = 0x9a,
    DW_OP_form_tls_address = 0x9b,/* TLS offset to address in current thread */
    DW_OP_call_frame_cfa = 0x9c,/* CFA as determined by CFI.  */
    DW_OP_bit_piece = 0x9d,	/* ULEB128 size and ULEB128 offset in bits.  */
    DW_OP_implicit_value = 0x9e, /* DW_FORM_block follows opcode.  */
    DW_OP_stack_value = 0x9f,	 /* No operands, special like DW_OP_piece.  */

    /* GNU extensions.  */
    DW_OP_GNU_push_tls_address = 0xe0,
    DW_OP_GNU_uninit = 0xf0,
    DW_OP_GNU_encoded_addr = 0xf1,
    DW_OP_GNU_implicit_pointer = 0xf2,
    DW_OP_GNU_entry_value = 0xf3,
    DW_OP_GNU_const_type = 0xf4,
    DW_OP_GNU_regval_type = 0xf5,
    DW_OP_GNU_deref_type = 0xf6,
    DW_OP_GNU_convert = 0xf7,
    DW_OP_GNU_reinterpret = 0xf9,
    DW_OP_GNU_parameter_ref = 0xfa,

    DW_OP_lo_user = 0xe0,	/* Implementation-defined range start.  */
    DW_OP_hi_user = 0xff	/* Implementation-defined range end.  */
  };
