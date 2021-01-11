#include "RegisterBlockMemoryDevice.h"


size_t CRegisterBlockMemoryDevice::CalculateRegisterIndex(uint32_t addr, uint32_t size){
    uint32_t Index = addr - DBaseAddress;
    if((Index < DScratchRead.size())&&(Index + size <= DScratchRead.size())){
        return Index / sizeof(uint32_t);
    }
    throw std::out_of_range("CRegisterBlockMemoryDevice addr out of range!");

}

bool CRegisterBlockMemoryDevice::AttachDevice(std::shared_ptr< CMemoryDevice > device, uint32_t addr){
    return false;
}

uint8_t CRegisterBlockMemoryDevice::LoadUINT8(uint32_t addr){
    auto Register = DRegisters[CalculateRegisterIndex(addr, sizeof(uint8_t))];
    return Register->load()>>((addr & 0x3) * 8);
}

// Ignoring potential alignment issues
uint16_t CRegisterBlockMemoryDevice::LoadUINT16(uint32_t addr){
    auto Register = DRegisters[CalculateRegisterIndex(addr, sizeof(uint16_t))];
    return Register->load()>>((addr & 0x2) * 8);    
}

uint32_t CRegisterBlockMemoryDevice::LoadUINT32(uint32_t addr){
    return DRegisters[CalculateRegisterIndex(addr, sizeof(uint32_t))]->load();
}

uint64_t CRegisterBlockMemoryDevice::LoadUINT64(uint32_t addr){
    auto Index = CalculateRegisterIndex(addr, sizeof(uint64_t));
    uint64_t Result = DRegisters[Index]->load();
    return (uint64_t(DRegisters[Index+1]->load())<<32) | Result;
}

void CRegisterBlockMemoryDevice::StoreUINT8(uint32_t addr, uint8_t val){
    auto Register = DRegisters[CalculateRegisterIndex(addr, sizeof(uint8_t))];
    Register->store((Register->load() & (~(0xFF<<((addr & 0x3) * 8)))) | (uint32_t(val)<<((addr & 0x3) * 8)));
}

void CRegisterBlockMemoryDevice::StoreUINT16(uint32_t addr, uint16_t val){
    auto Register = DRegisters[CalculateRegisterIndex(addr, sizeof(uint16_t))];
    Register->store((Register->load() & (~(0xFFFF<<((addr & 0x2) * 8)))) | (uint32_t(val)<<((addr & 0x3) * 8)));
}

void CRegisterBlockMemoryDevice::StoreUINT32(uint32_t addr, uint32_t val){
    DRegisters[CalculateRegisterIndex(addr, sizeof(uint32_t))]->store(val);
}

void CRegisterBlockMemoryDevice::StoreUINT64(uint32_t addr, uint64_t val){
    auto Index = CalculateRegisterIndex(addr, sizeof(uint64_t));
    DRegisters[Index]->store(val);
    DRegisters[Index+1]->store(val>>32);
}

const uint8_t *CRegisterBlockMemoryDevice::LoadData(uint32_t addr, uint32_t size){
    auto Index = CalculateRegisterIndex(addr, sizeof(uint8_t));
    auto Count = ((size + 3) / 4) + ((addr + size) & 0x3 ? 1 : 0);
    uint32_t *Base = (uint32_t *)DScratchRead.data() + Index;
    while(Count--){
        *Base++ = DRegisters[Index++]->load();
    }
    return DScratchRead.data() + (addr - DBaseAddress);
}

void CRegisterBlockMemoryDevice::StoreData(uint32_t addr, const uint8_t *src, uint32_t size){
    auto Index = CalculateRegisterIndex(addr, sizeof(uint8_t));
    switch(addr & 0x3){
        case 1:     {
                        uint32_t Original = DRegisters[Index]->load();
                        Original &= 0xFFFF00FF;
                        Original |= uint32_t(*src++)<<8;
                        size--;
                        if(!size){
                            DRegisters[Index]->store(Original);
                            return;
                        }
                        Original &= 0xFF00FFFF;
                        Original |= uint32_t(*src++)<<16;
                        size--;
                        if(!size){
                            DRegisters[Index]->store(Original);
                            return;
                        }
                        Original &= 0x00FFFFFF;
                        Original |= uint32_t(*src++)<<24;
                        size--;
                        DRegisters[Index++]->store(Original);
                    }
                    break;
        case 2:     {
                        uint32_t Original = DRegisters[Index]->load();
                        Original &= 0xFF00FFFF;
                        Original |= uint32_t(*src++)<<16;
                        size--;
                        if(!size){
                            DRegisters[Index]->store(Original);
                            return;
                        }
                        Original &= 0x00FFFFFF;
                        Original |= uint32_t(*src++)<<24;
                        size--;
                        DRegisters[Index++]->store(Original);
                    }
                    break;
        case 3:     {
                        uint32_t Original = DRegisters[Index]->load();
                        Original &= 0x00FFFFFF;
                        Original |= uint32_t(*src++)<<24;
                        size--;
                        DRegisters[Index++]->store(Original);
                    }
                    break;
        default:    break;
    }
    while(sizeof(uint32_t) <= size){
        DRegisters[Index++]->store(*(uint32_t *)src);
        src += sizeof(uint32_t);
        size -= sizeof(uint32_t);
    }
    if(size){
        uint32_t Original = DRegisters[Index]->load();
        Original &= 0xFFFFFF00;
        Original |= uint32_t(*src++);
        size--;
        if(!size){
            DRegisters[Index]->store(Original);
            return;
        }
        Original &= 0xFFFF00FF;
        Original |= uint32_t(*src++)<<8;
        size--;
        if(!size){
            DRegisters[Index]->store(Original);
            return;
        }
        Original &= 0xFF00FFFF;
        Original |= uint32_t(*src++)<<16;
        size--;
        DRegisters[Index]->store(Original);
    }
}

void CRegisterBlockMemoryDevice::AttachRegister(std::shared_ptr< CHardwareRegister< uint32_t > > reg){
    DRegisters.push_back(reg);
    DScratchRead.resize(DScratchRead.size() + sizeof(uint32_t));
}
