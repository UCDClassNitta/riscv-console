#ifndef ELFLOAD_H
#define ELFLOAD_H

#include "DataSource.h"
#include <cstdint>
#include <vector>

class CElfLoad{
    public:
        struct CProgramHeader{
            uint32_t DSegmentType;
            uint32_t DFlags;
            uint64_t DFileOffset;
            uint64_t DVirtualAddress;
            uint64_t DPhysicalAddress;
            uint64_t DFileSize;
            uint64_t DMemorySize;
            uint64_t DAlignment;
            std::vector<uint8_t> DPayload;
            void Print();
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

        std::vector< CProgramHeader > DProgramHeaders;
        bool DValidFile;

        bool ValidateHeader();
        void PrintHeader();

        void ReadData(uint8_t *buf, size_t size);
        void Seek(uint32_t pos);
        uint16_t ReadUINT16();
        uint32_t ReadUINT32();
        uint64_t ReadUINT64();

        bool ReadProgramHeaders();
        bool LoadProgramPayloads();

    public:
        CElfLoad(std::shared_ptr< CDataSource > src);
        ~CElfLoad();

        bool ValidFile() const{
            return DValidFile;
        };
        size_t ProgramHeaderCount() const;
        const CProgramHeader &ProgramHeader(size_t index) const;
        void PrintHeaders();



};

#endif
