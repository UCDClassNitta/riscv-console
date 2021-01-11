#ifndef MEMORYDEVICE_H
#define MEMORYDEVICE_H

#include <memory>
#include <vector>
#include <cstdint>
#include <ostream>

class CMemoryDevice{
    public:
        virtual ~CMemoryDevice(){};
        virtual void DumpData(std::ostream &out, uint32_t saddr=0, uint32_t eaddr=0);
        virtual bool AttachDevice(std::shared_ptr< CMemoryDevice > device, uint32_t addr) = 0;
        virtual uint32_t BaseAddress() const noexcept = 0;
        virtual uint32_t BaseAddress(uint32_t addr) noexcept = 0;
        virtual uint32_t MemorySize() const noexcept = 0;

        virtual uint8_t LoadUINT8(uint32_t addr) = 0;
        virtual uint16_t LoadUINT16(uint32_t addr) = 0;
        virtual uint32_t LoadUINT32(uint32_t addr) = 0;
        virtual uint64_t LoadUINT64(uint32_t addr) = 0;

        virtual void StoreUINT8(uint32_t addr, uint8_t val) = 0;
        virtual void StoreUINT16(uint32_t addr, uint16_t val) = 0;
        virtual void StoreUINT32(uint32_t addr, uint32_t val) = 0;
        virtual void StoreUINT64(uint32_t addr, uint64_t val) = 0;

        virtual const uint8_t *LoadData(uint32_t addr, uint32_t size) = 0;
        virtual void StoreData(uint32_t addr, const uint8_t *src, uint32_t size) = 0;
};

#endif
