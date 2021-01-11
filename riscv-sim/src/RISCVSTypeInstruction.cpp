#include "RISCVSTypeInstruction.h"

class CRISCVSBInstruction : public CRISCVSTypeInstruction{
    public:
        CRISCVSBInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem)  : CRISCVSTypeInstruction(addr,raw, pc, regs, mem){

        };

        bool Execute(){
            DMemory->StoreUINT8(DSource1->load() + DOffset,DSource2->load());
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVSTypeInstruction::ToString().replace(0,2,"sb");
        };
};

class CRISCVSHInstruction : public CRISCVSTypeInstruction{
    public:
        CRISCVSHInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem)  : CRISCVSTypeInstruction(addr,raw, pc, regs, mem){

        };

        bool Execute(){
            DMemory->StoreUINT16(DSource1->load() + DOffset,DSource2->load());
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVSTypeInstruction::ToString().replace(0,2,"sh");
        };
};

class CRISCVSWInstruction : public CRISCVSTypeInstruction{
    public:
        CRISCVSWInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem)  : CRISCVSTypeInstruction(addr,raw, pc, regs, mem){

        };

        bool Execute(){
            DMemory->StoreUINT32(DSource1->load() + DOffset,DSource2->load());
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVSTypeInstruction::ToString().replace(0,2,"sw");
        };
};

CRISCVSTypeInstruction::CRISCVSTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem) : CInstruction(addr,raw){
    SRISCVSType *Encoded = (SRISCVSType *)&raw;
    DProgramCounter = pc;
    DSource1 = regs[Encoded->rs1];
    DSource2 = regs[Encoded->rs2];
    DMemory = mem;
    DOffset = (uint32_t(Encoded->imm11_5)<<5) | Encoded->imm4_0;
    DOffset |= DOffset & 0x800 ? 0xFFFFF000 : 0;
}

std::string CRISCVSTypeInstruction::ToString() const{
    SRISCVSType *Encoded = (SRISCVSType *)&DRawInstruction;
    return std::string("       ") + RegisterName(Encoded->rs2) + "," + std::to_string(DOffset) + "(" + RegisterName(Encoded->rs1) + ")";
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVSTypeInstruction::DecodeSType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem){
    SRISCVSType *Encoded = (SRISCVSType *)&raw;

    switch(Encoded->funct3){
        case 0x0:   return std::make_shared<CRISCVSBInstruction>(addr,raw,pc,regs,mem);
        case 0x1:   return std::make_shared<CRISCVSHInstruction>(addr,raw,pc,regs,mem);
        case 0x2:   return std::make_shared<CRISCVSWInstruction>(addr,raw,pc,regs,mem);
        
        default:    return nullptr;
    }
}
