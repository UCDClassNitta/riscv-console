#ifndef FLASHMEMORYDEVICE_H
#define FLASHMEMORYDEVICE_H

#include "MemoryDevice.h"
#include <vector>

class CFlashMemoryDevice : public CMemoryDevice{
    protected:
        uint32_t DBaseAddress;
        std::vector<uint8_t> DData;
        bool DWriteEnabled;
        uint8_t *AccessAddress(uint32_t addr, uint32_t size);

    public:
        CFlashMemoryDevice(uint32_t size);
        virtual ~CFlashMemoryDevice(){};

        virtual bool AttachDevice(std::shared_ptr< CMemoryDevice > device, uint32_t addr){
            return false;
        };

        virtual uint32_t BaseAddress() const noexcept{
            return DBaseAddress;
        };

        virtual uint32_t BaseAddress(uint32_t addr) noexcept{
            return DBaseAddress = addr;
        };

        virtual uint32_t MemorySize() const noexcept{
            return uint32_t(DData.size());
        };

        virtual bool WriteEnabled() const noexcept{
            return DWriteEnabled;
        };

        virtual bool WriteEnabled(bool enable) noexcept{
            return DWriteEnabled = enable;
        };

        virtual bool EraseAll() noexcept;

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
