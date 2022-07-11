#ifndef ELFLOAD_H
#define ELFLOAD_H

#include "DataSource.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <utility>
#include "DwarfConstants.h"

class CElfLoad{
    public:
        struct SProgramHeader{
            uint32_t DSegmentType;
            uint32_t DFlags;
            uint64_t DFileOffset;
            uint64_t DVirtualAddress;
            uint64_t DPhysicalAddress;
            uint64_t DFileSize;
            uint64_t DMemorySize;
            uint64_t DAlignment;
            std::vector<uint8_t> DPayload;
            std::unordered_map<uint64_t,std::string> DSymbols;
            void Print();
        };

        struct SSectionHeader{
            uint32_t DNameIndex;
            uint32_t DType;
            uint64_t DFlags;
            uint64_t DVirtualAddress;
            uint64_t DFileOffset;
            uint64_t DSize;
            uint32_t DLink;
            uint64_t DInfo;
            uint64_t DAddressAlign;
            uint64_t DEntrySize;
            std::unordered_map<uint64_t,std::string> DSymbols;
            void Print(const std::unordered_map< uint32_t, std::string > &names);
        };

        struct SSymbolEntity{
            uint32_t DNameIndex;
            uint8_t DInfo;
            uint8_t DOther;
            uint16_t DSectionIndex;
            uint64_t DAddress;
            uint64_t DSize;
            uint32_t DNameSectionIndex;
            void Print(const std::unordered_map< uint32_t,  std::vector< char > > &stringtables);
        };

        struct SAttributeSpecification{
            DW_AT DName;
            DW_FORM DForm;
            int64_t DExtra;
        };

        struct SAbbreviationEntry{
            uint64_t DCode;
            DW_TAG DTag;
            bool DChildren;
            std::vector< SAttributeSpecification > DAttributes;
        };

        struct SValue{
            std::vector< uint8_t > DData;
            DW_FORM DForm;
            
            std::string GetString();
            int64_t GetINT64();
            uint64_t GetUINT64();
        };

        struct SCompilationUnit;

        struct SDie{
            std::weak_ptr< SCompilationUnit > DCompilationUnit;
            DW_TAG DTag;
            std::shared_ptr< SAbbreviationEntry > DAttributeEntry;
            std::unordered_map<DW_AT, SValue> DAttributes;
            std::vector< std::shared_ptr< SDie > > DChildren;
            bool HasAttribute(DW_AT att){
                return DAttributes.find(att) != DAttributes.end();
            };
            SValue GetAttribute(DW_AT att){
                return DAttributes[att];
            };
        };

        struct SLineNumberEntry{
            uint32_t DFileIndex;
            uint32_t DLineNumber;
            uint32_t DColumn;
            uint32_t DAddress;
        };

        struct SLineNumberData{
            std::vector< std::string > DFilePaths;
            std::vector<SLineNumberEntry> DLineNumberEntries;
        };

        struct SLineNumberStateMachine{
            struct{
                uint64_t address;
                uint32_t op_index;
                uint32_t file;
                uint32_t line;
                uint32_t column;
                bool is_stmt;
                bool basic_block;
                bool end_sequence;
                bool prologue_end;
                bool epilogue_begin;
                uint32_t isa;
                uint32_t discriminator;
            } DRegs;
            uint8_t DAddressSize;
            uint8_t DSegmentSelectorSize;
            uint8_t DMinimumInstructionLength;
            uint8_t DMaximumOperandsPerInstruction;
            bool DDefaultIsStmt;
            int8_t DLineBase;
            uint8_t DLineRange;
            uint8_t DOpcodeBase;
            std::vector<uint8_t> DOpcodeLengths;
            void Reset();
        };

        struct SCompilationUnit{
            uint64_t DOffset;
            uint64_t DLength;
            uint16_t DVersion;
            DW_UT DType;
            uint8_t DAddressSize;
            uint64_t DAbbreviationOffset;
            std::unordered_map< uint64_t, std::shared_ptr< SAbbreviationEntry > > DAbbreviationTable;
            std::shared_ptr< SDie > DRoot;
            SLineNumberData DLineNumberData;
        };

