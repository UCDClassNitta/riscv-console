#include "RISCVRTypeInstruction.h"

class CRISCVADDInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVADDInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() + DSource2->load());
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"add");
        };
};

class CRISCVSUBInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVSUBInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() - DSource2->load());
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"sub");
        };
};

class CRISCVSLLInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVSLLInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() << (DSource2->load() & 0x1F));
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"sll");
        };
};

class CRISCVSRLInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVSRLInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() >> (DSource2->load() & 0x1F));
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"srl");
        };
};

class CRISCVSRAInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVSRAInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            int32_t Temp;
            *(uint32_t *)&Temp = DSource1->load();
            Temp >>= (DSource2->load() & 0x1F);
            DDestination->store(*(uint32_t *)&Temp);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"sra");
        };
};

class CRISCVSLTInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVSLTInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            int32_t Temp1, Temp2;
            *(uint32_t *)&Temp1 = DSource1->load();
            *(uint32_t *)&Temp2 = DSource2->load();
            DDestination->store(Temp1 < Temp2 ? 1 : 0);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"slt");
        };
};

class CRISCVSLTUInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVSLTUInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() < DSource2->load() ? 1 : 0);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,4,"sltu");
        };
};

class CRISCVANDInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVANDInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() & DSource2->load());
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"and");
        };
};

class CRISCVORInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVORInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() | DSource2->load());
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,2,"or");
        };
};

class CRISCVXORInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVXORInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() ^ DSource2->load());
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"xor");
        };
};

class CRISCVMULInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVMULInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DSource1->load() * DSource2->load());
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"mul");
        };
};

class CRISCVMULHInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVMULHInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            int32_t Temp1, Temp2;
            *(uint32_t *)&Temp1 = DSource1->load();
            *(uint32_t *)&Temp2 = DSource2->load();
            DDestination->store(((int64_t)Temp1 * Temp2)>>32);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,4,"mulh");
        };
};

class CRISCVMULHSUInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVMULHSUInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            int32_t Temp1;
            *(uint32_t *)&Temp1 = DSource1->load();

            DDestination->store(((int64_t)Temp1 * DSource2->load())>>32);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,6,"mulhsu");
        };
};

class CRISCVDIVInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVDIVInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            int32_t Temp1, Temp2;
            *(uint32_t *)&Temp1 = DSource1->load();
            *(uint32_t *)&Temp2 = DSource2->load();
            DDestination->store(Temp2 ? Temp1 / Temp2 : -1);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"div");
        };
};

class CRISCVDIVUInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVDIVUInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            uint32_t Temp2 = DSource2->load();
            DDestination->store(Temp2 ? DSource1->load() / Temp2 : -1);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,4,"divu");
        };
};

class CRISCVREMInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVREMInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            int32_t Temp1, Temp2;
            *(uint32_t *)&Temp1 = DSource1->load();
            *(uint32_t *)&Temp2 = DSource2->load();
            DDestination->store(Temp2 ? Temp1 % Temp2 : Temp1);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,3,"rem");
        };
};

class CRISCVREMUInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVREMUInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            uint32_t Temp1 = DSource1->load();
            uint32_t Temp2 = DSource2->load();
            DDestination->store(Temp2 ? Temp1 % Temp2 : Temp1);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,4,"remu");
        };
};

class CRISCVMULHUInstruction : public CRISCVRTypeInstruction{
    public:
        CRISCVMULHUInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVRTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(((uint64_t)DSource1->load() * DSource2->load())>>32);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;   
        };

        std::string ToString() const{
            return CRISCVRTypeInstruction::ToString().replace(0,5,"mulhu");
        };
};

CRISCVRTypeInstruction::CRISCVRTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CInstruction(addr,raw){
    SRISCVRType *Encoded = (SRISCVRType *)&raw;
    DProgramCounter = pc;
    DDestination = regs[Encoded->rd];
    DSource1 = regs[Encoded->rs1];
    DSource2 = regs[Encoded->rs2];
}

std::string CRISCVRTypeInstruction::ToString() const{
    SRISCVRType *Encoded = (SRISCVRType *)&DRawInstruction;
    return std::string("       ") + RegisterName(Encoded->rd) + "," + RegisterName(Encoded->rs1) + "," + RegisterName(Encoded->rs2);
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVRTypeInstruction::DecodeRType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs){
    SRISCVRType *Encoded = (SRISCVRType *)&raw;
    uint32_t Funct10 = (((uint32_t)Encoded->funct7)<<3) | Encoded->funct3;

    switch(Funct10){
        case 0x000: return std::make_shared<CRISCVADDInstruction>(addr,raw,pc,regs);
        case 0x100: return std::make_shared<CRISCVSUBInstruction>(addr,raw,pc,regs);
        case 0x001: return std::make_shared<CRISCVSLLInstruction>(addr,raw,pc,regs);
        case 0x002: return std::make_shared<CRISCVSLTInstruction>(addr,raw,pc,regs);
        case 0x003: return std::make_shared<CRISCVSLTUInstruction>(addr,raw,pc,regs);
        case 0x004: return std::make_shared<CRISCVXORInstruction>(addr,raw,pc,regs);
        case 0x005: return std::make_shared<CRISCVSRLInstruction>(addr,raw,pc,regs);
        case 0x105: return std::make_shared<CRISCVSRAInstruction>(addr,raw,pc,regs);
        case 0x006: return std::make_shared<CRISCVORInstruction>(addr,raw,pc,regs);
        case 0x007: return std::make_shared<CRISCVANDInstruction>(addr,raw,pc,regs);
        case 0x008: return std::make_shared<CRISCVMULInstruction>(addr,raw,pc,regs);
        case 0x009: return std::make_shared<CRISCVMULHInstruction>(addr,raw,pc,regs);
        case 0x00A: return std::make_shared<CRISCVMULHSUInstruction>(addr,raw,pc,regs);
        case 0x00B: return std::make_shared<CRISCVMULHUInstruction>(addr,raw,pc,regs);
        case 0x00C: return std::make_shared<CRISCVDIVInstruction>(addr,raw,pc,regs);
        case 0x00D: return std::make_shared<CRISCVDIVUInstruction>(addr,raw,pc,regs);
        case 0x00E: return std::make_shared<CRISCVREMInstruction>(addr,raw,pc,regs);
        case 0x00F: return std::make_shared<CRISCVREMUInstruction>(addr,raw,pc,regs);
        
        default:    return nullptr;
    }
}