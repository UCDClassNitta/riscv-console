#ifndef RISCVITYPEINSTRUCTION_H
#define RISCVITYPEINSTRUCTION_H

#include "RISCVCPU.h"

#pragma pack(1)
struct SRISCVIType{
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t imm11_0 : 12;
};
#pragma pack()

class CRISCVITypeInstruction : public CRISCVCPU::CInstruction{
    protected:
        std::shared_ptr< CHardwareRegister< uint32_t > > DProgramCounter;
        std::shared_ptr< CHardwareRegister< uint32_t > > DDestination;
        std::shared_ptr< CHardwareRegister< uint32_t > > DSource1;
        int32_t DOffset;
    public:
        CRISCVITypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs);
        virtual ~CRISCVITypeInstruction(){};

        virtual bool Execute() = 0;

        virtual std::string ToString() const;

        static std::shared_ptr< CInstruction > DecodeIType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs);
};


#endif
