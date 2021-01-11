#include "RISCVLTypeInstruction.h"
#include "RISCVITypeInstruction.h"

class CRISCVLBInstruction : public CRISCVLDTypeInstruction{
    public:
        CRISCVLBInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem)  : CRISCVLDTypeInstruction(addr,raw, pc, regs, mem){

        };

        bool Execute(){
            uint8_t Fetched = DMemory->LoadUINT8(DSource1->load() + DOffset);
            DDestination->store(Fetched & 0x80 ? 0xFFFFFF00 | Fetched : Fetched);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVLDTypeInstruction::ToString().replace(0,2,"lb");
        };
};

class CRISCVLHInstruction : public CRISCVLDTypeInstruction{
    public:
        CRISCVLHInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem)  : CRISCVLDTypeInstruction(addr,raw, pc, regs, mem){

        };

        bool Execute(){
            uint16_t Fetched = DMemory->LoadUINT16(DSource1->load() + DOffset);
            DDestination->store(Fetched & 0x8000 ? 0xFFFF0000 | Fetched : Fetched);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVLDTypeInstruction::ToString().replace(0,2,"lh");
        };
};

class CRISCVLWInstruction : public CRISCVLDTypeInstruction{
    public:
        CRISCVLWInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem)  : CRISCVLDTypeInstruction(addr,raw, pc, regs, mem){

        };

        bool Execute(){
            DDestination->store(DMemory->LoadUINT32(DSource1->load() + DOffset));
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVLDTypeInstruction::ToString().replace(0,2,"lw");
        };
};

class CRISCVLBUInstruction : public CRISCVLDTypeInstruction{
    public:
        CRISCVLBUInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem)  : CRISCVLDTypeInstruction(addr,raw, pc, regs, mem){

        };

        bool Execute(){
            DDestination->store(DMemory->LoadUINT8(DSource1->load() + DOffset));
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVLDTypeInstruction::ToString().replace(0,3,"lbu");
        };
};

class CRISCVLHUInstruction : public CRISCVLDTypeInstruction{
    public:
        CRISCVLHUInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem)  : CRISCVLDTypeInstruction(addr,raw, pc, regs, mem){

        };

        bool Execute(){
            DDestination->store(DMemory->LoadUINT16(DSource1->load() + DOffset));
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVLDTypeInstruction::ToString().replace(0,3,"lhu");
        };
};

CRISCVLDTypeInstruction::CRISCVLDTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem) : CInstruction(addr,raw){
    SRISCVIType *Encoded = (SRISCVIType *)&raw;
    DProgramCounter = pc;
    DDestination = regs[Encoded->rd];
    DSource1 = regs[Encoded->rs1];
    DMemory = mem;
    DOffset = Encoded->imm11_0 & 0x800 ? 0xFFFFF000 | Encoded->imm11_0 : Encoded->imm11_0;
}

std::string CRISCVLDTypeInstruction::ToString() const{
    SRISCVIType *Encoded = (SRISCVIType *)&DRawInstruction;
    return std::string("       ") + RegisterName(Encoded->rd) + "," + std::to_string(DOffset) + "(" + RegisterName(Encoded->rs1) + ")";
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVLDTypeInstruction::DecodeLDType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::shared_ptr< CMemoryDevice > mem){
    SRISCVIType *Encoded = (SRISCVIType *)&raw;

    switch(Encoded->funct3){
        case 0x0:   return std::make_shared<CRISCVLBInstruction>(addr,raw,pc,regs,mem);
        case 0x1:   return std::make_shared<CRISCVLHInstruction>(addr,raw,pc,regs,mem);
        case 0x2:   return std::make_shared<CRISCVLWInstruction>(addr,raw,pc,regs,mem);
        case 0x4:   return std::make_shared<CRISCVLBUInstruction>(addr,raw,pc,regs,mem);
        case 0x5:   return std::make_shared<CRISCVLHUInstruction>(addr,raw,pc,regs,mem);
        
        default:    return nullptr;
    }
}