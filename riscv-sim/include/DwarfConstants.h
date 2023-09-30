#ifndef DWARFCONSTANTS_H
#define DWARFCONSTANTS_H


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.2: Unit header unit type encodings
enum class DW_UT{
    compile         = 0x01,
    type            = 0x02,
    partial         = 0x03,
    skeleton        = 0x04,
    split_compile   = 0x05,
    split_type      = 0x06,
    lo_user         = 0x80,
    hi_user         = 0xff
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.3: Tag encodings
enum class DW_TAG{
    array_type                  = 0x01,
    class_type                  = 0x02,
    entry_point                 = 0x03,
    enumeration_type            = 0x04,
    formal_parameter            = 0x05,
    imported_declaration        = 0x08,
    label                       = 0x0a,
    lexical_block               = 0x0b,
    member                      = 0x0d,
    pointer_type                = 0x0f,
    reference_type              = 0x10,
    compile_unit                = 0x11,
    string_type                 = 0x12,
    structure_type              = 0x13,
    subroutine_type             = 0x15,
    typedef_                    = 0x16,
    union_type                  = 0x17,
    unspecified_parameters      = 0x18,
    variant                     = 0x19,
    common_block                = 0x1a,
    common_inclusion            = 0x1b,
    inheritance                 = 0x1c,
    inlined_subroutine          = 0x1d,
    module                      = 0x1e,
    ptr_to_member_type          = 0x1f,
    set_type                    = 0x20,
    subrange_type               = 0x21,
    with_stmt                   = 0x22,
    access_declaration          = 0x23,
    base_type                   = 0x24,
    catch_block                 = 0x25,
    const_type                  = 0x26,
    constant                    = 0x27,
    enumerator                  = 0x28,
    file_type                   = 0x29,
    friend_                     = 0x2a,
    namelist                    = 0x2b,
    namelist_item               = 0x2c,
    packed_type                 = 0x2d,
    subprogram                  = 0x2e,
    template_type_parameter     = 0x2f,
    template_value_parameter    = 0x30,
    thrown_type                 = 0x31,
    try_block                   = 0x32,
    variant_part                = 0x33,
    variable                    = 0x34,
    volatile_type               = 0x35,
    dwarf_procedure             = 0x36,
    restrict_type               = 0x37,
    interface_type              = 0x38,
    namespace_                  = 0x39,
    imported_module             = 0x3a,
    unspecified_type            = 0x3b,
    partial_unit                = 0x3c,
    imported_unit               = 0x3d,
    condition                   = 0x3f,
    shared_type                 = 0x40,
    type_unit                   = 0x41,
    rvalue_reference_type       = 0x42,
    template_alias              = 0x43,
    coarray_type                = 0x44,
    generic_subrange            = 0x45,
    dynamic_type                = 0x46,
    atomic_type                 = 0x47,
    call_site                   = 0x48,
    call_site_parameter         = 0x49,
    skeleton_unit               = 0x4a,
    immutable_type              = 0x4b,
    lo_user                     = 0x4080,
    hi_user                     = 0xffff
};

// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.4: Child determination encodings
enum class DW_CHILDREN{
    no = 0x00,
    yes = 0x01
};

// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.5: Attribute encodings
enum class DW_AT{
    sibling                 = 0x01,     // reference
    location                = 0x02,     // exprloc, loclist
    name                    = 0x03,     // string
    ordering                = 0x09,     // constant
    byte_size               = 0x0b,     // constant, exprloc, reference
    bit_size                = 0x0d,     // constant, exprloc, reference
    stmt_list               = 0x10,     // lineptr
    low_pc                  = 0x11,     // address
    high_pc                 = 0x12,     // address, constant
    language                = 0x13,     // constant
    discr                   = 0x15,     // reference
    discr_value             = 0x16,     // constant
    visibility              = 0x17,     // constant
    import                  = 0x18,     // reference
    string_length           = 0x19,     // exprloc, loclist, reference
    common_reference        = 0x1a,     // reference
    comp_dir                = 0x1b,     // string
    const_value             = 0x1c,     // block, constant, string
    containing_type         = 0x1d,     // reference
    default_value           = 0x1e,     // constant, reference, flag
    inline_                 = 0x20,     // constant
    is_optional             = 0x21,     // flag
    lower_bound             = 0x22,     // constant, exprloc, reference
    producer                = 0x25,     // string
    prototyped              = 0x27,     // flag
    return_addr             = 0x2a,     // exprloc, loclist
    start_scope             = 0x2c,     // constant, rnglist
    bit_stride              = 0x2e,     // constant, exprloc, reference
    upper_bound             = 0x2f,     // constant, exprloc, reference
    abstract_origin         = 0x31,     // reference
    accessibility           = 0x32,     // constant
    address_class           = 0x33,     // constant
    artificial              = 0x34,     // flag
    base_types              = 0x35,     // reference
    calling_convention      = 0x36,     // constant
    count                   = 0x37,     // constant, exprloc, reference
    data_member_location    = 0x38,     // constant, exprloc, loclist
    decl_column             = 0x39,     // constant
    decl_file               = 0x3a,     // constant
    decl_line               = 0x3b,     // constant
    declaration             = 0x3c,     // flag
    discr_list              = 0x3d,     // block
    encoding                = 0x3e,     // constant
    external                = 0x3f,     // flag
    frame_base              = 0x40,     // exprloc, loclist
    friend_                 = 0x41,     // reference
    identifier_case         = 0x42,     // constant
    namelist_item           = 0x44,     // reference
    priority                = 0x45,     // reference
    segment                 = 0x46,     // exprloc, loclist
    specification           = 0x47,     // reference
    static_link             = 0x48,     // exprloc, loclist
    type                    = 0x49,     // reference
    use_location            = 0x4a,     // exprloc, loclist
    variable_parameter      = 0x4b,     // flag
    virtuality              = 0x4c,     // constant
    vtable_elem_location    = 0x4d,     // exprloc, loclist
    allocated               = 0x4e,     // constant, exprloc, reference
    associated              = 0x4f,     // constant, exprloc, reference
    data_location           = 0x50,     // exprloc
    byte_stride             = 0x51,     // constant, exprloc, reference
    entry_pc                = 0x52,     // address, constant
    use_UTF8                = 0x53,     // flag
    extension               = 0x54,     // reference
    ranges                  = 0x55,     // rnglist
    trampoline              = 0x56,     // address, flag, reference, string
    call_column             = 0x57,     // constant
    call_file               = 0x58,     // constant
    call_line               = 0x59,     // constant
    description             = 0x5a,     // string
    binary_scale            = 0x5b,     // constant
    decimal_scale           = 0x5c,     // constant
    small                   = 0x5d,     // reference
    decimal_sign            = 0x5e,     // constant
    digit_count             = 0x5f,     // constant
    picture_string          = 0x60,     // string
    mutable_                = 0x61,     // flag
    threads_scaled          = 0x62,     // flag
    explicit_               = 0x63,     // flag
    object_pointer          = 0x64,     // reference
    endianity               = 0x65,     // constant
    elemental               = 0x66,     // flag
    pure                    = 0x67,     // flag
    recursive               = 0x68,     // flag
    signature               = 0x69,     // reference
    main_subprogram         = 0x6a,     // flag
    data_bit_offset         = 0x6b,     // constant
    const_expr              = 0x6c,     // flag
    enum_class              = 0x6d,     // flag
    linkage_name            = 0x6e,     // string
    string_length_bit_size  = 0x6f,     // constant
    string_length_byte_size = 0x70,     // constant
    rank                    = 0x71,     // constant, exprloc
    str_offsets_base        = 0x72,     // stroffsetsptr
    addr_base               = 0x73,     // addrptr
    rnglists_base           = 0x74,     // rnglistsptr
    dwo_name                = 0x76,     // string
    reference               = 0x77,     // flag
    rvalue_reference        = 0x78,     // flag
    macros                  = 0x79,     // macptr
    call_all_calls          = 0x7a,     // flag
    call_all_source_calls   = 0x7b,     // flag
    call_all_tail_calls     = 0x7c,     // flag
    call_return_pc          = 0x7d,     // address
    call_value              = 0x7e,     // exprloc
    call_origin             = 0x7f,     // exprloc
    call_parameter          = 0x80,     // reference
    call_pc                 = 0x81,     // address
    call_tail_call          = 0x82,     // flag
    call_target             = 0x83,     // exprloc
    call_target_clobbered   = 0x84,     // exprloc
    call_data_location      = 0x85,     // exprloc
    call_data_value         = 0x86,     // exprloc
    noreturn                = 0x87,     // flag
    alignment               = 0x88,     // constant
    export_symbols          = 0x89,     // flag
    deleted                 = 0x8a,     // flag
    defaulted               = 0x8b,     // constant
    loclists_base           = 0x8c,     // loclistsptr
    lo_user                 = 0x2000,   // —
    hi_user                 = 0x3fff    // —
};

// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.6: Attribute form encodings
enum class DW_FORM{
    none            = 0x00,
    addr            = 0x01, // address
    block2          = 0x03, // block
    block4          = 0x04, // block
    data2           = 0x05, // constant
    data4           = 0x06, // constant
    data8           = 0x07, // constant
    string          = 0x08, // string
    block           = 0x09, // block
    block1          = 0x0a, // block
    data1           = 0x0b, // constant
    flag            = 0x0c, // flag
    sdata           = 0x0d, // constant
    strp            = 0x0e, // string
    udata           = 0x0f, // constant
    ref_addr        = 0x10, // reference
    ref1            = 0x11, // reference
    ref2            = 0x12, // reference
    ref4            = 0x13, // reference
    ref8            = 0x14, // reference
    ref_udata       = 0x15, // reference
    indirect        = 0x16, // (see Section 7.5.3 on page 203)
    sec_offset      = 0x17, // addrptr, lineptr, loclist, loclistsptr, macptr, rnglist, rnglistsptr, stroffsetsptr
    exprloc         = 0x18, // exprloc
    flag_present    = 0x19, // flag
    strx            = 0x1a, // string
    addrx           = 0x1b, // address
    ref_sup4        = 0x1c, // reference
    strp_sup        = 0x1d, // string
    data16          = 0x1e, // constant
    line_strp       = 0x1f, // string
    ref_sig8        = 0x20, // reference
    implicit_const  = 0x21, // constant
    loclistx        = 0x22, // loclist
    rnglistx        = 0x23, // rnglist
    ref_sup8        = 0x24, // reference
    strx1           = 0x25, // string
    strx2           = 0x26, // string
    strx3           = 0x27, // string
    strx4           = 0x28, // string
    addrx1          = 0x29, // address
    addrx2          = 0x2a, // address
    addrx3          = 0x2b, // address
    addrx4          = 0x2c, // address
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.9: DWARF operation encodings
enum class DW_OP{
    none                = 0x00,
    addr                = 0x03, // constant address (size is target specific)
    deref               = 0x06, //
    const1u             = 0x08, // 1-byte constant
    const1s             = 0x09, // 1-byte constant
    const2u             = 0x0a, // 2-byte constant
    const2s             = 0x0b, // 2-byte constant
    const4u             = 0x0c, // 4-byte constant
    const4s             = 0x0d, // 4-byte constant
    const8u             = 0x0e, // 8-byte constant
    const8s             = 0x0f, // 8-byte constant
    constu              = 0x10, // ULEB128 constant
    consts              = 0x11, // SLEB128 constant
    dup                 = 0x12, //
    drop                = 0x13, //
    over                = 0x14, //
    pick                = 0x15, // 1-byte stack index
    swap                = 0x16, //
    rot                 = 0x17, //
    xderef              = 0x18, //
    abs                 = 0x19, //
    and_                = 0x1a, //
    div                 = 0x1b, //
    minus               = 0x1c, //
    mod                 = 0x1d, //
    mul                 = 0x1e, //
    neg                 = 0x1f, //
    not_                = 0x20, //
    or_                 = 0x21, //
    plus                = 0x22, //
    plus_uconst         = 0x23, // ULEB128 addend
    shl                 = 0x24, //
    shr                 = 0x25, //
    shra                = 0x26, //
    xor_                = 0x27, //
    bra                 = 0x28, // signed 2-byte constant
    eq                  = 0x29, //
    ge                  = 0x2a, //
    gt                  = 0x2b, //
    le                  = 0x2c, //
    lt                  = 0x2d, //
    ne                  = 0x2e, //
    skip                = 0x2f, // signed 2-byte constant
    lit0                = 0x30, // literals 0 .. 31 = (lit0 + literal)
    lit1                = 0x31, // 
    lit2                = 0x32, //
    lit3                = 0x33, //
    lit4                = 0x34, //
    lit5                = 0x35, //
    lit6                = 0x36, //
    lit7                = 0x37, //
    lit8                = 0x38, //
    lit9                = 0x39, //
    lit10               = 0x3a, //
    lit11               = 0x3b, //
    lit12               = 0x3c, //
    lit13               = 0x3d, //
    lit14               = 0x3e, //
    lit15               = 0x3f, //
    lit16               = 0x40, //
    lit17               = 0x41, //
    lit18               = 0x42, //
    lit19               = 0x43, //
    lit20               = 0x44, //
    lit21               = 0x45, //
    lit22               = 0x46, //
    lit23               = 0x47, //
    lit24               = 0x48, //
    lit25               = 0x49, //
    lit26               = 0x4a, //
    lit27               = 0x4b, //
    lit28               = 0x4c, //
    lit29               = 0x4d, //
    lit30               = 0x4e, //
    lit31               = 0x4f, //
    reg0                = 0x50, // reg 0 .. 31 = (reg0 + regnum)
    reg1                = 0x51, // 
    reg2                = 0x52, //
    reg3                = 0x53, //
    reg4                = 0x54, //
    reg5                = 0x55, //
    reg6                = 0x56, //
    reg7                = 0x57, //
    reg8                = 0x58, //
    reg9                = 0x59, //
    reg10               = 0x5a, //
    reg11               = 0x5b, //
    reg12               = 0x5c, //
    reg13               = 0x5d, //
    reg14               = 0x5e, //
    reg15               = 0x5f, //
    reg16               = 0x60, //
    reg17               = 0x61, //
    reg18               = 0x62, //
    reg19               = 0x63, //
    reg20               = 0x64, //
    reg21               = 0x65, //
    reg22               = 0x66, //
    reg23               = 0x67, //
    reg24               = 0x68, //
    reg25               = 0x69, //
    reg26               = 0x6a, //
    reg27               = 0x6b, //
    reg28               = 0x6c, //
    reg29               = 0x6d, //
    reg30               = 0x6e, //
    reg31               = 0x6f, //
    breg0               = 0x70, // SLEB128 offset base register 0 .. 31 = (breg0 + regnum)
    breg1               = 0x71, // 
    breg2               = 0x72, //
    breg3               = 0x73, //
    breg4               = 0x74, //
    breg5               = 0x75, //
    breg6               = 0x76, //
    breg7               = 0x77, //
    breg8               = 0x78, //
    breg9               = 0x79, //
    breg10              = 0x7a, //
    breg11              = 0x7b, //
    breg12              = 0x7c, //
    breg13              = 0x7d, //
    breg14              = 0x7e, //
    breg15              = 0x7f, //
    breg16              = 0x80, //
    breg17              = 0x81, //
    breg18              = 0x82, //
    breg19              = 0x83, //
    breg20              = 0x84, //
    breg21              = 0x85, //
    breg22              = 0x86, //
    breg23              = 0x87, //
    breg24              = 0x88, //
    breg25              = 0x89, //
    breg26              = 0x8a, //
    breg27              = 0x8b, //
    breg28              = 0x8c, //
    breg29              = 0x8d, //
    breg30              = 0x8e, //
    breg31              = 0x8f, // 
    regx                = 0x90, // ULEB128 register
    fbreg               = 0x91, // SLEB128 offset
    bregx               = 0x92, // ULEB128 register, SLEB128 offset
    piece               = 0x93, // ULEB128 size of piece
    deref_size          = 0x94, // 1-byte size of data retrieved
    xderef_size         = 0x95, // 1-byte size of data retrieved
    nop                 = 0x96, //
    push_object_address = 0x97, //
    call2               = 0x98, // 2-byte offset of DIE
    call4               = 0x99, // 4-byte offset of DIE
    call_ref            = 0x9a, // 4- or 8-byte offset of DIE
    form_tls_address    = 0x9b, //
    call_frame_cfa      = 0x9c, //
    bit_piece           = 0x9d, // ULEB128 size, ULEB128 offset
    implicit_value      = 0x9e, // ULEB128 size, block of that size
    stack_value         = 0x9f, //
    implicit_pointer    = 0xa0, // 4- or 8-byte offset of DIE, SLEB128 constant offset
    addrx               = 0xa1, // ULEB128 indirect address
    constx              = 0xa2, // ULEB128 indirect constant
    entry_value         = 0xa3, // ULEB128 size, block of that size
    const_type          = 0xa4, // ULEB128 type entry offset, 1-byte size, constant value
    regval_type         = 0xa5, // ULEB128 register number, ULEB128 constant offset
    deref_type          = 0xa6, // 1-byte size, ULEB128 type entry offset
    xderef_type         = 0xa7, // 1-byte size, ULEB128 type entry offset
    convert             = 0xa8, // ULEB128 type entry offset
    reinterpret         = 0xa9, // ULEB128 type entry offset
    lo_user             = 0xe0, //
    hi_user             = 0xff  //
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.10: Location list entry encoding values
enum class DW_LLE{
    end_of_list         = 0x00,
    base_addressx       = 0x01,
    startx_endx         = 0x02,
    startx_length       = 0x03,
    offset_pair         = 0x04,
    default_location    = 0x05,
    base_address        = 0x06,
    start_end           = 0x07,
    start_length        = 0x08
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.11: Base type encoding values
enum class DW_ATE{
    none            = 0x00,
    address         = 0x01,
    boolean         = 0x02,
    complex_float   = 0x03,
    float_          = 0x04,
    signed_         = 0x05,
    signed_char     = 0x06,
    unsigned_       = 0x07,
    unsigned_char   = 0x08,
    imaginary_float = 0x09,
    packed_decimal  = 0x0a,
    numeric_string  = 0x0b,
    edited          = 0x0c,
    signed_fixed    = 0x0d,
    unsigned_fixed  = 0x0e,
    decimal_float   = 0x0f,
    UTF             = 0x10,
    UCS             = 0x11,
    ASCII           = 0x12,
    lo_user         = 0x80,
    hi_user         = 0xff
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.12: Decimal sign encodings
enum class DW_DS{
    unsigned_           = 0x01,
    leading_overpunch   = 0x02,
    trailing_overpunch  = 0x03,
    leading_separate    = 0x04,
    trailing_separate   = 0x05
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.13: Endianity encodings
enum class DW_END{
    default_    = 0x00,
    big         = 0x01,
    little      = 0x02,
    lo_user     = 0x40,
    hi_user     = 0xff
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.14: Accessibility encodings
enum class DW_ACCESS{
    public_     = 0x01,
    protected_  = 0x02,
    private_    = 0x03
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.15: Visibility encodings
enum class DW_VIS{
    local       = 0x01,
    exported    = 0x02,
    qualified   = 0x03
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// TTable 7.16: Virtuality encodings
enum class DW_VIRTUALITY{
    none            = 0x00,
    virtual_        = 0x01,
    pure_virtual    = 0x02
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.17: Language encodings
enum class DW_LANG{
    C89             = 0x0001,
    C               = 0x0002,
    Ada83           = 0x0003,
    C_plus_plus     = 0x0004,
    Cobol74         = 0x0005,
    Cobol85         = 0x0006,
    Fortran77       = 0x0007,
    Fortran90       = 0x0008,
    Pascal83        = 0x0009,
    Modula2         = 0x000a,
    Java            = 0x000b,
    C99             = 0x000c,
    Ada95           = 0x000d,
    Fortran95       = 0x000e,
    PLI             = 0x000f,
    ObjC            = 0x0010,
    ObjC_plus_plus  = 0x0011,
    UPC             = 0x0012,
    D               = 0x0013,
    Python          = 0x0014,
    OpenCL          = 0x0015,
    Go              = 0x0016,
    Modula3         = 0x0017,
    Haskell         = 0x0018,
    C_plus_plus_03  = 0x0019,
    C_plus_plus_11  = 0x001a,
    OCaml           = 0x001b,
    Rust            = 0x001c,
    C11             = 0x001d,
    Swift           = 0x001e,
    Julia           = 0x001f,
    Dylan           = 0x0020,
    C_plus_plus_14  = 0x0021,
    Fortran03       = 0x0022,
    Fortran08       = 0x0023,
    RenderScript    = 0x0024,
    BLISS           = 0x0025,
    lo_user         = 0x8000,
    hi_user         = 0xffff
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.18: Identifier case encodings
enum class DW_ID{
    case_sensitive      = 0x00,
    up_case             = 0x01,
    down_case           = 0x02,
    case_insensitive    = 0x03
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.19: Calling convention encodings
enum class DW_CC{
    normal              = 0x01,
    program             = 0x02,
    nocall              = 0x03,
    pass_by_reference   = 0x04,
    pass_by_value       = 0x05,
    lo_user             = 0x40,
    hi_user             = 0xff
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.20: Inline encodings
enum class DW_INL{
    not_inlined             = 0x00,
    inlined                 = 0x01,
    declared_not_inlined    = 0x02,
    declared_inlined        = 0x03
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.21: Ordering encodings
enum class DW_ORD{
    row_major   = 0x00,
    col_major   = 0x01
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.22: Discriminant descriptor encodings
enum class DW_DSC{
    label   = 0x00,
    range   = 0x01
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.23: Name index attribute encodings
enum class DW_IDX{
    compile_unit    = 0x01, // constant
    type_unit       = 0x2, //  constant
    die_offset      = 0x3, //  reference
    parent          = 0x4, //  constant
    type_hash       = 0x5, //  DW_FORM_data8
    lo_user         = 0x2000,
    hi_user         = 0x3fff
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.24: Defaulted attribute encodings    
enum class DW_DEFAULTED{
    no              = 0x00,
    in_class        = 0x01,
    out_of_class    = 0x02
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.25: Line number standard opcode encodings   
enum class DW_LNS{
    copy                = 0x01,
    advance_pc          = 0x02,
    advance_line        = 0x03,
    set_file            = 0x04,
    set_column          = 0x05,
    negate_stmt         = 0x06,
    set_basic_block     = 0x07,
    const_add_pc        = 0x08,
    fixed_advance_pc    = 0x09,
    set_prologue_end    = 0x0a,
    set_epilogue_begin  = 0x0b,
    set_isa             = 0x0c
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.26: Line number extended opcode encodings
enum class DW_LNE{
    end_sequence        = 0x01,
    set_address         = 0x02,
    set_discriminator   = 0x04,
    lo_user             = 0x80,
    hi_user             = 0xff
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.27: Line number header entry format encodings
enum class DW_LNCT{
    path            = 0x1,
    directory_index = 0x2,
    timestamp       = 0x3,
    size            = 0x4,
    MD5             = 0x5,
    lo_user         = 0x2000,
    hi_user         = 0x3fff
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.28: Macro information entry type encodings
enum class DW_MACRO{
    define      = 0x01,
    undef       = 0x02,
    start_file  = 0x03,
    end_file    = 0x04,
    define_strp = 0x05,
    undef_strp  = 0x06,
    import      = 0x07,
    define_sup  = 0x08,
    undef_sup   = 0x09,
    import_sup  = 0x0a,
    define_strx = 0x0b,
    undef_strx  = 0x0c,
    lo_user     = 0xe0,
    hi_user     = 0xff
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.29: Call frame instruction encodings
enum class DW_CFA{
    nop                 = 0x00, //
    set_loc             = 0x01, // address
    advance_loc1        = 0x02, // 1-byte delta
    advance_loc2        = 0x03, // 2-byte delta
    advance_loc4        = 0x04, // 4-byte delta
    offset_extended     = 0x05, // ULEB128 register ULEB128 offset
    restore_extended    = 0x06, // ULEB128 register
    undefined           = 0x07, // ULEB128 register
    same_value          = 0x08, // ULEB128 register
    register_           = 0x09, // ULEB128 register ULEB128 offset
    remember_state      = 0x0a, //
    restore_state       = 0x0b, //
    def_cfa             = 0x0c, // ULEB128 register ULEB128 offset
    def_cfa_register    = 0x0d, // ULEB128 register
    def_cfa_offset      = 0x0e, // ULEB128 offset
    def_cfa_expression  = 0x0f, // BLOCK
    expression          = 0x10, // ULEB128 register BLOCK
    offset_extended_sf  = 0x11, // ULEB128 register SLEB128 offset
    def_cfa_sf          = 0x12, // ULEB128 register SLEB128 offset
    def_cfa_offset_sf   = 0x13, // SLEB128 offset
    val_offset          = 0x14, // ULEB128 ULEB128
    val_offset_sf       = 0x15, // ULEB128 SLEB128
    val_expression      = 0x16, // ULEB128 BLOCK
    lo_user             = 0x1c, //
    hi_user             = 0x3f, //
    advance_loc_delta0  = 0x40, //
    offset_reg0         = 0x80, // register ULEB128 offset
    restore_reg0        = 0xC0  // register
};


// DWARF Debugging Information Format Version 5  
// https://dwarfstd.org/doc/DWARF5.pdf
// Table 7.30: Range list entry encoding values
enum class DW_RLE{
    end_of_list     = 0x00,
    base_addressx   = 0x01,
    startx_endx     = 0x02,
    startx_length   = 0x03,
    offset_pair     = 0x04,
    base_address    = 0x05,
    start_end       = 0x06,
    start_length    = 0x07
};

#endif
