#ifndef DWARFSTRUCTURES_H
#define DWARFSTRUCTURES_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "ElfStructures.h"
#include "DwarfConstants.h"
#include "SeekableDataSourceConverter.h"

class CDwarfStructures{
    public:

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
            
            void SetDataFromString(const std::string &str);
            std::string GetString() const;
            int64_t GetINT64() const;
            uint64_t GetUINT64() const;
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

        struct SProgram{
            bool D32Bit;
            std::shared_ptr< CElfStructures::CStringTable > DDebugStrings;
            std::shared_ptr< CElfStructures::CStringTable > DDebugLineStrings;
            std::shared_ptr< CSeekableDataSourceConverter > DAbbreviationsSourceConverter;
            uint64_t DAbbreviationsSize;
            std::shared_ptr< CSeekableDataSourceConverter > DLineNumberSourceConverter;
            uint64_t DLineNumberSize;
            std::vector< std::shared_ptr< SCompilationUnit > > DCompilaitonUnits;
            SLineNumberData DLineNumberData;
            uint64_t ReadCompilationUnit(std::shared_ptr<CSeekableDataSource> source);
            bool ReadAbbreviationTable(std::shared_ptr< SCompilationUnit > cu);
            bool ReadLineNumbers(std::shared_ptr< SCompilationUnit > cu);
            bool ComputeLineNumberStep(CDwarfStructures::SLineNumberStateMachine &machine, std::shared_ptr< CSeekableDataSourceConverter > source);
            bool ConsolidateLineNumbers();
        };

        struct SCompilationUnit : public std::enable_shared_from_this< SCompilationUnit >{
            bool D32Bit;
            std::shared_ptr< CElfStructures::CStringTable > DDebugStrings;
            std::shared_ptr< CElfStructures::CStringTable > DDebugLineStrings;
            uint64_t DLength;
            uint16_t DVersion;
            DW_UT DType;
            uint8_t DAddressSize;
            uint64_t DAbbreviationOffset;
            std::unordered_map< uint64_t, std::shared_ptr< SAbbreviationEntry > > DAbbreviationTable;
            std::shared_ptr< SDie > DRoot;
            SLineNumberData DLineNumberData;
            SValue ReadValue(std::shared_ptr< CSeekableDataSourceConverter > source, DW_FORM form, int64_t implicit);
            bool ReadDebugInformationEntry(std::shared_ptr< CDwarfStructures::SDie > &die, std::shared_ptr< CSeekableDataSourceConverter > source);
            bool ResolvePaths(std::vector< std::unordered_map<DW_LNCT,CDwarfStructures::SValue> > &directories, std::vector< std::unordered_map<DW_LNCT,CDwarfStructures::SValue> > &files);
        };
};

#endif
