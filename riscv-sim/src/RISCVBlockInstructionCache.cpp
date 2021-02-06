#include "RISCVBlockInstructionCache.h"

const uint32_t CRISCVBlockInstructionCache::DSubBlockSize = 1024;
const uint32_t CRISCVBlockInstructionCache::DSubBlockMask = ~(CRISCVBlockInstructionCache::DSubBlockSize - 1);
const uint32_t CRISCVBlockInstructionCache::DBlockSize = CRISCVBlockInstructionCache::DSubBlockSize * 4;
const uint32_t CRISCVBlockInstructionCache::DBlockMask = ~(CRISCVBlockInstructionCache::DBlockSize - 1);

CRISCVBlockInstructionCache::CRISCVBlockInstructionCache(){
    DCurrentBlock = GetBlock(0);
    DCurrentSubBlock = DCurrentBlock->DSubBlocks[0];
}

std::shared_ptr< CRISCVBlockInstructionCache::SBlock > CRISCVBlockInstructionCache::GetBlock(uint32_t addr){
    auto BlockSearch = DHashedBlocks.find(addr);
    if(DHashedBlocks.end() == BlockSearch){
        auto NewBlock = std::make_shared< SBlock >();
        NewBlock->DBaseAddress = addr;
        for(uint32_t Offset = 0; Offset < DBlockSize; Offset += DSubBlockSize){
            NewBlock->DSubBlocks.push_back(GetSubBlock(addr + Offset));
        }
        DHashedBlocks[addr] = NewBlock;
        return NewBlock;
    }
    return BlockSearch->second;
}

std::shared_ptr< CRISCVBlockInstructionCache::SSubBlock > CRISCVBlockInstructionCache::GetSubBlock(uint32_t addr){
    auto SubBlockSearch = DHashedSubBlocks.find(addr);
    if(DHashedSubBlocks.end() == SubBlockSearch){
        auto NewSubBlock = std::make_shared< SSubBlock >();
        NewSubBlock->DBaseAddress = addr;
        NewSubBlock->DInstructions.resize(DSubBlockSize/4);
        DHashedSubBlocks[addr] = NewSubBlock;
        return NewSubBlock;
    }
    return SubBlockSearch->second;
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVBlockInstructionCache::Fetch(uint32_t addr){
    uint32_t SubBlockAddr = addr & DSubBlockMask;
    if(DCurrentSubBlock->DBaseAddress != SubBlockAddr){
        if((DCurrentBlock->DBaseAddress > SubBlockAddr) || (SubBlockAddr >= DCurrentBlock->DBaseAddress + DBlockSize)){
            DCurrentBlock = GetBlock(SubBlockAddr);
        }
        auto BlockIndex = (SubBlockAddr - DCurrentBlock->DBaseAddress) / DSubBlockSize;
        DCurrentSubBlock = DCurrentBlock->DSubBlocks[BlockIndex];
    }
    auto InstructionIndex = ((addr - SubBlockAddr) & (DSubBlockSize - 1))/4;
    return DCurrentSubBlock->DInstructions[InstructionIndex];
}

void CRISCVBlockInstructionCache::Insert(std::shared_ptr< CRISCVCPU::CInstruction > inst){
    uint32_t SubBlockAddr = inst->Address() & DSubBlockMask;
    if(DCurrentSubBlock->DBaseAddress != SubBlockAddr){
        if((DCurrentBlock->DBaseAddress > SubBlockAddr) || (SubBlockAddr >= DCurrentBlock->DBaseAddress + DBlockSize)){
            DCurrentBlock = GetBlock(SubBlockAddr);
        }
        auto BlockIndex = (SubBlockAddr - DCurrentBlock->DBaseAddress) / DSubBlockSize;
        DCurrentSubBlock = DCurrentBlock->DSubBlocks[BlockIndex];
    }
    auto InstructionIndex = ((inst->Address() - SubBlockAddr) & (DSubBlockSize - 1))/4;
    DCurrentSubBlock->DInstructions[InstructionIndex] = inst;

}

void CRISCVBlockInstructionCache::Reset(){
    DHashedBlocks.clear();
    DHashedSubBlocks.clear();
    DCurrentBlock = GetBlock(0);
    DCurrentSubBlock = DCurrentBlock->DSubBlocks[0];
}

void CRISCVBlockInstructionCache::FlushRange(uint32_t addr, uint32_t size){
    for(auto &SubBlock : DHashedSubBlocks){
        if((SubBlock.first + DSubBlockSize < addr)||(addr + size < SubBlock.first)){
            continue;
        }
        for(auto &CachedInstruction : SubBlock.second->DInstructions){
            CachedInstruction.reset();
        }

    }
}