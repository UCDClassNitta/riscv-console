#ifndef RISCVUTYPEINSTRUCTION_H
#define RISCVUTYPEINSTRUCTION_H

#include "RISCVCPU.h"

#pragma pack(1)
struct SRISCVUType{
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t imm31_12 : 20;
};
#pragma pack()

class CRISCVUTypeInstruction : public CRISCVCPU::CInstruction{
    protected:
        std::shared_ptr< CHardwareRegister< uint32_t > > DProgramCounter;
        std::shared_ptr< CHardwareRegister< uint32_t > > DDestination;
        uint32_t DOffset;
        
    public:
        CRISCVUTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs);
        virtual ~CRISCVUTypeInstruction(){};
        
        virtual bool Execute() = 0;

        virtual std::string ToString() const;

        static std::shared_ptr< CInstruction > DecodeUType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs);
};

#endif
