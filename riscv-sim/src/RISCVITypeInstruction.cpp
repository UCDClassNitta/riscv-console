#include "RISCVITypeInstruction.h"

class CRISCVADDIInstruction : public CRISCVITypeInstruction{
    public:
        CRISCVADDIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVITypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            int32_t Temp1;
            *(uint32_t *)&Temp1 = DSource1->load();
            DDestination->store(Temp1 + DOffset);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            SRISCVIType *Encoded = (SRISCVIType *)&DRawInstruction;
            if(!Encoded->rs1){
                if(!DOffset && !Encoded->rd){
                    return std::string("nop    ");
                }
                return std::string("li     ") + RegisterName(Encoded->rd) + "," + std::to_string(DOffset);
            }
            else if(!DOffset){
                 return std::string("mv     ") + RegisterName(Encoded->rd) + "," + RegisterName(Encoded->rs1);
            }
            return CRISCVITypeInstruction::ToString().replace(0,4,"addi");
        };
};

class CRISCVSLTIInstruction : public CRISCVITypeInstruction{
    public:
        CRISCVSLTIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVITypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            int32_t Temp1;
            *(uint32_t *)&Temp1 = DSource1->load();
            DDestination->store(Temp1 < DOffset ? 1 : 0);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVITypeInstruction::ToString().replace(0,4,"slti");
        };
};

class CRISCVSLTIUInstruction : public CRISCVITypeInstruction{
    public:
        CRISCVSLTIUInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVITypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() < *(uint32_t*)&DOffset ? 1 : 0);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            if(DOffset == 1){
                SRISCVIType *Encoded = (SRISCVIType *)&DRawInstruction;
                return std::string("seqz   ") + RegisterName(Encoded->rd) + "," + RegisterName(Encoded->rs1);
            }
            return CRISCVITypeInstruction::ToString().replace(0,5,"sltiu");
        };
};

class CRISCVXORIInstruction : public CRISCVITypeInstruction{
    public:
        CRISCVXORIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVITypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() ^ (*(uint32_t*)&DOffset));
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            if(DOffset == -1){
                SRISCVIType *Encoded = (SRISCVIType *)&DRawInstruction;
                return std::string("not    ") + RegisterName(Encoded->rd) + "," + RegisterName(Encoded->rs1);
            }
            return CRISCVITypeInstruction::ToString().replace(0,4,"xori");
        };
};

class CRISCVORIInstruction : public CRISCVITypeInstruction{
    public:
        CRISCVORIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVITypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() | (*(uint32_t*)&DOffset));
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVITypeInstruction::ToString().replace(0,3,"ori");
        };
};

class CRISCVANDIInstruction : public CRISCVITypeInstruction{
    public:
        CRISCVANDIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVITypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() & (*(uint32_t*)&DOffset));
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVITypeInstruction::ToString().replace(0,4,"andi");
        };
};

class CRISCVSLLIInstruction : public CRISCVITypeInstruction{
    public:
        CRISCVSLLIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVITypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() << DOffset);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVITypeInstruction::ToString().replace(0,4,"slli");
        };
};

class CRISCVSRLIInstruction : public CRISCVITypeInstruction{
    public:
        CRISCVSRLIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVITypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() >> DOffset);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVITypeInstruction::ToString().replace(0,4,"srli");
        };
};

class CRISCVSRAIInstruction : public CRISCVITypeInstruction{
    public:
        CRISCVSRAIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVITypeInstruction(addr,raw, pc, regs){
            DOffset &= 0x1F;
        };

        bool Execute(){
            int32_t Temp;
            *(uint32_t *)&Temp = DSource1->load();
            DDestination->store(Temp >> DOffset);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVITypeInstruction::ToString().replace(0,4,"srai");
        };
};

CRISCVITypeInstruction::CRISCVITypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CInstruction(addr,raw){
    SRISCVIType *Encoded = (SRISCVIType *)&raw;
    DProgramCounter = pc;
    DDestination = regs[Encoded->rd];
    DSource1 = regs[Encoded->rs1];
    DOffset = Encoded->imm11_0 & 0x800 ? 0xFFFFF000 | Encoded->imm11_0 : Encoded->imm11_0;
}

std::string CRISCVITypeInstruction::ToString() const{
    SRISCVIType *Encoded = (SRISCVIType *)&DRawInstruction;
    return std::string("       ") + RegisterName(Encoded->rd) + "," + RegisterName(Encoded->rs1) + "," + std::to_string(DOffset);
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVITypeInstruction::DecodeIType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs){
    SRISCVIType *Encoded = (SRISCVIType *)&raw;

    switch(Encoded->funct3){
        case 0x0:   return std::make_shared<CRISCVADDIInstruction>(addr,raw,pc,regs);
        case 0x1:   return std::make_shared<CRISCVSLLIInstruction>(addr,raw,pc,regs);
        case 0x2:   return std::make_shared<CRISCVSLTIInstruction>(addr,raw,pc,regs);
        case 0x3:   return std::make_shared<CRISCVSLTIUInstruction>(addr,raw,pc,regs);
        case 0x4:   return std::make_shared<CRISCVXORIInstruction>(addr,raw,pc,regs);
        case 0x5:   if(Encoded->imm11_0 & 0x400){
                        return std::make_shared<CRISCVSRAIInstruction>(addr,raw,pc,regs);
                    }
                    else{
                        return std::make_shared<CRISCVSRLIInstruction>(addr,raw,pc,regs);
                    }
        case 0x6:   return std::make_shared<CRISCVORIInstruction>(addr,raw,pc,regs);
        case 0x7:   return std::make_shared<CRISCVANDIInstruction>(addr,raw,pc,regs);
        
        default:    return nullptr;
    }
}
