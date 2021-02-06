#ifndef RISCVBLOCKINSTRUCTIONCACHE_H
#define RISCVBLOCKINSTRUCTIONCACHE_H

#include "RISCVCPU.h"
#include <unordered_map>

class CRISCVBlockInstructionCache : public CRISCVCPU::CInstructionCache{
    protected:
        struct SSubBlock{
            uint32_t DBaseAddress;
            std::vector< std::shared_ptr< CRISCVCPU::CInstruction > > DInstructions;
        };
        struct SBlock{
            uint32_t DBaseAddress;
            std::vector< std::shared_ptr< SSubBlock > > DSubBlocks;
        };
        std::shared_ptr< SSubBlock > DCurrentSubBlock;
        std::shared_ptr< SBlock > DCurrentBlock;
        std::unordered_map< uint32_t, std::shared_ptr< SBlock > > DHashedBlocks;
        std::unordered_map< uint32_t, std::shared_ptr< SSubBlock > > DHashedSubBlocks;
        static const uint32_t DSubBlockSize;
        static const uint32_t DSubBlockMask;
        static const uint32_t DBlockSize;
        static const uint32_t DBlockMask;

        std::shared_ptr< SBlock > GetBlock(uint32_t addr);
        std::shared_ptr< SSubBlock > GetSubBlock(uint32_t addr);

    public:
        CRISCVBlockInstructionCache();
        virtual ~CRISCVBlockInstructionCache(){};
        virtual std::shared_ptr< CRISCVCPU::CInstruction > Fetch(uint32_t addr);
        virtual void Insert(std::shared_ptr< CRISCVCPU::CInstruction > inst);
        virtual void Reset();
        virtual void FlushRange(uint32_t addr, uint32_t size);
};

#endif
