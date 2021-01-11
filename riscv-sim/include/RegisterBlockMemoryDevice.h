#ifndef REGISTERBLOCKMEMORYDEVICE_H
#define REGISTERBLOCKMEMORYDEVICE_H

#include <vector>
#include "MemoryDevice.h"
#include "HardwareRegister.h"

class CRegisterBlockMemoryDevice : public CMemoryDevice{
    protected:
        uint32_t DBaseAddress = 0;
        std::vector< uint8_t > DScratchRead;
        std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > DRegisters;
        size_t CalculateRegisterIndex(uint32_t addr, uint32_t size);

    public:
        virtual ~CRegisterBlockMemoryDevice(){};
        virtual bool AttachDevice(std::shared_ptr< CMemoryDevice > device, uint32_t addr);
        virtual uint32_t BaseAddress() const noexcept{
            return DBaseAddress;
        };
        virtual uint32_t BaseAddress(uint32_t addr) noexcept{
            return DBaseAddress = addr & (~0x3); // force 32-bit alignment
        };
        virtual uint32_t MemorySize() const noexcept{
            return DScratchRead.size();
        };

        virtual uint8_t LoadUINT8(uint32_t addr);
        virtual uint16_t LoadUINT16(uint32_t addr);
        virtual uint32_t LoadUINT32(uint32_t addr);
        virtual uint64_t LoadUINT64(uint32_t addr);

        virtual void StoreUINT8(uint32_t addr, uint8_t val);
        virtual void StoreUINT16(uint32_t addr, uint16_t val);
        virtual void StoreUINT32(uint32_t addr, uint32_t val);
        virtual void StoreUINT64(uint32_t addr, uint64_t val);

        virtual const uint8_t *LoadData(uint32_t addr, uint32_t size);
        virtual void StoreData(uint32_t addr, const uint8_t *src, uint32_t size);

        void AttachRegister(std::shared_ptr< CHardwareRegister< uint32_t > > reg);
};

#endif
