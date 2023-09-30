#ifndef ELFCONSTANTS_H
#define ELFCONSTANTS_H

#include <cstdint>


class CElfConstants{
    public:
        // Program Header Flags
        static const uint32_t PF_X = 0x04; // Executable
        static const uint32_t PF_W = 0x01; // Writable
        static const uint32_t PF_R = 0x02; // Readable


        enum EProgramHeaderType : uint32_t{
            PT_NULL     = 0x00000000, // Program header table entry unused.
            PT_LOAD     = 0x00000001, // Loadable segment.
            PT_DYNAMIC  = 0x00000002, // Dynamic linking information.
            PT_INTERP   = 0x00000003, // Interpreter information.
            PT_NOTE     = 0x00000004, // Auxiliary information.
            PT_SHLIB    = 0x00000005, // Reserved.
            PT_PHDR     = 0x00000006, // Segment containing program header table itself.
            PT_TLS      = 0x00000007, // Thread-Local Storage template.
            PT_LOOS     = 0x60000000, // Reserved inclusive range. Operating system specific.
            PT_HIOS     = 0x6FFFFFFF, // 
            PT_LOPROC   = 0x70000000, // Reserved inclusive range. Processor specific.
            PT_HIPROC   = 0x7FFFFFFF  // 
        };

        enum ESectionHeaderType : uint32_t{
            SHT_NULL            = 0x00000000, // Section header table entry unused
            SHT_PROGBITS        = 0x00000001, // Program data
            SHT_SYMTAB          = 0x00000002, // Symbol table
            SHT_STRTAB          = 0x00000003, // String table
            SHT_RELA            = 0x00000004, // Relocation entries with addends
            SHT_HASH            = 0x00000005, // Symbol hash table
            SHT_DYNAMIC         = 0x00000006, // Dynamic linking information
            SHT_NOTE            = 0x00000007, // Notes
            SHT_NOBITS          = 0x00000008, // Program space with no data (bss)
            SHT_REL             = 0x00000009, // Relocation entries, no addends
            SHT_SHLIB           = 0x0000000A, // Reserved
            SHT_DYNSYM          = 0x0000000B, // Dynamic linker symbol table
            SHT_INIT_ARRAY      = 0x0000000E, // Array of constructors
            SHT_FINI_ARRAY      = 0x0000000F, // Array of destructors
            SHT_PREINIT_ARRAY   = 0x00000010, // Array of pre-constructors
            SHT_GROUP           = 0x00000011, // Section group
            SHT_SYMTAB_SHNDX    = 0x00000012, // Extended section indices
            SHT_NUM             = 0x00000013, // Number of defined types.
            SHT_LOOS            = 0x60000000    //    Start OS-specific.
        };

        // Symbol bindings
        static const uint8_t STB_LOCAL  = 0x00; // Local
        static const uint8_t STB_GLOBAL = 0x01; // Global
        static const uint8_t STB_WEAK	= 0x02; // Weak global symbol

        // Symbol type
        static const uint8_t STT_NOTYPE    = 0x00; // Type not specified
        static const uint8_t STT_OBJECT    = 0x01; // Data object
        static const uint8_t STT_FUNC	    = 0x02; // Function or executable code
        static const uint8_t STT_SECTION   = 0x03; // Section
        static const uint8_t STT_FILE      = 0x04; // File 
        static const uint8_t STT_COMMON    = 0x05; // Common block
        static const uint8_t STT_TLS       = 0x06; // Thread local storage

        static constexpr uint8_t SYMBOL_INFO(uint8_t binding, uint8_t type){ return (binding<<4) | (type & 0x0F);};

};



#endif