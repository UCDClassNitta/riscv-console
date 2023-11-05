#ifndef DWARFSTRUCTURES_H
#define DWARFSTRUCTURES_H

#include <cstdint>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
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

        struct SObjectLocation{
            DW_OP DType;
            union{
                uint64_t DAddress;
                int64_t DOffset;
            };
        };

        struct SValue{
            std::vector< uint8_t > DData;
            DW_FORM DForm;
            
            void SetDataFromString(const std::string &str);
            bool IsStringForm() const;
            std::string GetString() const;
            int64_t GetINT64() const;
            uint64_t GetUINT64() const;
            SObjectLocation GetLocation(uint8_t addrsize, bool littleendian) const;
        };

        struct SCompilationUnit;

        struct SDie{
            uint32_t DAddress;
            std::weak_ptr< SCompilationUnit > DCompilationUnit;
            DW_TAG DTag;
            std::shared_ptr< SAbbreviationEntry > DAttributeEntry;
            std::unordered_map<DW_AT, SValue> DAttributes;
            std::weak_ptr< SDie > DParent;
            std::vector< std::shared_ptr< SDie > > DChildren;
            static bool IsDataType(DW_TAG tag);
            bool HasAttribute(DW_AT att){
                return DAttributes.find(att) != DAttributes.end();
            };
            SValue GetAttribute(DW_AT att){
                return DAttributes[att];
            };
            bool ProcessDataType();
            bool ProcessVariable();
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

        struct SDataType{
            enum class EQualifiers {Volatile, Const, Pointer, Array, Struct, Union, Enum};
            std::string DName;
            std::string DAlias;
            size_t DByteSize;
            size_t DBitSize;    // Will be 8 * byte size unless bit field
            size_t DBitOffset;  // Will be 0 for all non bit fields
            DW_ATE DBaseEncoding;
            SValue DConstant;   // Used for enums, etc.
            std::shared_ptr< SDataType > DReferencedType;
            std::vector< std::shared_ptr< SDataType > >  DChildren;
            std::unordered_set< EQualifiers > DQualifiers;
            bool IsStruct() const;
            bool IsUnion() const;
        };

        struct SPCRange{
            uint64_t DLowPC;
            uint64_t DHighPC;
        };

        struct SVariable{
            std::string DName;
            SObjectLocation DLocation;
            SPCRange DValidRange;
            std::shared_ptr< SDataType > DType;
        };

        struct SProgrammaticScope : public std::enable_shared_from_this< SProgrammaticScope >{
            std::string DName;
            SPCRange DPCRange;
            std::weak_ptr< SProgrammaticScope > DParent;
            std::vector< std::shared_ptr< SVariable > > DVariables;
            std::map< uint64_t, std::shared_ptr< SProgrammaticScope > > DSubScopes;
            bool AddVariableToScope(std::shared_ptr< SVariable > variable);
            bool GetScopesFromPC(uint64_t pc, std::vector< std::shared_ptr< SProgrammaticScope > > &scopes) const;
            bool AddCompilationUnitName(SPCRange range, const std::string &name);
            bool AddFunctionName(SPCRange range, const std::string &name);
            bool Print(int indent, bool recurse) const;
        };

        struct SProgram : public std::enable_shared_from_this< SProgram >{
            bool D32Bit;
            bool DLittleEndian;
            std::shared_ptr< CElfStructures::CStringTable > DDebugStrings;
            std::shared_ptr< CElfStructures::CStringTable > DDebugLineStrings;
            std::shared_ptr< CSeekableDataSourceConverter > DAbbreviationsSourceConverter;
            uint64_t DAbbreviationsSize;
            std::shared_ptr< CSeekableDataSourceConverter > DLineNumberSourceConverter;
            uint64_t DLineNumberSize;
            std::vector< std::shared_ptr< SCompilationUnit > > DCompilaitonUnits;
            SLineNumberData DLineNumberData;
            std::shared_ptr< SProgrammaticScope > DGlobalScope;
            std::shared_ptr< SDie > GetDIEByAddress(uint32_t addr);
            std::shared_ptr< SDataType > GetDataTypeByAddress(uint32_t addr);
            uint64_t ReadCompilationUnit(std::shared_ptr<CSeekableDataSource> source);
            bool ReadAbbreviationTable(std::shared_ptr< SCompilationUnit > cu);
            bool ReadLineNumbers(std::shared_ptr< SCompilationUnit > cu);
            bool ComputeLineNumberStep(CDwarfStructures::SLineNumberStateMachine &machine, std::shared_ptr< CSeekableDataSourceConverter > source);
            bool ConsolidateLineNumbers();
            bool ConsolidateVariables(const std::unordered_map<std::string, uint64_t> &globalsymbols);
        };

        struct SCompilationUnit : public std::enable_shared_from_this< SCompilationUnit >{
            std::weak_ptr< SProgram > DProgram;
            bool D32Bit;
            bool DLittleEndian;
            uint32_t DOffset;
            std::shared_ptr< CElfStructures::CStringTable > DDebugStrings;
            std::shared_ptr< CElfStructures::CStringTable > DDebugLineStrings;
            uint64_t DLength;
            uint16_t DVersion;
            DW_UT DType;
            uint8_t DAddressSize;
            uint64_t DAbbreviationOffset;
            std::unordered_map< uint64_t, std::shared_ptr< SAbbreviationEntry > > DAbbreviationTable;
            std::shared_ptr< SDie > DRoot;
            std::unordered_map< uint32_t, std::shared_ptr< SDie > > DDIEsByAddress;
            std::unordered_map< uint32_t, std::shared_ptr< SDataType > > DDataTypesByAddress;
            std::unordered_map< uint32_t, std::shared_ptr< SVariable > > DVariablesByAddress;
            std::unordered_map< std::string, std::shared_ptr< SDataType > > DDefinedStructsByName;
            std::unordered_map< std::string, std::shared_ptr< SDataType > > DDefinedUnionsByName;
            std::vector< std::shared_ptr< SDataType > > DDeclaredDataTypes;
            std::vector< std::shared_ptr< SVariable > > DExternVariables;
            SLineNumberData DLineNumberData;
            SValue ReadValue(std::shared_ptr< CSeekableDataSourceConverter > source, DW_FORM form, int64_t implicit);
            std::shared_ptr< SDie > GetDIEByAddress(uint32_t addr, bool deepsearch = true);
            std::shared_ptr< SDataType > GetDataTypeByAddress(uint32_t addr, bool deepsearch = true);
            bool ReadDebugInformationEntry(std::shared_ptr< CDwarfStructures::SDie > &die, std::shared_ptr< CSeekableDataSourceConverter > source);
            bool ResolvePaths(std::vector< std::unordered_map<DW_LNCT,CDwarfStructures::SValue> > &directories, std::vector< std::unordered_map<DW_LNCT,CDwarfStructures::SValue> > &files);
            bool ProcessDataTypes();
            bool ProcessVariables();
        };
};

#endif
