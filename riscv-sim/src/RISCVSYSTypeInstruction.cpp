#include "RISCVSYSTypeInstruction.h"
#include <sstream>
#include <iomanip>

const std::unordered_map< uint32_t, std::string > CRISCVSYSTypeInstruction::DControlStatusRegisterNames = 
{   // Machine Information Registers
    {0xF11,"mvendorid"},
    {0xF12,"marchid"},
    {0xF13,"mimpid"},
    {0xF14,"mhartid"},
    //Machine Trap Setup
    {0x300,"mstatus"},
    {0x301,"misa"},
    {0x302,"medeleg"},
    {0x303,"mideleg"},
    {0x304,"mie"},
    {0x305,"mtvec"},
    {0x306,"mcounteren"},
    // Machine Trap Handling
    {0x340,"mscratch"},
    {0x341,"mepc"},
    {0x342,"mcause"},
    {0x343,"mtval"},
    {0x344,"mip"},
    // Machine Memory Protection
    {0x3A0,"pmpcfg0"}, {0x3A1,"pmpcfg1"}, {0x3A2,"pmpcfg2"}, {0x3A3,"pmpcfg3"},
    {0x3B0,"pmpaddr0"}, {0x3B1,"pmpaddr1"}, {0x3B2,"pmpaddr2"}, {0x3B3,"pmpaddr3"},
    {0x3B4,"pmpaddr4"}, {0x3B5,"pmpaddr5"}, {0x3B6,"pmpaddr6"}, {0x3B7,"pmpaddr7"},
    {0x3B8,"pmpaddr8"}, {0x3B9,"pmpaddr9"}, {0x3BA,"pmpaddr10"}, {0x3BB,"pmpaddr11"},
    {0x3BC,"pmpaddr12"}, {0x3BD,"pmpaddr13"}, {0x3BE,"pmpaddr14"}, {0x3BF,"pmpaddr15"},
    // Machine Counter/Timers
    {0xB00,"mcycle"},
    {0xB02,"minstret"},
    {0xB03,"mhpmcounter3"},
    {0xB04,"mhpmcounter4"},  {0xB05,"mhpmcounter5"},  {0xB06,"mhpmcounter6"},  {0xB07,"mhpmcounter7"},
    {0xB08,"mhpmcounter8"},  {0xB09,"mhpmcounter9"},  {0xB0A,"mhpmcounter10"}, {0xB0B,"mhpmcounter11"},
    {0xB0C,"mhpmcounter12"}, {0xB0D,"mhpmcounter13"}, {0xB0E,"mhpmcounter14"}, {0xB0F,"mhpmcounter15"},
    {0xB10,"mhpmcounter16"}, {0xB11,"mhpmcounter17"}, {0xB12,"mhpmcounter18"}, {0xB13,"mhpmcounter19"},
    {0xB14,"mhpmcounter20"}, {0xB15,"mhpmcounter21"}, {0xB16,"mhpmcounter22"}, {0xB17,"mhpmcounter23"},
    {0xB18,"mhpmcounter24"}, {0xB19,"mhpmcounter25"}, {0xB1A,"mhpmcounter26"}, {0xB1B,"mhpmcounter27"},
    {0xB1C,"mhpmcounter28"}, {0xB1D,"mhpmcounter29"}, {0xB1E,"mhpmcounter30"}, {0xB1F,"mhpmcounter31"},
    {0xB80,"mcycleh"},
    {0xB82,"minstreth"},
    {0xB83,"mhpmcounter3h"},
    {0xB84,"mhpmcounter4h"},  {0xB85,"mhpmcounter5h"},  {0xB86,"mhpmcounter6h"},  {0xB87,"mhpmcounter7h"},
    {0xB88,"mhpmcounter8h"},  {0xB89,"mhpmcounter9h"},  {0xB8A,"mhpmcounter10h"}, {0xB8B,"mhpmcounter11h"},
    {0xB8C,"mhpmcounter12h"}, {0xB8D,"mhpmcounter13h"}, {0xB8E,"mhpmcounter14h"}, {0xB8F,"mhpmcounter15h"},
    {0xB90,"mhpmcounter16h"}, {0xB91,"mhpmcounter17h"}, {0xB92,"mhpmcounter18h"}, {0xB93,"mhpmcounter19h"},
    {0xB94,"mhpmcounter20h"}, {0xB95,"mhpmcounter21h"}, {0xB96,"mhpmcounter22h"}, {0xB97,"mhpmcounter23h"},
    {0xB98,"mhpmcounter24h"}, {0xB99,"mhpmcounter25h"}, {0xB9A,"mhpmcounter26h"}, {0xB9B,"mhpmcounter27h"},
    {0xB9C,"mhpmcounter28h"}, {0xB9D,"mhpmcounter29h"}, {0xB9E,"mhpmcounter30h"}, {0xB9F,"mhpmcounter31h"},
    // Machine Counter Setup
    {0x320,"mcountinhibit"},
    {0x323,"mhpmevent3"},
    {0x324,"mhpmevent4"},  {0x325,"mhpmevent5"},  {0x326,"mhpmevent6"},  {0x327,"mhpmevent7"},
    {0x328,"mhpmevent8"},  {0x329,"mhpmevent9"},  {0x32A,"mhpmevent10"}, {0x32B,"mhpmevent11"},
    {0x32C,"mhpmevent12"}, {0x32D,"mhpmevent13"}, {0x32E,"mhpmevent14"}, {0x32F,"mhpmevent15"},
    {0x330,"mhpmevent16"}, {0x331,"mhpmevent17"}, {0x332,"mhpmevent18"}, {0x333,"mhpmevent19"},
    {0x334,"mhpmevent20"}, {0x335,"mhpmevent21"}, {0x336,"mhpmevent22"}, {0x337,"mhpmevent23"},
    {0x338,"mhpmevent24"}, {0x339,"mhpmevent25"}, {0x33A,"mhpmevent26"}, {0x33B,"mhpmevent27"},
    {0x33C,"mhpmevent28"}, {0x33D,"mhpmevent29"}, {0x33E,"mhpmevent30"}, {0x33F,"mhpmevent31"},

};

