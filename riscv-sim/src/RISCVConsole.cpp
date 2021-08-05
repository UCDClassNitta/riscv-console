#include "RISCVConsole.h"
#include "MemoryControllerDevice.h"
#include "RAMMemoryDevice.h"
#include "RISCVBlockInstructionCache.h"
#include "GraphicFactory.h"
#include <iostream>
#include <functional>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits>

const uint32_t CRISCVConsole::DMainMemorySize = 0x1000000;          // 16MB
const uint32_t CRISCVConsole::DMainMemoryBase = 0x70000000;         // Set at base in first 2GB region
const uint32_t CRISCVConsole::DFirmwareMemorySize = 0x1000000;      // 16MB
const uint32_t CRISCVConsole::DFirmwareMemoryBase = 0x00000000;     // Set at base of memory
const uint32_t CRISCVConsole::DCartridgeMemorySize = 0x1000000;     // 16MB
const uint32_t CRISCVConsole::DCartridgeMemoryBase = 0x20000000;    // Set 512MB above Flash
const uint32_t CRISCVConsole::DVideoMemoryBase = 0x50000000;        // Set at 1.25GB base
const uint32_t CRISCVConsole::DRegisterMemoryBase = 0x40000000;     // Set at 1GB base

// Code form Effective Modern C++ by Scott Meyers (see Item 10)
template<typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E enumerator) noexcept{
    return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

CRISCVConsole::CRISCVConsole(uint32_t timerus, uint32_t videoms, uint32_t cpufreq){
    DDebugMode = false;
    DTimerDelayUS = timerus;
    DVideoDelayMS = videoms;
    DDebugCPUFreq = cpufreq;
    DVideoTicks = DVideoDelayMS;
    DTimerTicks = uint64_t(DTimerDelayUS) * DDebugCPUFreq / 1000000;

    DRefreshScreenBuffer.store(false);
    DVideoController = std::make_shared< CVideoController >();

    DMemoryController = std::make_shared< CMemoryControllerDevice >(32);    
    DMainMemory = std::make_shared< CRAMMemoryDevice >(DMainMemorySize);
    DMemoryController->AttachDevice(DMainMemory,DMainMemoryBase);
    DFirmwareFlash = std::make_shared< CFlashMemoryDevice >(DFirmwareMemorySize);
    DMemoryController->AttachDevice(DFirmwareFlash,DFirmwareMemoryBase);
    DCartridgeFlash = std::make_shared< CFlashMemoryDevice >(DCartridgeMemorySize);
    DMemoryController->AttachDevice(DCartridgeFlash,DCartridgeMemoryBase);
    DMemoryController->AttachDevice(DVideoController->VideoRAM(),DVideoMemoryBase);
    
    

    DCPUCache = std::make_shared<CRISCVBlockInstructionCache>();
    DCPU = std::make_shared< CRISCVCPU >(DMemoryController, DCPUCache);
    DChipset = std::make_shared< CRISCVConsoleChipset >(DCPU, DMemoryController, timerus, videoms);

    DRegisterBlock = DChipset->RegisterBlock();

    DMemoryController->AttachDevice(DRegisterBlock, DRegisterMemoryBase);

    DSystemCommand.store(to_underlying(EThreadState::Stop));
    DCPUAcknowledge.store(to_underlying(EThreadState::Stop));
    DTimerAcknowledge.store(to_underlying(EThreadState::Stop));
    DSystemAcknowledge.store(to_underlying(EThreadState::Stop));
}

CRISCVConsole::~CRISCVConsole(){
    SystemStop();
}

void CRISCVConsole::CPUThreadExecute(){
    DCPUAcknowledge.store(to_underlying(EThreadState::Run));
    while(DSystemCommand.load() == to_underlying(EThreadState::Run)){
        DCPU->ExecuteInstruction();
        DChipset->IncrementDMA();
    }
    DCPUAcknowledge.store(to_underlying(EThreadState::Stop));
}

void CRISCVConsole::TimerThreadExecute(){
    DTimerAcknowledge.store(to_underlying(EThreadState::Run));
    auto LastTickTime = std::chrono::steady_clock::now();
    while(DSystemCommand.load() == to_underlying(EThreadState::Run)){
        auto ThisTickTime = std::chrono::steady_clock::now();
        auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(ThisTickTime - LastTickTime);
        if(Duration.count() < DTimerDelayUS){
            std::this_thread::sleep_for(std::chrono::microseconds(DTimerDelayUS - Duration.count()));
        }
        LastTickTime = ThisTickTime;
        DChipset->IncrementTimer();
    }
    DTimerAcknowledge.store(to_underlying(EThreadState::Stop));
}

void CRISCVConsole::SystemThreadExecute(){
    bool HitBreakpoint = false;
    DSystemAcknowledge.store(to_underlying(EThreadState::Run));
    while(DSystemCommand.load() == to_underlying(EThreadState::Run)){
        if(SystemStep()){
            HitBreakpoint = true;
            break;
        }
    }
    DSystemAcknowledge.store(to_underlying(HitBreakpoint ? EThreadState::Breakpoint : EThreadState::Stop));
}

void CRISCVConsole::SystemRun(){
    if(DDebugMode){
        if(DSystemThread){
            return;
        }
        DSystemAcknowledge.store(to_underlying(EThreadState::Stop));
        DSystemCommand.store(to_underlying(EThreadState::Run));
        DSystemThread = std::make_shared< std::thread >(&CRISCVConsole::SystemThreadExecute,this);

        while(DSystemAcknowledge.load() == to_underlying(EThreadState::Stop)){
            std::this_thread::yield();
        }
    }
    else{
        if(DCPUThread){
            return;
        }

        DSystemCommand.store(to_underlying(EThreadState::Run));
        DCPUThread = std::make_shared< std::thread >(&CRISCVConsole::CPUThreadExecute,this);
        DTimerThread = std::make_shared< std::thread >(&CRISCVConsole::TimerThreadExecute,this);

        while((DCPUAcknowledge.load() != to_underlying(EThreadState::Run))||(DTimerAcknowledge.load() != to_underlying(EThreadState::Run))){
            std::this_thread::yield();
        }
    }
}

void CRISCVConsole::SystemStop(){
    if(DDebugMode){
        if(!DSystemThread){
            return;
        }
        DSystemCommand.store(to_underlying(EThreadState::Stop));
        DSystemThread->join();
        DSystemThread.reset();
        DSystemAcknowledge.store(to_underlying(EThreadState::Stop));
    }
    else{
        if(!DCPUThread){
            return;
        }
        DSystemCommand.store(to_underlying(EThreadState::Stop));
        DCPUThread->join();
        DTimerThread->join();
        DCPUThread.reset();
        DTimerThread.reset();
    }
}

bool CRISCVConsole::SystemStep(){
    DCPU->ExecuteInstruction();
    DChipset->IncrementDMA();
    DTimerTicks--;
    if(!DTimerTicks){
        DChipset->IncrementTimer();
        DTimerTicks = uint64_t(DTimerDelayUS) * DDebugCPUFreq / 1000000;
    }
    DVideoTicks--;
    if(!DVideoTicks){
        if(DVideoController->Tick()){
            DChipset->SetInterruptPending(CRISCVConsoleChipset::EInterruptSource::Video);
            DRefreshScreenBuffer.store(true);
        }
        DVideoTicks = uint64_t(DVideoDelayMS) * DDebugCPUFreq / 1000;
    }
    return DBreakpoints.end() != DBreakpoints.find(DCPU->ProgramCounter());
}

void CRISCVConsole::ResetComponents(){
    DVideoController->Reset();
    DCPU->Reset();
    DChipset->Reset();
}

void CRISCVConsole::ConstructInstructionStrings(CElfLoad &elffile, std::vector< std::string > &strings, std::unordered_map< uint32_t, size_t > &translations, std::vector< std::string > &labels, std::vector< size_t > &labelindices){
    strings.clear();
    translations.clear();
    labels.clear();
    labelindices.clear();
    for(size_t Index = 0; Index < elffile.SectionHeaderCount(); Index++){
        auto &Header = elffile.SectionHeader(Index);
        if(Header.DFlags & 0x4){
            std::vector< uint32_t > AddressKeys;
            AddressKeys.reserve(Header.DSymbols.size());
            for(auto &Symbol : Header.DSymbols){
                AddressKeys.push_back(Symbol.first);
            }
            std::sort(AddressKeys.begin(),AddressKeys.end());
            auto NextSymbol = AddressKeys.begin();
            uint32_t CurrentAddress = Header.DVirtualAddress;
            uint32_t EndAddress = Header.DVirtualAddress + Header.DSize;
            while(CurrentAddress < EndAddress){
                if((NextSymbol != AddressKeys.end())&&(CurrentAddress == *NextSymbol)){
                    labels.push_back(Header.DSymbols.find(CurrentAddress)->second);
                    labelindices.push_back(strings.size());
                    strings.push_back(Header.DSymbols.find(CurrentAddress)->second);
                    NextSymbol++;
                }
                translations[CurrentAddress] = strings.size();
                std::stringstream Stream;
                Stream<<" "<<std::setfill('0') << std::setw(8) << std::hex << CurrentAddress << ": ";
                auto NewInstruction = DCPUCache->Fetch(CurrentAddress);
                if(!NewInstruction){
                    NewInstruction = DCPU->DecodeInstruction(CurrentAddress);
                    if(NewInstruction){
                        DCPUCache->Insert(NewInstruction);
                    }
                }
                if(NewInstruction){
                    Stream<<NewInstruction->ToString();
                }
                else{
                    Stream<<"invalid";
                }
                
                strings.push_back(Stream.str());

                CurrentAddress += sizeof(uint32_t);
            }
        }
    }
}

void CRISCVConsole::ConstructFirmwareStrings(CElfLoad &elffile){
    DFirmwareInstructionStrings.clear();
    DFirmwareAddressesToIndices.clear();
    DFirmwareInstructionLabels.clear();
    DFirmwareInstructionLabelIndices.clear();
    ConstructInstructionStrings(elffile, DFirmwareInstructionStrings, DFirmwareAddressesToIndices, DFirmwareInstructionLabels, DFirmwareInstructionLabelIndices);
    DInstructionStrings = DFirmwareInstructionStrings;
    DInstructionStrings.insert(DInstructionStrings.end(), DCartridgeInstructionStrings.begin(),DCartridgeInstructionStrings.end());
    DInstructionAddressesToIndices = DFirmwareAddressesToIndices;
    for(auto &AddrIdx : DCartridgeAddressesToIndices){
        DInstructionAddressesToIndices[AddrIdx.first] = AddrIdx.second + DFirmwareInstructionStrings.size();
    }
    DInstructionLabels = DFirmwareInstructionLabels;
    DInstructionLabels.insert(DInstructionLabels.end(), DCartridgeInstructionLabels.begin(),DCartridgeInstructionLabels.end());
    DInstructionLabelIndices = DFirmwareInstructionLabelIndices;
    for(auto &Index : DCartridgeInstructionLabelIndices){
        DInstructionLabelIndices.push_back(DFirmwareInstructionStrings.size() + Index);
    }
    MarkBreakpointStrings();
}

void CRISCVConsole::ConstructCartridgeStrings(CElfLoad &elffile){
    DCartridgeInstructionStrings.clear();
    DCartridgeAddressesToIndices.clear();
    DCartridgeInstructionLabels.clear();
    DCartridgeInstructionLabelIndices.clear();
    ConstructInstructionStrings(elffile, DCartridgeInstructionStrings, DCartridgeAddressesToIndices, DCartridgeInstructionLabels, DCartridgeInstructionLabelIndices);
    DInstructionStrings = DFirmwareInstructionStrings;
    DInstructionStrings.insert(DInstructionStrings.end(), DCartridgeInstructionStrings.begin(),DCartridgeInstructionStrings.end());
    DInstructionAddressesToIndices = DFirmwareAddressesToIndices;
    for(auto &AddrIdx : DCartridgeAddressesToIndices){
        DInstructionAddressesToIndices[AddrIdx.first] = AddrIdx.second + DFirmwareInstructionStrings.size();
    }
    DInstructionLabels = DFirmwareInstructionLabels;
    DInstructionLabels.insert(DInstructionLabels.end(), DCartridgeInstructionLabels.begin(),DCartridgeInstructionLabels.end());
    DInstructionLabelIndices = DFirmwareInstructionLabelIndices;
    for(auto &Index : DCartridgeInstructionLabelIndices){
        DInstructionLabelIndices.push_back(DFirmwareInstructionStrings.size() + Index);
    }
    MarkBreakpointStrings();
}

void CRISCVConsole::MarkBreakpointStrings(){
    for(auto &Addr : DBreakpoints){
        auto Search = DInstructionAddressesToIndices.find(Addr);
        if(DInstructionAddressesToIndices.end() != Search){
            DInstructionStrings[Search->second][0] = '@';
        }
    }
}

void CRISCVConsole::SetDebugMode(bool debug){
    DDebugMode = debug;
    if(debug){
        DVideoTicks = uint64_t(DVideoDelayMS) * DDebugCPUFreq / 1000;
        DTimerTicks = uint64_t(DTimerDelayUS) * DDebugCPUFreq / 1000000;
    }
}

void CRISCVConsole::Reset(){
    auto CurrentState = DSystemCommand.load();
    SystemStop();
    ResetComponents();
    if(CurrentState == to_underlying(EThreadState::Run)){
        // System was running, start it up again
        SystemRun();
    }
}

void CRISCVConsole::PowerOn(){
    Reset();
    if(!DDebugMode){
        SystemRun();
    }
}

void CRISCVConsole::PowerOff(){
    SystemStop();
}

void CRISCVConsole::Run(){
    SystemRun();
}

void CRISCVConsole::Stop(){
    SystemStop();
}

void CRISCVConsole::Step(){
    SystemStep();
}

uint64_t CRISCVConsole::PressDirection(EDirection dir){
    DChipset->ControllerPress(to_underlying(dir));
    // Provide best effort instruction for potential recording
    return DCPU->RetiredInstructionCount();
}

uint64_t CRISCVConsole::ReleaseDirection(EDirection dir){
    DChipset->ControllerRelease(to_underlying(dir));
    // Provide best effort instruction for potential recording
    return DCPU->RetiredInstructionCount();
}

uint64_t CRISCVConsole::PressButton(EButtonNumber button){
    DChipset->ControllerPress(to_underlying(button));
    // Provide best effort instruction for potential recording
    return DCPU->RetiredInstructionCount();
}

uint64_t CRISCVConsole::ReleaseButton(EButtonNumber button){
    DChipset->ControllerRelease(to_underlying(button));
    // Provide best effort instruction for potential recording
    return DCPU->RetiredInstructionCount();
}

uint64_t CRISCVConsole::PressCommand(){
    DChipset->ControllerCommandPress();
    // Provide best effort instruction for potential recording
    return DCPU->RetiredInstructionCount();
}

bool CRISCVConsole::VideoTimerTick(std::shared_ptr<CGraphicSurface> screensurface){
    if(DDebugMode){
        if(DSystemAcknowledge.load() == to_underlying(EThreadState::Breakpoint)){
            if(DSystemThread){
                SystemStop();
                if(DBreakpointCallback){
                    DBreakpointCallback(DBreakpointCalldata);
                }
            }
        }
        if(DRefreshScreenBuffer.exchange(false)){
            DVideoController->Refresh(screensurface);
            return true;
        }
    }
    else{
        DVideoTicks--;
        if(!DVideoTicks){
            DVideoTicks = DVideoDelayMS;
            if(DVideoController->Tick()){
                DVideoController->Refresh(screensurface);
                DChipset->SetInterruptPending(CRISCVConsoleChipset::EInterruptSource::Video);
                return true;
            }
        }
    }
    return false;
}

bool CRISCVConsole::ProgramFirmware(std::shared_ptr< CDataSource > elfsrc){
    // Program the firmware
    CElfLoad ElfFile(elfsrc);
    if(ElfFile.ValidFile()){
        for(size_t Index = 0; Index < ElfFile.ProgramHeaderCount(); Index++){
            auto &Header = ElfFile.ProgramHeader(Index);
            if((Header.DPhysicalAddress < DFirmwareMemoryBase)||(DFirmwareMemoryBase + DFirmwareMemorySize < Header.DPhysicalAddress + Header.DFileSize)){
                return false;
            }
        }
        auto CurrentState = DSystemCommand.load();
        SystemStop();
        DFirmwareFlash->WriteEnabled(true);
        DFirmwareFlash->EraseAll();
        for(size_t Index = 0; Index < ElfFile.ProgramHeaderCount(); Index++){
            auto &Header = ElfFile.ProgramHeader(Index);
            DFirmwareFlash->StoreData(Header.DPhysicalAddress,Header.DPayload.data(),Header.DFileSize);
        }
        DFirmwareFlash->WriteEnabled(false);
        DCPUCache->FlushRange(DFirmwareMemoryBase, DFirmwareMemorySize);
        ResetComponents();
        ConstructFirmwareStrings(ElfFile);
        if(CurrentState == to_underlying(EThreadState::Run)){
            // System was running, start it up again
            SystemRun();
        }
        return true;
    }
    return false;
}

uint64_t CRISCVConsole::InsertCartridge(std::shared_ptr< CDataSource > elfsrc){
    // Program the cartridge
    CElfLoad ElfFile(elfsrc);
    if(ElfFile.ValidFile()){
        for(size_t Index = 0; Index < ElfFile.ProgramHeaderCount(); Index++){
            auto &Header = ElfFile.ProgramHeader(Index);
            if((Header.DPhysicalAddress < DCartridgeMemoryBase)||(DCartridgeMemoryBase + DCartridgeMemorySize < Header.DPhysicalAddress + Header.DFileSize)){
                return false;
            }
        }
        auto CurrentState = DSystemCommand.load();
        SystemStop();
        DCartridgeFlash->WriteEnabled(true);
        DCartridgeFlash->EraseAll();
        for(size_t Index = 0; Index < ElfFile.ProgramHeaderCount(); Index++){
            auto &Header = ElfFile.ProgramHeader(Index);
            DCartridgeFlash->StoreData(Header.DPhysicalAddress,Header.DPayload.data(),Header.DFileSize);
        }
        DCartridgeFlash->WriteEnabled(false);
        DCPUCache->FlushRange(DCartridgeMemoryBase, DCartridgeMemorySize);
        DChipset->InsertCartridge(ElfFile.Entry());
        ConstructCartridgeStrings(ElfFile);
        auto EventCycle = DCPU->RetiredInstructionCount();
        if(CurrentState == to_underlying(EThreadState::Run)){
            // System was running, start it up again, mark cartridge entry
            SystemRun();
        }
        return EventCycle;
    }
    return std::numeric_limits<uint64_t>::max();
}

uint64_t CRISCVConsole::RemoveCartridge(){
    auto CurrentState = DSystemCommand.load();
    SystemStop();

    DCartridgeInstructionStrings.clear();
    DCartridgeAddressesToIndices.clear();
    DCartridgeInstructionLabels.clear();
    DCartridgeInstructionLabelIndices.clear();
    DInstructionStrings = DFirmwareInstructionStrings;
    DInstructionAddressesToIndices = DFirmwareAddressesToIndices;
    DInstructionLabels = DFirmwareInstructionLabels;
    DInstructionLabelIndices = DFirmwareInstructionLabelIndices;
    MarkBreakpointStrings();
    DCartridgeFlash->WriteEnabled(true);
    DCartridgeFlash->EraseAll();
    DCartridgeFlash->WriteEnabled(false);
    DCPUCache->FlushRange(DCartridgeMemoryBase, DCartridgeMemorySize);
    DChipset->RemoveCartridge();
    auto EventCycle = DCPU->RetiredInstructionCount();
    if(CurrentState == to_underlying(EThreadState::Run)){
        // Clear the cache for cartridge
        SystemRun();
    }

    return EventCycle;
}

void CRISCVConsole::AddBreakpoint(uint32_t addr){
    auto CurrentState = DSystemCommand.load();
    SystemStop();
    DBreakpoints.insert(addr);
    if(CurrentState == to_underlying(EThreadState::Run)){
        SystemRun();
    }
}

void CRISCVConsole::RemoveBreakpoint(uint32_t addr){
    auto CurrentState = DSystemCommand.load();
    SystemStop();
    DBreakpoints.erase(addr);
    if(CurrentState == to_underlying(EThreadState::Run)){
        SystemRun();
    }
}

void CRISCVConsole::SetBreakcpointCallback(CRISCVConsoleBreakpointCalldata calldata, CRISCVConsoleBreakpointCallback callback){
    DBreakpointCalldata = calldata;
    DBreakpointCallback = callback;
}

void CRISCVConsole::ClearBreakpoints(){
    auto CurrentState = DSystemCommand.load();
    SystemStop();
    for(auto &Addr : DBreakpoints){
        auto Search = DInstructionAddressesToIndices.find(Addr);
        if(DInstructionAddressesToIndices.end() != Search){
            DInstructionStrings[Search->second][0] = ' ';
        }
    }
    DBreakpoints.clear();
    if(CurrentState == to_underlying(EThreadState::Run)){
        SystemRun();
    }
}
