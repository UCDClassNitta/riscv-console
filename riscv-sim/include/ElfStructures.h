#ifndef ELFSTRUCTURES_H
#define ELFSTRUCTURES_H

#include "DataSource.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <utility>
#include <string>
#include "ElfConstants.h"

class CElfStructures{
    public:
        struct SProgramHeader{
            CElfConstants::EProgramHeaderType DSegmentType;
            uint32_t DFlags;
            uint64_t DFileOffset;
            uint64_t DVirtualAddress;
            uint64_t DPhysicalAddress;
            uint64_t DFileSize;
            uint64_t DMemorySize;
            uint64_t DAlignment;
            std::vector<uint8_t> DPayload;
            std::unordered_map<uint64_t,std::string> DSymbols;
        };

        struct SSectionHeader{
            uint32_t DNameIndex;
            CElfConstants::ESectionHeaderType DType;
            uint64_t DFlags;
            uint64_t DVirtualAddress;
            uint64_t DFileOffset;
            uint64_t DSize;
            uint32_t DLink;
            uint64_t DInfo;
            uint64_t DAddressAlign;
            uint64_t DEntrySize;
            std::unordered_map<uint64_t,std::string> DSymbols;
        };

        struct SSymbolEntity{
            uint32_t DNameIndex;
            uint8_t DInfo;
            uint8_t DOther;
            uint16_t DSectionIndex;
            uint64_t DAddress;
            uint64_t DSize;
            uint32_t DNameSectionIndex;
        };

        class CStringTable{
            std::vector< char > DPayload;
            public:
                void SetPayload(const std::vector< char > &payload){
                    DPayload = payload;
                };

                std::string GetString(uint32_t index) const{
                    if(index < DPayload.size()){
                        return std::string(DPayload.data() + index);
                    }
                    return std::string();
                };
        };
};

#endif
