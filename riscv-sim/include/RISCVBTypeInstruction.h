#ifndef RISCVBTYPEINSTRUCTION_H
#define RISCVBTYPEINSTRUCTION_H

#include "RISCVCPU.h"

#pragma pack(1)
struct SRISCVBType{
    uint32_t opcode : 7;
    uint32_t imm11 : 1;
    uint32_t imm4_1 : 4;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t imm10_5 : 6;
    uint32_t imm12 : 1;
};
#pragma pack()

class CRISCVBTypeInstruction : public CRISCVCPU::CInstruction{
    protected:
        std::shared_ptr< CHardwareRegister< uint32_t > > DProgramCounter;
        std::shared_ptr< CHardwareRegister< uint32_t > > DSource1;
        std::shared_ptr< CHardwareRegister< uint32_t > > DSource2;
        int32_t DOffset;
    public:
        CRISCVBTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs);
        virtual ~CRISCVBTypeInstruction(){};

        virtual bool Execute() = 0;

        virtual std::string ToString() const;

        static std::shared_ptr< CInstruction > DecodeBType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs);
};


#endif
