#include "RISCVCPU.h"
#include "RISCVRTypeInstruction.h"
#include "RISCVITypeInstruction.h"
#include "RISCVLTypeInstruction.h"
#include "RISCVSTypeInstruction.h"
#include "RISCVBTypeInstruction.h"
#include "RISCVUTypeInstruction.h"
#include "RISCVJTypeInstruction.h"
#include "RISCVSYSTypeInstruction.h"
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <algorithm>

// Code form Effective Modern C++ by Scott Meyers (see Item 10)
template<typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E enumerator) noexcept{
    return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

const size_t CRISCVCPU::DRegisterCount = 16;

const std::string CRISCVCPU::CInstruction::DRegisterNames[] = {"zero","ra", "sp", "gp",
                                                                "tp", "t0", "t1", "t2",
                                                                "s0", "s1", "a0", "a1",
                                                                "a2", "a3", "a4", "a5",
                                                                };
const uint32_t CRISCVCPU::CInstruction::DInstructionAlignment = 4;

#pragma pack(1)
struct SRISCVUnknownType{
    uint32_t opcode : 7;
    uint32_t unknown : 25;
};
#pragma pack()

std::string CRISCVCPU::CInstruction::RegisterName(uint32_t index){
    return DRegisterNames[index % CRISCVCPU::DRegisterCount];
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVCPU::CInstruction::DecodeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CRISCVCPU > cpu){
    SRISCVUnknownType *Encoded = ((SRISCVUnknownType *)&raw);

    switch(Encoded->opcode){
        case 0x03:  return CRISCVLDTypeInstruction::DecodeLDType(addr,raw,cpu->DProgramCounter,cpu->DRegisters,cpu->DMemory);
        case 0x13:  return CRISCVITypeInstruction::DecodeIType(addr,raw,cpu->DProgramCounter,cpu->DRegisters);
        case 0x23:  return CRISCVSTypeInstruction::DecodeSType(addr,raw,cpu->DProgramCounter,cpu->DRegisters,cpu->DMemory);
        case 0x33:  return CRISCVRTypeInstruction::DecodeRType(addr,raw,cpu->DProgramCounter,cpu->DRegisters);
        case 0x17:
        case 0x37:  return CRISCVUTypeInstruction::DecodeUType(addr,raw,cpu->DProgramCounter,cpu->DRegisters);
        case 0x63:  return CRISCVBTypeInstruction::DecodeBType(addr,raw,cpu->DProgramCounter,cpu->DRegisters);
        case 0x67:
        case 0x6F:  return CRISCVJTypeInstruction::DecodeJType(addr,raw,cpu->DProgramCounter,cpu->DRegisters);
        case 0x73:  return CRISCVSYSTypeInstruction::DecodeSYSType(addr,raw,cpu->DProgramCounter,cpu->DRegisters,cpu->DControlStatusRegisters);
        default:    return nullptr;
    }
}

class CNullInstructionCache : public CRISCVCPU::CInstructionCache{
    public:
        virtual ~CNullInstructionCache(){};
        std::shared_ptr< CRISCVCPU::CInstruction > Fetch(uint32_t addr){
            return nullptr;
        };
        void Insert(std::shared_ptr< CRISCVCPU::CInstruction > inst){};
        void Reset(){};
        void FlushRange(uint32_t addr, uint32_t size){};
};

CRISCVCPU::CRISCVCPU(std::shared_ptr< CMemoryDevice > memory, std::shared_ptr< CInstructionCache > icache){
    DProgramCounter = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(0);
    DRegisters.push_back(std::make_shared< CHardwareRegister< uint32_t > >());
    while(DRegisters.size() < DRegisterCount){
        DRegisters.push_back(std::make_shared< CReadWriteHardwareRegister< uint32_t > >(0));
    }
    DMemory = memory;
    DInstructionCache = icache ? icache : std::make_shared< CNullInstructionCache >();
    DCycleRegister = std::make_shared< CReadWriteHardwareRegister< uint64_t > >(0);
    DRetiredInstructionRegister = std::make_shared< CReadWriteHardwareRegister< uint64_t > >(0);

    DControlStatusRegisters[0xF11] = std::make_shared< CReadOnlyHardwareRegister< uint32_t > >(0); // mvendorid
    DControlStatusRegisterNames[0xF11] = "mvendorid";
    DControlStatusRegisters[0xF12] = std::make_shared< CReadOnlyHardwareRegister< uint32_t > >(0); // marchid
    DControlStatusRegisterNames[0xF12] = "marchid";
    DControlStatusRegisters[0xF13] = std::make_shared< CReadOnlyHardwareRegister< uint32_t > >(0); // mimpid
    DControlStatusRegisterNames[0xF13] = "mimpid";
    DControlStatusRegisters[0xF14] = std::make_shared< CReadOnlyHardwareRegister< uint32_t > >(0); // mhartid
    DControlStatusRegisterNames[0xF14] = "mhartid";

    DControlStatusRegisters[0x300] = DMachineStatusRegister = std::make_shared< CMaskedReadWriteHardwareRegister< uint32_t > >(0x1800,0x1888); // mstatus
    DControlStatusRegisterNames[0x300] = "mstatus";
    DControlStatusRegisters[0x301] = std::make_shared< CReadOnlyHardwareRegister< uint32_t > >(0x40001010); // misa
    DControlStatusRegisterNames[0x301] = "misa";
    // medeleg and mideleg shouldn't exist since only machine mode.
    //DControlStatusRegisters[0x302] = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(); // medeleg
    //DControlStatusRegisters[0x303] = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(); // mideleg
    DControlStatusRegisters[0x304] = DMachineInterruptEnableRegister = std::make_shared< CMaskedReadWriteHardwareRegister< uint32_t > >(0,0x888); // mie
    DControlStatusRegisterNames[0x304] = "mie";
    DControlStatusRegisters[0x305] = DMachineTrapVectorRegister = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(); // mtvec
    DControlStatusRegisterNames[0x305] = "mtvec";
    DControlStatusRegisters[0x306] = std::make_shared< CReadOnlyHardwareRegister< uint32_t > >(0x7); // mcounteren
    DControlStatusRegisterNames[0x306] = "mcounteren";

    DControlStatusRegisters[0x340] = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(); // mscratch
    DControlStatusRegisterNames[0x340] = "mscratch";
    DControlStatusRegisters[0x341] = DMachineExceptionProgramCounterRegister = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(); // mepc
    DControlStatusRegisterNames[0x341] = "mepc";
    DControlStatusRegisters[0x342] = DMachineCauseRegister = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(); // mcause
    DControlStatusRegisterNames[0x342] = "mcause";
    DControlStatusRegisters[0x343] = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(); // mtval
    DControlStatusRegisterNames[0x343] = "mtval";
    DControlStatusRegisters[0x344] = DMachineInterruptPendingRegister = std::make_shared< CMaskedReadWriteHardwareRegister< uint32_t > >(0,0x888); // mip
    DControlStatusRegisterNames[0x344] = "mip";
    
    DControlStatusRegisters[0xB00] = std::make_shared< CReadWriteHardwareRegisterLow< uint32_t, uint64_t > >(*DCycleRegister.get());
    DControlStatusRegisterNames[0xB00] = "mcycle";
    DControlStatusRegisters[0xB80] = std::make_shared< CReadWriteHardwareRegisterHigh< uint32_t, uint64_t > >(*DCycleRegister.get());
    DControlStatusRegisterNames[0xB80] = "mcycleh";
    DControlStatusRegisters[0xB02] = std::make_shared< CReadWriteHardwareRegisterLow< uint32_t, uint64_t > >(*DRetiredInstructionRegister.get());
    DControlStatusRegisterNames[0xB02] = "minstret";
    DControlStatusRegisters[0xB82] = std::make_shared< CReadWriteHardwareRegisterHigh< uint32_t, uint64_t > >(*DRetiredInstructionRegister.get());
    DControlStatusRegisterNames[0xB82] = "minstreth";

    for(auto &Item : DControlStatusRegisters){
        DControlStatusRegisterKeys.push_back(Item.first);
    }
    std::sort(DControlStatusRegisterKeys.begin(),DControlStatusRegisterKeys.end());

}

uint32_t CRISCVCPU::ProgramCounter() const{
    return DProgramCounter->load();
}

uint32_t CRISCVCPU::ProgramCounter(uint32_t newpc){
    DProgramCounter->store(newpc);
    return DProgramCounter->load();    
}

uint32_t CRISCVCPU::Register(uint32_t index) const{
    if(index < DRegisterCount){
        return DRegisters[index]->load();
    }
    return 0;
}

uint64_t CRISCVCPU::RetiredInstructionCount() const{
    return DRetiredInstructionRegister->load();
}

void CRISCVCPU::CheckInterrupts(){
    if(auto Pending = DMachineInterruptPendingRegister->load()){
        Pending &= DMachineInterruptEnableRegister->load();
        if(Pending){
            if(DMachineStatusRegister->load() & 0x8){ // MIE is set
                uint32_t Cause = 0;
                if(Pending & to_underlying(EInterruptSource::External)){
                    Cause = 11;
                }
                else if(Pending & to_underlying(EInterruptSource::Timer)){
                    Cause = 7;
                }
                else if(Pending & to_underlying(EInterruptSource::Software)){
                    Cause = 3;
                }
                InvokeTrap(Cause,true);
            }
        }
    }
}

void CRISCVCPU::InvokeTrap(uint32_t cause, bool interrupt){
    uint32_t TargetAddress = DMachineTrapVectorRegister->load();
    uint32_t MStatusOriginal = DMachineStatusRegister->load();
    // Check vectored or not 43 is 11 * 4 - 1 since 1 there for vectored
    TargetAddress += TargetAddress & 0x1 ? cause * 4 - 1 : 0;  
    DMachineCauseRegister->store(cause |(interrupt ? 0x80000000 : 0));
    DMachineStatusRegister->store((MStatusOriginal & 0x1800) | ((MStatusOriginal & 0x8)<<4)); // move MIE into MPIE
    DMachineExceptionProgramCounterRegister->store(DProgramCounter->load());
    DProgramCounter->store(TargetAddress);
}

bool CRISCVCPU::ExecuteInstruction(){
    auto NextInstructionAddress = DProgramCounter->load();
    if(NextInstructionAddress & 0x3){
        // Instruction address misaligne cause 0
        InvokeTrap(0, false); 
        return false;
    }
    std::shared_ptr< CInstruction > NextInstruction;
    try{
        NextInstruction = DInstructionCache->Fetch(NextInstructionAddress);
        if(!NextInstruction){
            NextInstruction = DecodeInstruction(DProgramCounter->load());
            if(!NextInstruction){
                // Illegal instruction cause 2
                InvokeTrap(2,false);
                return false;
            }
            DInstructionCache->Insert(NextInstruction);
        }
        if(NextInstruction->Execute()){
            DCycleRegister->operator++();
            DRetiredInstructionRegister->operator++(); 
            CheckInterrupts();
            return true;
        }
    }
    catch(const std::out_of_range &OutOfRange){
        if(!NextInstruction){
            // Instruction access fault cause 1
            InvokeTrap(1, false);
        }
        else{
            // Determine if load or store differ at bit 5
            if(NextInstruction->RawInstruction() & 0x20){
                // Store/AMO access fault cause 7
                InvokeTrap(7,false);
            }
            else{
                // Load access fault cause 5
                InvokeTrap(5,false);
            }
        }
        
    }
    return false;
}

void CRISCVCPU::SetInterruptPending(EInterruptSource source){
    DMachineInterruptPendingRegister->fetch_or(to_underlying(source));
}

void CRISCVCPU::ClearInterruptPending(EInterruptSource source){
    DMachineInterruptPendingRegister->fetch_and(~to_underlying(source));
}

std::shared_ptr< CRISCVCPU::CInstruction > CRISCVCPU::DecodeInstruction(uint32_t addr){
    
    return CInstruction::DecodeInstruction(addr,DMemory->LoadUINT32(addr),shared_from_this());
}

void CRISCVCPU::OutputCSRs(){
    for(auto &CSRKey : DControlStatusRegisterKeys){
        printf("%03X: %08X\n",CSRKey,DControlStatusRegisters[CSRKey]->load());
    }   
}

uint32_t CRISCVCPU::ControlStatusRegister(uint32_t index) const{
    auto Search = DControlStatusRegisters.find(index);
    if(Search != DControlStatusRegisters.end()){
        return Search->second->load();
    }
    return 0;
}

std::string CRISCVCPU::ControlStatusRegisterName(uint32_t index) const{
    auto Search = DControlStatusRegisterNames.find(index);
    if(Search != DControlStatusRegisterNames.end()){
        return Search->second;
    }
    return std::string();
}

void CRISCVCPU::Reset(){
    DProgramCounter->reset();
    for(auto &Register : DRegisters){
        Register->reset();
    }
    DCycleRegister->reset();
    DRetiredInstructionRegister->reset();
    for(auto &CSRPair : DControlStatusRegisters){
        CSRPair.second->reset();
    }
    //DInstructionCache->Reset();
}