#include "FlashMemoryDevice.h"
#include <stdexcept>
#include <cstring>
#include <string>

CFlashMemoryDevice::CFlashMemoryDevice(uint32_t size){
    DBaseAddress = 0;
    DData.resize(size);
    DWriteEnabled = false;
}

uint8_t *CFlashMemoryDevice::AccessAddress(uint32_t addr, uint32_t size){
    uint32_t Index = addr - DBaseAddress;
    if((Index < DData.size())&&(Index + size <= DData.size())){
        return DData.data() + Index;
    }

    throw std::out_of_range(std::string("CFlashMemoryDevice addr ") + std::to_string(addr) + " out of range!");
}

bool CFlashMemoryDevice::EraseAll() noexcept{
    if(DWriteEnabled){
        memset(DData.data(), 0,DData.size());
    }
    return true;
}

uint8_t CFlashMemoryDevice::LoadUINT8(uint32_t addr){
    return *AccessAddress(addr, sizeof(uint8_t));
}

uint16_t CFlashMemoryDevice::LoadUINT16(uint32_t addr){
    return *(uint16_t *)AccessAddress(addr, sizeof(uint16_t));
}

uint32_t CFlashMemoryDevice::LoadUINT32(uint32_t addr){
    return *(uint32_t *)AccessAddress(addr, sizeof(uint32_t));
}

uint64_t CFlashMemoryDevice::LoadUINT64(uint32_t addr){
    return *(uint64_t *)AccessAddress(addr, sizeof(uint64_t));
}

void CFlashMemoryDevice::StoreUINT8(uint32_t addr, uint8_t val){
    if(DWriteEnabled){
        *AccessAddress(addr, sizeof(uint8_t)) = val;
    }
}

void CFlashMemoryDevice::StoreUINT16(uint32_t addr, uint16_t val){
    if(DWriteEnabled){
        *(uint16_t *)AccessAddress(addr, sizeof(uint16_t)) = val;
    }
}

void CFlashMemoryDevice::StoreUINT32(uint32_t addr, uint32_t val){
    if(DWriteEnabled){
        *(uint32_t *)AccessAddress(addr, sizeof(uint32_t)) = val;
    }
}

void CFlashMemoryDevice::StoreUINT64(uint32_t addr, uint64_t val){
    if(DWriteEnabled){
        *(uint64_t *)AccessAddress(addr, sizeof(uint64_t)) = val;
    }
}

const uint8_t *CFlashMemoryDevice::LoadData(uint32_t addr, uint32_t size){
    return AccessAddress(addr,size);
}

void CFlashMemoryDevice::StoreData(uint32_t addr, const uint8_t *src, uint32_t size){
    if(DWriteEnabled){
        std::memcpy(AccessAddress(addr,size),src,size);
    }
}

