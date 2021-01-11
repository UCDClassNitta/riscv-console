#ifndef RISCVLTYPEINSTRUCTION_H
#define RISCVLTYPEINSTRUCTION_H

#include "RISCVCPU.h"

#pragma pack(1)

#pragma pack()

class CRISCVLDTypeInstruction : public CRISCVCPU::CInstruction{
    protected:
        std::shared_ptr< CHardwareRegister< uint32_t > > DProgramCounter;
        std::shared_ptr< CHardwareRegister< uint32_t > > DDestination;
        std::shared_ptr< CHardwareRegister< uint32_t > > DSource1;
        std::shared_ptr< CMemoryDevice > DMemory;
        int32_t DOffset;
    public:
        CRISCVLDTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem);
        virtual ~CRISCVLDTypeInstruction(){};

        virtual bool Execute() = 0;

        virtual std::string ToString() const;

        static std::shared_ptr< CInstruction > DecodeLDType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem);
};


#endif
