#ifndef ELFLOAD_H
#define ELFLOAD_H

#include "DataSource.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <utility>

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

        std::vector< SProgramHeader > DProgramHeaders;
        std::vector< SSectionHeader > DSectionHeaders;
        std::unordered_map< uint32_t, std::string > DSectionNames;
        std::unordered_map< uint32_t,  std::vector< char > > DStringTables;
        std::vector< SSymbolEntity > DSymbolEntities;
        bool DValidFile;

        bool ValidateHeader();
        void PrintHeader();

        void ReadData(uint8_t *buf, size_t size);
        void Seek(uint32_t pos);
        uint8_t ReadUINT8();
        uint16_t ReadUINT16();
        uint32_t ReadUINT32();
        uint64_t ReadUINT64();

        bool ReadProgramHeaders();
        bool ReadSectionHeaders();
        bool ReadSectionNames();
        bool ReadStringTables();
        bool ReadSymbolTables();
        bool MergeSymbols();
        bool LoadProgramPayloads();

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
        void PrintHeaders();



};

#endif
