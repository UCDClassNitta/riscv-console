#include "RISCVUTypeInstruction.h"
#include <iomanip>
#include <sstream>

class CRISCVLUIInstruction : public CRISCVUTypeInstruction{
    public:
        CRISCVLUIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVUTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DOffset);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVUTypeInstruction::ToString().replace(0,3,"lui");
        };
};

class CRISCVAUIPCInstruction : public CRISCVUTypeInstruction{
    public:
        CRISCVAUIPCInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CRISCVUTypeInstruction(addr,raw, pc, regs){

        };

        bool Execute(){
            DDestination->store(DProgramCounter->load() + DOffset);
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            return CRISCVUTypeInstruction::ToString().replace(0,5,"auipc");
        };
};

CRISCVUTypeInstruction::CRISCVUTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs) : CInstruction(addr,raw){
    SRISCVUType *Encoded = (SRISCVUType *)&raw;
    DProgramCounter = pc;
    DDestination = regs[Encoded->rd];
    DOffset = uint32_t(Encoded->imm31_12)<<12;
};

std::string CRISCVUTypeInstruction::ToString() const{
    SRISCVUType *Encoded = (SRISCVUType *)&DRawInstruction;
    std::stringstream TempStream;
    TempStream<<std::setfill('0')<<std::setw(5)<<std::right<<std::hex<<uint32_t(Encoded->imm31_12);
    return std::string("       ") + RegisterName(Encoded->rd) + ",0x" + TempStream.str();
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVUTypeInstruction::DecodeUType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs){
    SRISCVUType *Encoded = (SRISCVUType *)&raw;

    switch(Encoded->opcode){
        case 0x37:  return std::make_shared<CRISCVLUIInstruction>(addr,raw,pc,regs);
        case 0x17:  return std::make_shared<CRISCVAUIPCInstruction>(addr,raw,pc,regs);
        default:    return nullptr;
    }
}