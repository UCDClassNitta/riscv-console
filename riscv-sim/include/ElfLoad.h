#ifndef ELFLOAD_H
#define ELFLOAD_H

#include "DataSource.h"
#include "SeekableDataSourceConverter.h"
#include "BufferedSeekableDataSource.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "ElfStructures.h"
#include "DwarfStructures.h"

class CElfLoad{
    public:

    protected:
        std::shared_ptr< CSeekableDataSource > DInputSource;
        std::shared_ptr< CSeekableDataSourceConverter > DInputSourceConverter;
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

        std::vector< CElfStructures::SProgramHeader > DProgramHeaders;
        std::vector< CElfStructures::SSectionHeader > DSectionHeaders;
        CElfStructures::CStringTable DSectionNames;
        std::unordered_set< uint16_t > DGlobalVariableSectionIndices;
        std::unordered_map< uint32_t,  CElfStructures::CStringTable > DStringTables;
        std::vector< CElfStructures::SSymbolEntity > DSymbolEntities;
        std::shared_ptr< CDwarfStructures::SProgram > DDwarfProgram;
        bool DValidFile;

        bool ValidateHeader();
        void PrintHeader();
        void PrintProgramHeader(const CElfStructures::SProgramHeader &header);
        void PrintSectionHeader(const CElfStructures::SSectionHeader &header);
        void PrintSymbolEntity(CElfStructures::SSymbolEntity &entity);

        bool ReadProgramHeaders();
        bool ReadSectionHeaders();
        bool ReadSectionNames();
        bool ReadStringTables();
        bool ReadSymbolTables();
        bool ReadDebugInfo();
        bool MergeSymbols();
        bool LoadProgramPayloads();
        bool PrintDebugInformationEntry(std::shared_ptr< CDwarfStructures::SDie > die, int indent);
        bool PrintValue(const CDwarfStructures::SValue &value);
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
        const CElfStructures::SProgramHeader &ProgramHeader(size_t index) const;
        size_t SectionHeaderCount() const;
        const CElfStructures::SSectionHeader &SectionHeader(size_t index) const;
        const CDwarfStructures::SLineNumberData &LineNumberData() const{
            return DDwarfProgram->DLineNumberData;
        };
        std::shared_ptr< CDwarfStructures::SProgrammaticScope > GetGlobalScopes() const{
            return DDwarfProgram->DGlobalScope;
        };

        void PrintHeaders();



};

#endif
