#include "RISCVBTypeInstruction.h"
#include <sstream>

class CRISCVBEQInstruction : public CRISCVBTypeInstruction{
    public:
        CRISCVBEQInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVBTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DProgramCounter->fetch_add(DSource1->load() == DSource2->load() ? DOffset : DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVBTypeInstruction::ToString().replace(0,3,"beq");
        };
};

class CRISCVBNEInstruction : public CRISCVBTypeInstruction{
    public:
        CRISCVBNEInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVBTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DProgramCounter->fetch_add(DSource1->load() != DSource2->load() ? DOffset : DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVBTypeInstruction::ToString().replace(0,3,"bne");
        };
};

class CRISCVBLTInstruction : public CRISCVBTypeInstruction{
    public:
        CRISCVBLTInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVBTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            int32_t Temp1, Temp2;
            *(uint32_t *)&Temp1 = DSource1->load();
            *(uint32_t *)&Temp2 = DSource2->load();
            DProgramCounter->fetch_add(Temp1 < Temp2 ? DOffset : DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVBTypeInstruction::ToString().replace(0,3,"blt");
        };
};

class CRISCVBGEInstruction : public CRISCVBTypeInstruction{
    public:
        CRISCVBGEInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVBTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            int32_t Temp1, Temp2;
            *(uint32_t *)&Temp1 = DSource1->load();
            *(uint32_t *)&Temp2 = DSource2->load();
            DProgramCounter->fetch_add(Temp1 >= Temp2 ? DOffset : DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVBTypeInstruction::ToString().replace(0,3,"bge");
        };
};

class CRISCVBLTUInstruction : public CRISCVBTypeInstruction{
    public:
        CRISCVBLTUInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVBTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DProgramCounter->fetch_add(DSource1->load() < DSource2->load() ? DOffset : DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVBTypeInstruction::ToString().replace(0,4,"bltu");
        };
};

class CRISCVBGEUInstruction : public CRISCVBTypeInstruction{
    public:
        CRISCVBGEUInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVBTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DProgramCounter->fetch_add(DSource1->load() >= DSource2->load() ? DOffset : DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVBTypeInstruction::ToString().replace(0,4,"bgeu");
        };
};

CRISCVBTypeInstruction::CRISCVBTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CInstruction(addr,raw){
    SRISCVBType *Encoded = (SRISCVBType *)&raw;
    DProgramCounter = pc;
    DSource1 = regs[Encoded->rs1];
    DSource2 = regs[Encoded->rs2];
    DOffset = (uint32_t(Encoded->imm12)<<12) | (uint32_t(Encoded->imm11)<<11) | (uint32_t(Encoded->imm10_5)<<5) | (uint32_t(Encoded->imm4_1)<<1);
    DOffset |= DOffset & 0x1000 ? 0xFFFFE000 : 0;
}

std::string CRISCVBTypeInstruction::ToString() const{
    SRISCVBType *Encoded = (SRISCVBType *)&DRawInstruction;
    std::stringstream TempStream;
    TempStream<<std::hex<<uint32_t(DAddress + DOffset);
    return std::string("       ") + RegisterName(Encoded->rs1) + "," + RegisterName(Encoded->rs2) + "," + TempStream.str();
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVBTypeInstruction::DecodeBType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs){
    SRISCVBType *Encoded = (SRISCVBType *)&raw;

    switch(Encoded->funct3){
        case 0x0:   return std::make_shared<CRISCVBEQInstruction>(addr,raw,pc,regs);
        case 0x1:   return std::make_shared<CRISCVBNEInstruction>(addr,raw,pc,regs);
        case 0x4:   return std::make_shared<CRISCVBLTInstruction>(addr,raw,pc,regs);
        case 0x5:   return std::make_shared<CRISCVBGEInstruction>(addr,raw,pc,regs);
        case 0x6:   return std::make_shared<CRISCVBLTUInstruction>(addr,raw,pc,regs);
        case 0x7:   return std::make_shared<CRISCVBGEUInstruction>(addr,raw,pc,regs);
        default:    return nullptr;
    }
}
