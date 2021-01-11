#ifndef RISCVSTYPEINSTRUCTION_H
#define RISCVSTYPEINSTRUCTION_H

#include "RISCVCPU.h"

#pragma pack(1)
struct SRISCVSType{
    uint32_t opcode : 7;
    uint32_t imm4_0 : 5;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t imm11_5 : 7;
};
#pragma pack()

class CRISCVSTypeInstruction : public CRISCVCPU::CInstruction{
    protected:
        std::shared_ptr< CHardwareRegister< uint32_t > > DProgramCounter;
        std::shared_ptr< CHardwareRegister< uint32_t > > DSource1;
        std::shared_ptr< CHardwareRegister< uint32_t > > DSource2;
        std::shared_ptr< CMemoryDevice > DMemory;
        int32_t DOffset;
    public:
        CRISCVSTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem);
        virtual ~CRISCVSTypeInstruction(){};

        virtual bool Execute() = 0;

        virtual std::string ToString() const;

        static std::shared_ptr< CInstruction > DecodeSType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem);
};

#endif
