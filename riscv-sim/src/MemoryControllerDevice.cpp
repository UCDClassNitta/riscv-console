#include "MemoryControllerDevice.h"
#include <cstdio>

static const uint32_t CMemoryControllerDeviceIndexBits = 4;
static const uint32_t CMemoryControllerDeviceIndices = 1<<CMemoryControllerDeviceIndexBits;
static const uint32_t CMemoryControllerDeviceIndexMask = CMemoryControllerDeviceIndices - 1;


CMemoryControllerDevice::CMemoryControllerDevice(uint32_t bits){
    DBaseAddress = 0;
    DMemorySize = 32 > bits ? 1<<bits : 0;
    DShiftBits = bits - CMemoryControllerDeviceIndexBits;
    DSubDevices.resize(CMemoryControllerDeviceIndices);
}

std::shared_ptr<CMemoryDevice> CMemoryControllerDevice::AccessAddress(uint32_t addr, uint32_t size){
    uint32_t Offset = addr - DBaseAddress;
    uint32_t Index = (Offset>>DShiftBits) & CMemoryControllerDeviceIndexMask;
    
    if((DMemorySize && ((DMemorySize <= Offset)||(Offset + size >= DMemorySize)))||(!DSubDevices[Index])){
        throw std::out_of_range("CMemoryControllerDevice addr out of range!");
    }
    return DSubDevices[Index];
}

void CMemoryControllerDevice::DumpData(std::ostream &out, uint32_t saddr, uint32_t eaddr){
    for(auto &SubDevice : DSubDevices){
        if(SubDevice){
            SubDevice->DumpData(out,saddr,eaddr);
        }
    }
}

bool CMemoryControllerDevice::AttachDevice(std::shared_ptr< CMemoryDevice > device, uint32_t addr){
    uint32_t Offset = addr - DBaseAddress;
    if(DMemorySize && DMemorySize <= Offset){
        printf("Address out of range! addr = %08X, MemSz = %08X, Offset = %08X\n",addr,DMemorySize,Offset);
        return false;
    }
    uint32_t Index = (Offset>>DShiftBits) & CMemoryControllerDeviceIndexMask;
    auto &SubDevice = DSubDevices[Index];
    if(!SubDevice){
        DSubDevices[Index] = device;
        device->BaseAddress(addr);
        return true;
    }
    if(SubDevice->AttachDevice(device,addr)){
        return true;
    }
    uint32_t NewBits = DShiftBits;
    uint32_t OtherOffset = SubDevice->BaseAddress() - DBaseAddress;
    uint32_t CurrentMask = CMemoryControllerDeviceIndexMask<<(NewBits - CMemoryControllerDeviceIndexBits);
    uint32_t DiffOffset = Offset ^ OtherOffset;
    while(CurrentMask && !(DiffOffset & CurrentMask)){
        CurrentMask >>= CMemoryControllerDeviceIndexBits;
        NewBits -= CMemoryControllerDeviceIndexBits;
    }
    if(!CurrentMask){
        return false;
    }
    uint32_t NewBase = addr & ((~(uint32_t)0)<<NewBits);
    auto NewMemoryController = std::make_shared<CMemoryControllerDevice>(NewBits);
    NewMemoryController->BaseAddress(NewBase);
    if(!NewMemoryController->AttachDevice(device,addr)){
        return false;
    }
    if(!NewMemoryController->AttachDevice(SubDevice,SubDevice->BaseAddress())){
        return false;
    }
    DSubDevices[Index] = NewMemoryController;
    return true;
}

uint8_t CMemoryControllerDevice::LoadUINT8(uint32_t addr){
    return AccessAddress(addr, sizeof(uint8_t))->LoadUINT8(addr);
}

uint16_t CMemoryControllerDevice::LoadUINT16(uint32_t addr){
    return AccessAddress(addr, sizeof(uint16_t))->LoadUINT16(addr);
}

uint32_t CMemoryControllerDevice::LoadUINT32(uint32_t addr){
    return AccessAddress(addr, sizeof(uint32_t))->LoadUINT32(addr);
}

uint64_t CMemoryControllerDevice::LoadUINT64(uint32_t addr){
    return AccessAddress(addr, sizeof(uint64_t))->LoadUINT64(addr);
}

void CMemoryControllerDevice::StoreUINT8(uint32_t addr, uint8_t val){
    AccessAddress(addr, sizeof(uint8_t))->StoreUINT8(addr, val);
}

void CMemoryControllerDevice::StoreUINT16(uint32_t addr, uint16_t val){
    AccessAddress(addr, sizeof(uint16_t))->StoreUINT16(addr, val);
}

void CMemoryControllerDevice::StoreUINT32(uint32_t addr, uint32_t val){
    AccessAddress(addr, sizeof(uint32_t))->StoreUINT32(addr, val);
}

void CMemoryControllerDevice::StoreUINT64(uint32_t addr, uint64_t val){
    AccessAddress(addr, sizeof(uint64_t))->StoreUINT64(addr, val);
}

const uint8_t *CMemoryControllerDevice::LoadData(uint32_t addr, uint32_t size){
    return AccessAddress(addr, size)->LoadData(addr, size);
}

void CMemoryControllerDevice::StoreData(uint32_t addr, const uint8_t *src, uint32_t size){
    AccessAddress(addr, size)->StoreData(addr, src, size);
}
