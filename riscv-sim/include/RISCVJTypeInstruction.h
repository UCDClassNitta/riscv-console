#ifndef RISCVJTYPEINSTRUCTION_H
#define RISCVJTYPEINSTRUCTION_H

#include "RISCVCPU.h"

#pragma pack(1)
struct SRISCVJType{
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t imm19_12 : 8;
    uint32_t imm11 : 1;
    uint32_t imm10_1 : 10;
    uint32_t imm20 : 1;
};
#pragma pack()

class CRISCVJTypeInstruction : public CRISCVCPU::CInstruction{
    protected:
        std::shared_ptr< CHardwareRegister< uint32_t > > DProgramCounter;
        std::shared_ptr< CHardwareRegister< uint32_t > > DDestination;
        std::shared_ptr< CHardwareRegister< uint32_t > > DSource1;
        int32_t DOffset;
    public:
        CRISCVJTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs);
        virtual ~CRISCVJTypeInstruction(){};

        virtual bool Execute() = 0;

        virtual std::string ToString() const;

        static std::shared_ptr< CInstruction > DecodeJType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs);
};

#endif
