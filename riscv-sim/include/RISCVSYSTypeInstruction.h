#ifndef RISCVSYSTYPEINSTRUCTION_H
#define RISCVSYSTYPEINSTRUCTION_H

#include "RISCVCPU.h"

#pragma pack(1)
struct SRISCVCSRType{
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t funct3 : 3;
    uint32_t rs1_uimm : 5;
    uint32_t csr : 12;
};
#pragma pack()

class CRISCVSYSTypeInstruction : public CRISCVCPU::CInstruction{
    protected:
        std::shared_ptr< CHardwareRegister< uint32_t > > DProgramCounter;
        std::shared_ptr< CHardwareRegister< uint32_t > > DDestination;
        std::shared_ptr< CHardwareRegister< uint32_t > > DSource1;
        std::shared_ptr< CHardwareRegister< uint32_t > > DControlStatusRegister;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineStatus;
        uint32_t DImmediate;
        static const std::unordered_map< uint32_t, std::string > DControlStatusRegisterNames;
        
    public:
        CRISCVSYSTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs);
        virtual ~CRISCVSYSTypeInstruction(){};

        virtual bool Execute() = 0;

        virtual std::string ToString() const;

        static std::string ControlStatusRegisterName(uint32_t index);

        static std::shared_ptr< CInstruction > DecodeSYSType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs);
};

#endif