class CRISCVCSRRWInstruction : public CRISCVSYSTypeInstruction{
    public:
        CRISCVCSRRWInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs) : CRISCVSYSTypeInstruction(addr,raw, pc, regs, csrregs){

        };

        bool Execute(){
            if(DDestination){
                DDestination->store(DControlStatusRegister->exchange(DSource1->load()));
            }
            else{
                DControlStatusRegister->store(DSource1->load());
            }
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            SRISCVCSRType *Encoded = (SRISCVCSRType *)&DRawInstruction;
            if(!DDestination){
                return std::string("csrw   ") + ControlStatusRegisterName(Encoded->csr) + "," + RegisterName(Encoded->rs1_uimm);
            }
            return CRISCVSYSTypeInstruction::ToString().replace(0,5,"csrrw");
        };
};

class CRISCVCSRRSInstruction : public CRISCVSYSTypeInstruction{
    public:
        CRISCVCSRRSInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs) : CRISCVSYSTypeInstruction(addr,raw, pc, regs, csrregs){

        };

        bool Execute(){
            if(DSource1){
                DDestination->store(DControlStatusRegister->fetch_or(DSource1->load()));
            }
            else{
                DDestination->store(DControlStatusRegister->load());
            }
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            SRISCVCSRType *Encoded = (SRISCVCSRType *)&DRawInstruction;
            if(!DSource1){
                return std::string("csrr   ") + RegisterName(Encoded->rd) + "," + ControlStatusRegisterName(Encoded->csr);
            }
            if(!Encoded->rd){
                std::stringstream TempStream;
                TempStream<<"0x"<<std::setfill('0')<<std::setw(5)<<std::right<<std::hex<<DImmediate;
                return std::string("csrs   ") + ControlStatusRegisterName(Encoded->csr) + "," + TempStream.str();
            }
            return CRISCVSYSTypeInstruction::ToString().replace(0,5,"csrrs");
        };
};

class CRISCVCSRRCInstruction : public CRISCVSYSTypeInstruction{
    public:
        CRISCVCSRRCInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs) : CRISCVSYSTypeInstruction(addr,raw, pc, regs, csrregs){

        };

        bool Execute(){
            if(DSource1){
                DDestination->store(DControlStatusRegister->fetch_and(~DSource1->load()));
            }
            else{
                DDestination->store(DControlStatusRegister->load());
            }
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            SRISCVCSRType *Encoded = (SRISCVCSRType *)&DRawInstruction;
            if(!DSource1){
                return std::string("csrr   ") + RegisterName(Encoded->rd) + "," + ControlStatusRegisterName(Encoded->csr);
            }
            if(!Encoded->rd){
                std::stringstream TempStream;
                TempStream<<"0x"<<std::setfill('0')<<std::setw(5)<<std::right<<std::hex<<DImmediate;
                return std::string("csrc   ") + ControlStatusRegisterName(Encoded->csr) + "," + TempStream.str();
            }
            return CRISCVSYSTypeInstruction::ToString().replace(0,5,"csrrc");
        };
};

