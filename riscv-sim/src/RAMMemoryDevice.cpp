#include "RAMMemoryDevice.h"
#include <stdexcept>
#include <cstring>

CRAMMemoryDevice::CRAMMemoryDevice(uint32_t size){
    DBaseAddress = 0;
    DData.resize(size);
}

uint8_t *CRAMMemoryDevice::AccessAddress(uint32_t addr, uint32_t size){
    uint32_t Index = addr - DBaseAddress;
    if((Index < DData.size())&&(Index + size <= DData.size())){
        return DData.data() + Index;
    }
    throw std::out_of_range("CRAMMemoryDevice addr out of range!");
}

uint8_t CRAMMemoryDevice::LoadUINT8(uint32_t addr){
    return *AccessAddress(addr, sizeof(uint8_t));
}

uint16_t CRAMMemoryDevice::LoadUINT16(uint32_t addr){
    return *(uint16_t *)AccessAddress(addr, sizeof(uint16_t));
}

uint32_t CRAMMemoryDevice::LoadUINT32(uint32_t addr){
    return *(uint32_t *)AccessAddress(addr, sizeof(uint32_t));
}

uint64_t CRAMMemoryDevice::LoadUINT64(uint32_t addr){
    return *(uint64_t *)AccessAddress(addr, sizeof(uint64_t));
}

void CRAMMemoryDevice::StoreUINT8(uint32_t addr, uint8_t val){
    *AccessAddress(addr, sizeof(uint8_t)) = val;
}

void CRAMMemoryDevice::StoreUINT16(uint32_t addr, uint16_t val){
    *(uint16_t *)AccessAddress(addr, sizeof(uint16_t)) = val;
}

void CRAMMemoryDevice::StoreUINT32(uint32_t addr, uint32_t val){
    *(uint32_t *)AccessAddress(addr, sizeof(uint32_t)) = val;
}

void CRAMMemoryDevice::StoreUINT64(uint32_t addr, uint64_t val){
    *(uint64_t *)AccessAddress(addr, sizeof(uint64_t)) = val;
}

const uint8_t *CRAMMemoryDevice::LoadData(uint32_t addr, uint32_t size){
    return AccessAddress(addr,size);
}

void CRAMMemoryDevice::StoreData(uint32_t addr, const uint8_t *src, uint32_t size){
    std::memcpy(AccessAddress(addr,size),src,size);
}

