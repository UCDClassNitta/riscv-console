#ifndef MEMORYCONTROLLERDEVICE_H
#define MEMORYCONTROLLERDEVICE_H

#include "MemoryDevice.h"

class CMemoryControllerDevice : public CMemoryDevice{
    protected:
        uint32_t DBaseAddress;
        uint32_t DMemorySize;
        uint32_t DShiftBits;
        std::vector< std::shared_ptr<CMemoryDevice> > DSubDevices;

        std::shared_ptr<CMemoryDevice> AccessAddress(uint32_t addr, uint32_t size);

    public:
        CMemoryControllerDevice(uint32_t bits);
        virtual ~CMemoryControllerDevice(){};
        virtual void DumpData(std::ostream &out, uint32_t saddr=0, uint32_t eaddr=0);
        virtual bool AttachDevice(std::shared_ptr< CMemoryDevice > device, uint32_t addr);
        virtual uint32_t BaseAddress() const noexcept{
            return DBaseAddress;
        };

        virtual uint32_t BaseAddress(uint32_t addr) noexcept{
            return DBaseAddress = addr;
        };

        virtual uint32_t MemorySize() const noexcept{
            return DMemorySize;
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
};

#endif
