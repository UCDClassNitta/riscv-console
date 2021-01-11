#include "RISCVJTypeInstruction.h"
#include "RISCVITypeInstruction.h"
#include <sstream>

class CRISCVJALInstruction : public CRISCVJTypeInstruction{
    public:
        CRISCVJALInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVJTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DProgramCounter->exchange(DAddress + DOffset) + DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            SRISCVJType *Encoded = (SRISCVJType *)&DRawInstruction;
            if(!Encoded->rd){
                std::stringstream TempStream;
                TempStream<<std::hex<<uint32_t(DAddress + DOffset);
                return std::string("j      ") + TempStream.str();
            }
            return CRISCVJTypeInstruction::ToString().replace(0,3,"jal");
        };
};

class CRISCVJALRInstruction : public CRISCVJTypeInstruction{
    public:
        CRISCVJALRInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVJTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DProgramCounter->exchange(DSource1->load() + DOffset) + DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            if(!DOffset){
                SRISCVIType *Encoded = (SRISCVIType *)&DRawInstruction;
                if(!Encoded->rd){
                    if(Encoded->rs1 == 1){
                        return std::string("ret    ");
                    }
                    if(Encoded->rd == 1){
                        std::string("jalr   ") + RegisterName(Encoded->rs1);
                    }
                }
            }
            return CRISCVJTypeInstruction::ToString().replace(0,4,"jalr");
        };
};

CRISCVJTypeInstruction::CRISCVJTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CInstruction(addr,raw){
    SRISCVJType *Encoded = (SRISCVJType *)&raw;
    DProgramCounter = pc;
    DDestination = regs[Encoded->rd];
    if(Encoded->opcode == 0x6F){
        DOffset = (uint32_t(Encoded->imm20)<<20) | (uint32_t(Encoded->imm19_12)<<12) | (uint32_t(Encoded->imm11)<<11) | (uint32_t(Encoded->imm10_1)<<1);
        DOffset |= DOffset & 0x00100000 ? 0xFFE00000 : 0;
    }
    else if(Encoded->opcode == 0x67){
        SRISCVIType *EncodedI = (SRISCVIType *)&raw;
        DSource1 = regs[EncodedI->rs1];
        DOffset = EncodedI->imm11_0 & 0x800 ? EncodedI->imm11_0 | 0xFFFFF000 : EncodedI->imm11_0;
    }
}

std::string CRISCVJTypeInstruction::ToString() const{
    SRISCVIType *Encoded = (SRISCVIType *)&DRawInstruction;
    if(DSource1){
        return std::string("       ") + RegisterName(Encoded->rd) + "," + RegisterName(Encoded->rs1) + "," + std::to_string(DOffset);
    }
    else{
        std::stringstream TempStream;
        TempStream<<std::hex<<uint32_t(DAddress + DOffset);
        return std::string("       ") + RegisterName(Encoded->rd) + "," +  TempStream.str();
    }
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVJTypeInstruction::DecodeJType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs){
    SRISCVJType *Encoded = (SRISCVJType *)&raw;

    switch(Encoded->opcode){
        case 0x6F:  return std::make_shared<CRISCVJALInstruction>(addr,raw,pc,regs);
        case 0x67:  return std::make_shared<CRISCVJALRInstruction>(addr,raw,pc,regs);
        default:    return nullptr;
    }
}