    protected:
        std::shared_ptr< CDataSource > DInputSource;
        std::vector< uint8_t > DBufferedData;
        uint32_t DBufferedPosition;
        bool DLittleEndian;
        bool D32Bit;
        uint8_t DOSABI;
        uint16_t DObjectType;
        uint16_t DMachine;
        uint32_t DELFVersion;
        uint64_t DEntry;
        uint64_t DProgramHeaderOffset;
        uint64_t DSectionHeaderOffset;
        uint32_t DFlags;
        uint16_t DHeaderSize;
        uint16_t DProgramHeaderEntrySize;
        uint16_t DProgramHeaderEntryCount;
        uint16_t DSectionHeaderEntrySize;
        uint16_t DSectionHeaderEntryCount;
        uint16_t DSectionNameIndex;
        uint64_t DAbbreviationsOffset;
        uint64_t DAbbreviationsSize;
        uint64_t DLineNumberOffset;
        uint64_t DLineNumberSize;

        std::vector< SProgramHeader > DProgramHeaders;
        std::vector< SSectionHeader > DSectionHeaders;
        std::unordered_map< uint32_t, std::string > DSectionNames;
        std::unordered_map< uint32_t,  std::vector< char > > DStringTables;
        std::vector< SSymbolEntity > DSymbolEntities;
        std::vector< char > DDebugStrings;
        std::vector< char > DDebugLineStrings;
        std::vector< std::shared_ptr< SCompilationUnit > > DCompilaitonUnits;
        SLineNumberData DLineNumberData;
        bool DValidFile;

        bool ValidateHeader();
        void PrintHeader();

        void ReadData(uint8_t *buf, size_t size);
        void Seek(uint32_t pos);
        uint32_t CurrentPosition();
        uint8_t ReadUINT8();
        uint16_t ReadUINT16();
        uint32_t ReadUINT32();
        uint64_t ReadUINT64();
        uint64_t ReadULEB128();
        int64_t ReadSLEB128();
        std::string ReadFormString(DW_FORM form);

        bool ReadProgramHeaders();
        bool ReadSectionHeaders();
        bool ReadSectionNames();
        bool ReadStringTables();
        bool ReadSymbolTables();
        bool ReadDebugInfo();
        bool ReadDebugAbbreviationTable(uint64_t offset, std::unordered_map< uint64_t, std::shared_ptr< SAbbreviationEntry > > &abbrev);
        bool ReadDebugInformationEntry(std::shared_ptr< SDie > &die);
        bool ReadDebugLineNumbers(uint64_t offset, std::shared_ptr<SCompilationUnit> cu);
        bool ComputeLineNumberStep(SLineNumberStateMachine &machine);
        bool ConsolidateLineNumbers();
        bool ResolvePaths(std::shared_ptr<SCompilationUnit> cu, std::vector< std::unordered_map<DW_LNCT,SValue> > &directories, std::vector< std::unordered_map<DW_LNCT,SValue> > &files);
        uint64_t ReadDebugCompilationUnit(uint64_t offset);
        SValue ReadDebugValue(DW_FORM form, std::shared_ptr<SCompilationUnit> cu, int64_t implicit);
        bool MergeSymbols();
        bool LoadProgramPayloads();
        bool PrintDebugInformationEntry(std::shared_ptr< SDie > die, int indent);
        bool PrintValue(const SValue &value);
    public:
        CElfLoad(std::shared_ptr< CDataSource > src);
        ~CElfLoad();

        bool ValidFile() const{
            return DValidFile;
        };

        uint64_t Entry() const{
            return DEntry;
        };

        size_t ProgramHeaderCount() const;
        const SProgramHeader &ProgramHeader(size_t index) const;
        size_t SectionHeaderCount() const;
        const SSectionHeader &SectionHeader(size_t index) const;
        const SLineNumberData &LineNumberData() const{
            return DLineNumberData;
        };
        void PrintHeaders();



};

#endif
