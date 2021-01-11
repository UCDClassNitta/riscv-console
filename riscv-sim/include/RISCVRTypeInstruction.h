#ifndef RISCVRTYPEINSTRUCTION_H
#define RISCVRTYPEINSTRUCTION_H

#include "RISCVCPU.h"

#pragma pack(1)
struct SRISCVRType{
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t funct7 : 7;
};
#pragma pack()

class CRISCVRTypeInstruction : public CRISCVCPU::CInstruction{
    protected:
        std::shared_ptr< CHardwareRegister< uint32_t > > DProgramCounter;
        std::shared_ptr< CHardwareRegister< uint32_t > > DDestination;
        std::shared_ptr< CHardwareRegister< uint32_t > > DSource1;
        std::shared_ptr< CHardwareRegister< uint32_t > > DSource2;
    public:
        CRISCVRTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs);
        virtual ~CRISCVRTypeInstruction(){};
        virtual bool Execute() = 0;

        virtual std::string ToString() const;

        static std::shared_ptr< CInstruction > DecodeRType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs);
};

#endif