class CRISCVCSRRWIInstruction : public CRISCVSYSTypeInstruction{
    public:
        CRISCVCSRRWIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs) : CRISCVSYSTypeInstruction(addr,raw, pc, regs, csrregs){

        };

        bool Execute(){
            if(DDestination){
                DDestination->store(DControlStatusRegister->exchange(DImmediate));
            }
            else{
                DControlStatusRegister->store(DSource1->load());
            }
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            SRISCVCSRType *Encoded = (SRISCVCSRType *)&DRawInstruction;
            if(!DDestination){
                std::stringstream TempStream;
                TempStream<<"0x"<<std::setfill('0')<<std::setw(5)<<std::right<<std::hex<<DImmediate;
                return std::string("csrwi  ") + ControlStatusRegisterName(Encoded->csr) + "," + TempStream.str();
            }
            return CRISCVSYSTypeInstruction::ToString().replace(0,6,"csrrwi");
        };
};

class CRISCVCSRRSIInstruction : public CRISCVSYSTypeInstruction{
    public:
        CRISCVCSRRSIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs) : CRISCVSYSTypeInstruction(addr,raw, pc, regs, csrregs){

        };

        bool Execute(){
            if(DImmediate){
                DDestination->store(DControlStatusRegister->fetch_or(DImmediate));
            }
            else{
                DDestination->store(DControlStatusRegister->load());
            }
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            SRISCVCSRType *Encoded = (SRISCVCSRType *)&DRawInstruction;
            if(!Encoded->rd){
                std::stringstream TempStream;
                TempStream<<"0x"<<std::setfill('0')<<std::setw(5)<<std::right<<std::hex<<DImmediate;
                return std::string("csrsi  ") + ControlStatusRegisterName(Encoded->csr) + "," + TempStream.str();
            }
            return CRISCVSYSTypeInstruction::ToString().replace(0,6,"csrrsi");
        };
};

class CRISCVCSRRCIInstruction : public CRISCVSYSTypeInstruction{
    public:
        CRISCVCSRRCIInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs) : CRISCVSYSTypeInstruction(addr,raw, pc, regs, csrregs){

        };

        bool Execute(){
            if(DImmediate){
                DDestination->store(DControlStatusRegister->fetch_and(~DImmediate));
            }
            else{
                DDestination->store(DControlStatusRegister->load());
            }
            DProgramCounter->fetch_add(DInstructionAlignment);
            return true;
        };

        std::string ToString() const{
            SRISCVCSRType *Encoded = (SRISCVCSRType *)&DRawInstruction;
            if(!Encoded->rd){
                std::stringstream TempStream;
                TempStream<<"0x"<<std::setfill('0')<<std::setw(5)<<std::right<<std::hex<<DImmediate;
                return std::string("csrci  ") + ControlStatusRegisterName(Encoded->csr) + "," + TempStream.str();
            }
            return CRISCVSYSTypeInstruction::ToString().replace(0,6,"csrrci");
        };
};

class CRISCVECALLInstruction : public CRISCVSYSTypeInstruction{
    public:
        CRISCVECALLInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs) : CRISCVSYSTypeInstruction(addr,raw, pc, regs, csrregs){
            DDestination = csrregs[0x341]; // mepc
            DSource1 = csrregs[0x305]; // mvtec
            DMachineStatus = csrregs[0x300]; // mstatus
        };

        bool Execute(){
            DDestination->store(DProgramCounter->load());
            uint32_t TargetAddress = DSource1->load();
            uint32_t MStatusOriginal = DMachineStatus->load();
            // Check vectored or not 43 is 11 * 4 - 1 since 1 there for vectored
            TargetAddress += TargetAddress & 0x1 ? 43 : 0;  
            DControlStatusRegister->store(11);
            DMachineStatus->store((MStatusOriginal & 0x1800) | ((MStatusOriginal & 0x8)<<4)); // move MIE into MPIE
            DProgramCounter->store(TargetAddress);
            return true;
        };

        std::string ToString() const{
            return std::string("ecall  ");
        };
};

class CRISCVEBREAKInstruction : public CRISCVSYSTypeInstruction{
    public:
        CRISCVEBREAKInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs) : CRISCVSYSTypeInstruction(addr,raw, pc, regs, csrregs){

        };

        bool Execute(){
            DProgramCounter->fetch_add(DInstructionAlignment); // Ignore the instruction
            return true;
        };

        std::string ToString() const{
            return std::string("ebreak ");
        };
};

class CRISCVMRETInstruction : public CRISCVSYSTypeInstruction{
    public:
        CRISCVMRETInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs) : CRISCVSYSTypeInstruction(addr,raw, pc, regs, csrregs){
            DSource1 = csrregs[0x341]; // mepc
            DMachineStatus = csrregs[0x300]; // mstatus
        };

        bool Execute(){
            uint32_t MStatusOriginal = DMachineStatus->load();
            DMachineStatus->store((MStatusOriginal & 0x1880) | ((MStatusOriginal & 0x80)>>4)); // move MPIE into MIE
            DProgramCounter->store(DSource1->load());
            return true;
        };

        std::string ToString() const{
            return std::string("mret   ");
        };
};


CRISCVSYSTypeInstruction::CRISCVSYSTypeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs) : CInstruction(addr,raw){
    SRISCVCSRType *Encoded = (SRISCVCSRType *)&raw;
    DProgramCounter = pc;
    // If not x0 or if RS[I]/RC[I] type instructions
    DDestination = Encoded->rd || (Encoded->funct3 & 0x2) ? regs[Encoded->rd] : nullptr;
    if(Encoded->funct3 & 0x4){
        DImmediate = Encoded->rs1_uimm;
    }
    else{
        DImmediate = 0;
        if(Encoded->rs1_uimm || Encoded->funct3 == 0x1){
            DSource1 = regs[Encoded->rs1_uimm];
        }
    }
    auto CSRSearch = csrregs.find(Encoded->csr);
    if(CSRSearch == csrregs.end()){
        if(Encoded->funct3 || Encoded->csr){
            // Hardwire to zero 
            DControlStatusRegister = regs[0];    
        }
        else{
            // Set ECALL CSR to mcause
            DControlStatusRegister = csrregs[0x342];
        }
    }
    else{
        DControlStatusRegister = CSRSearch->second;
    }
}

std::string CRISCVSYSTypeInstruction::ToString() const{
    SRISCVCSRType *Encoded = (SRISCVCSRType *)&DRawInstruction;
    std::stringstream TempStream;
    TempStream<<"0x"<<std::setfill('0')<<std::setw(5)<<std::right<<std::hex<<DImmediate;
    return std::string("       ") + RegisterName(Encoded->rd) + "," + ControlStatusRegisterName(Encoded->csr) + "," + (Encoded->funct3 & 0x4 ? TempStream.str() : RegisterName(Encoded->rs1_uimm));
}

std::string CRISCVSYSTypeInstruction::ControlStatusRegisterName(uint32_t index){
    auto NameSearch = DControlStatusRegisterNames.find(index);
    if(DControlStatusRegisterNames.end() != NameSearch){
        return NameSearch->second;
    }
    std::stringstream TempStream;
    TempStream<<"0x"<<std::setfill('0')<<std::setw(5)<<std::right<<std::hex<<uint32_t(index & 0xFFF);
    return TempStream.str();
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVSYSTypeInstruction::DecodeSYSType(uint32_t addr, uint32_t raw, std::shared_ptr< CHardwareRegister< uint32_t > > pc, std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > &regs, std::unordered_map<uint32_t, std::shared_ptr< CHardwareRegister< uint32_t > > > &csrregs){
    SRISCVCSRType *Encoded = (SRISCVCSRType *)&raw;

    switch(Encoded->funct3){
        case 0x0:   switch(Encoded->csr){
                        case 0x000: return std::make_shared<CRISCVECALLInstruction>(addr,raw,pc,regs,csrregs);
                        case 0x001: return std::make_shared<CRISCVEBREAKInstruction>(addr,raw,pc,regs,csrregs);
                        case 0x302: return std::make_shared<CRISCVMRETInstruction>(addr,raw,pc,regs,csrregs);
                        default:    return nullptr;
                    }
                    break;
        case 0x1:   return std::make_shared<CRISCVCSRRWInstruction>(addr,raw,pc,regs,csrregs);
        case 0x2:   return std::make_shared<CRISCVCSRRSInstruction>(addr,raw,pc,regs,csrregs);
        case 0x3:   return std::make_shared<CRISCVCSRRCInstruction>(addr,raw,pc,regs,csrregs);
        case 0x5:   return std::make_shared<CRISCVCSRRWIInstruction>(addr,raw,pc,regs,csrregs);
        case 0x6:   return std::make_shared<CRISCVCSRRSIInstruction>(addr,raw,pc,regs,csrregs);
        case 0x7:   return std::make_shared<CRISCVCSRRCIInstruction>(addr,raw,pc,regs,csrregs);
        
        default:    return nullptr;
    }
}
