#include "RISCVConsole.h"
#include "MemoryControllerDevice.h"
#include "RAMMemoryDevice.h"
#include "RISCVBlockInstructionCache.h"
#include "ElfLoad.h"
#include <iostream>
#include <functional>

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

CRISCVConsole::CRISCVConsole(){
    DVideoController = std::make_shared< CVideoController >();
    
    DControllerState = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(0);

    DMemoryController = std::make_shared< CMemoryControllerDevice >(32);    
    DMainMemory = std::make_shared< CRAMMemoryDevice >(DMainMemorySize);
    DMemoryController->AttachDevice(DMainMemory,DMainMemoryBase);
    DFirmwareFlash = std::make_shared< CFlashMemoryDevice >(DFirmwareMemorySize);
    DMemoryController->AttachDevice(DFirmwareFlash,DFirmwareMemoryBase);
    DCartridgeFlash = std::make_shared< CFlashMemoryDevice >(DCartridgeMemorySize);
    DMemoryController->AttachDevice(DCartridgeFlash,DCartridgeMemoryBase);
    DMemoryController->AttachDevice(DVideoController->VideoRAM(),DVideoMemoryBase);
    
    DRegisterBlock = std::make_shared< CRegisterBlockMemoryDevice >();
    

    DCPUCache = std::make_shared<CRISCVBlockInstructionCache>();
    DCPU = std::make_shared< CRISCVCPU >(DMemoryController, DCPUCache);
    DChipset = std::make_shared< CRISCVConsoleChipset >(DCPU);
    // TODO add registers to memory controller
    DRegisterBlock->AttachRegister(DChipset->InterruptEnable());
    DRegisterBlock->AttachRegister(DChipset->InterruptPending());
    DRegisterBlock->AttachRegister(DChipset->MachineTimeLow());
    DRegisterBlock->AttachRegister(DChipset->MachineTimeHigh());
    DRegisterBlock->AttachRegister(DChipset->MachineTimeCompareLow());
    DRegisterBlock->AttachRegister(DChipset->MachineTimeCompareHigh());
    DRegisterBlock->AttachRegister(DControllerState);
    DMemoryController->AttachDevice(DRegisterBlock, DRegisterMemoryBase);

    DSystemCommand.store(to_underlying(EThreadState::Stop));
    DCPUAcknowledge.store(to_underlying(EThreadState::Stop));
    DTimerAcknowledge.store(to_underlying(EThreadState::Stop));
    DCPUThread = std::make_shared< std::thread >(&CRISCVConsole::CPUThreadExecute,this);
    DTimerThread = std::make_shared< std::thread >(&CRISCVConsole::TimerThreadExecute,this);
}

CRISCVConsole::~CRISCVConsole(){
    SystemTerminate();
    DCPUThread->join();
    DTimerThread->join();
}

bool CRISCVConsole::SystemNotStop(){
    return DSystemCommand.load() != to_underlying(EThreadState::Stop);
}

void CRISCVConsole::CPUThreadExecute(){
    std::unique_lock<std::mutex> Lock(DCPUMutex);
    bool WasSleeping = true;
    bool Running = false;
    while(DSystemCommand.load() != to_underlying(EThreadState::Terminate)){
        if(DSystemCommand.load() == to_underlying(EThreadState::Stop)){
            DCPUAcknowledge.store(to_underlying(EThreadState::Stop));
            printf("CPU Sleeping\n");
            DCPUConditionVariable.wait(Lock, std::bind(&CRISCVConsole::SystemNotStop,this));
            printf("CPU Awake\n");
            WasSleeping = true;
            Running = DSystemCommand.load() == to_underlying(EThreadState::Run);
        }
        if(WasSleeping && (DSystemCommand.load() == to_underlying(EThreadState::Run))){
            DCPUAcknowledge.store(to_underlying(EThreadState::Run));
            WasSleeping = false;
        }
        // Execute instruction
        if(Running){
            DCPU->ExecuteInstruction();
        }
    }
    DCPUAcknowledge.store(to_underlying(EThreadState::Terminate));
}

void CRISCVConsole::TimerThreadExecute(){
    std::unique_lock<std::mutex> Lock(DTimerMutex);
    bool WasSleeping = true;
    bool Running = false;
    auto LastTickTime = std::chrono::steady_clock::now();
    while(DSystemCommand.load() != to_underlying(EThreadState::Terminate)){
        if(DSystemCommand.load() == to_underlying(EThreadState::Stop)){
            DTimerAcknowledge.store(to_underlying(EThreadState::Stop));
            //printf("Timer Sleeping\n");
            DTimerConditionVariable.wait(Lock, std::bind(&CRISCVConsole::SystemNotStop,this));
            //printf("Timer Awake\n");
            WasSleeping = true;
            Running = DSystemCommand.load() == to_underlying(EThreadState::Run);
        }
        if(WasSleeping && (DSystemCommand.load() == to_underlying(EThreadState::Run))){
            DTimerAcknowledge.store(to_underlying(EThreadState::Run));
            WasSleeping = false;
        }
        // Execute instruction
        if(Running){
            DChipset->IncrementTimer();
            auto ThisTickTime = std::chrono::steady_clock::now();
            auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(ThisTickTime - LastTickTime);
            if(Duration.count() < 1000){
                std::this_thread::sleep_for(std::chrono::microseconds(1000 - Duration.count()));
            }
            LastTickTime = ThisTickTime;
        }
    }
    DTimerAcknowledge.store(to_underlying(EThreadState::Terminate));
}

void CRISCVConsole::SystemRun(){
    if(to_underlying(EThreadState::Stop) == DSystemCommand.exchange(to_underlying(EThreadState::Run))){
        DSystemStartTime = std::chrono::steady_clock::now();
        DCPUStartInstructionCount = DCPU->RetiredInstructionCount();
        DCPUConditionVariable.notify_one();
        DTimerConditionVariable.notify_one();
    }
    while((DCPUAcknowledge.load() != to_underlying(EThreadState::Run))&&(DTimerAcknowledge.load() != to_underlying(EThreadState::Run))){
        std::this_thread::yield();
    }
}

void CRISCVConsole::SystemStop(){
    auto PreviousState = DSystemCommand.exchange(to_underlying(EThreadState::Stop));
    while((DCPUAcknowledge.load() != to_underlying(EThreadState::Stop))&&(DTimerAcknowledge.load() != to_underlying(EThreadState::Stop))){
        std::this_thread::yield();
    }
    if(PreviousState == to_underlying(EThreadState::Run)){
        auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - DSystemStartTime);
        auto InstructionCount = DCPU->RetiredInstructionCount() - DCPUStartInstructionCount;
        printf("CPU executed at %.2lfMHz\n",double(InstructionCount) / Duration.count());
    }
    
}

void CRISCVConsole::SystemTerminate(){
    if(to_underlying(EThreadState::Stop) == DSystemCommand.exchange(to_underlying(EThreadState::Terminate))){
        DCPUConditionVariable.notify_one();
        DTimerConditionVariable.notify_one();
    }
    while((DCPUAcknowledge.load() != to_underlying(EThreadState::Terminate))&&(DTimerAcknowledge.load() != to_underlying(EThreadState::Terminate))){
        std::this_thread::yield();
    }
}

void CRISCVConsole::ResetComponents(){
    DVideoController->Reset();
    DCPU->Reset();
    DChipset->Reset();
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
    SystemRun();
}

void CRISCVConsole::PowerOff(){
    SystemStop();
    for(int Index = 0; Index < 8; Index++){
        printf("x%02d: %08x %-10u | x%02d: %08x %u\n",Index,DCPU->Register(Index),DCPU->Register(Index),Index+8,DCPU->Register(Index+8),DCPU->Register(Index+8));
    }
    DCPU->OutputCSRs();
    DMemoryController->DumpData(std::cout);
}

void CRISCVConsole::PressDirection(EDirection dir){
    DControllerState->fetch_or(to_underlying(dir));
}

void CRISCVConsole::ReleaseDirection(EDirection dir){
    DControllerState->fetch_and(~to_underlying(dir));
}

void CRISCVConsole::PressButton(EButtonNumber button){
    DControllerState->fetch_or(to_underlying(button));
}

void CRISCVConsole::ReleaseButton(EButtonNumber button){
    DControllerState->fetch_and(~to_underlying(button));
}

void CRISCVConsole::PressCommand(){
    DChipset->SetInterruptPending(CRISCVConsoleChipset::EInterruptSource::Command);
}

bool CRISCVConsole::VideoTimerTick(std::shared_ptr<CGraphicSurface> screensurface){
    if(DVideoController->Tick()){
        DVideoController->Refresh(screensurface);
        DChipset->SetInterruptPending(CRISCVConsoleChipset::EInterruptSource::Video);
        return true;
    }
    return false;
}

bool CRISCVConsole::ProgramFirmware(std::shared_ptr< CDataSource > elfsrc){
    auto CurrentState = DSystemCommand.load();
    SystemStop();
    // Program the firmware
    CElfLoad ElfFile(elfsrc);
    if(ElfFile.ValidFile()){
        DFirmwareFlash->WriteEnabled(true);
        DFirmwareFlash->EraseAll();
        for(size_t Index = 0; Index < ElfFile.ProgramHeaderCount(); Index++){
            auto &Header = ElfFile.ProgramHeader(Index);
            DFirmwareFlash->StoreData(Header.DPhysicalAddress,Header.DPayload.data(),Header.DFileSize);
        }
        DFirmwareFlash->WriteEnabled(false);
        ResetComponents();
        if(CurrentState == to_underlying(EThreadState::Run)){
            // System was running, start it up again
            SystemRun();
        }
        return true;
    }
    return false;
}

bool CRISCVConsole::InsertCartridge(std::shared_ptr< CDataSource > elfsrc){
    auto CurrentState = DSystemCommand.load();
    SystemStop();
    // Program the cartridge
    CElfLoad ElfFile(elfsrc);
    if(ElfFile.ValidFile()){

        DCartridgeFlash->WriteEnabled(true);
        DCartridgeFlash->EraseAll();
        for(size_t Index = 0; Index < ElfFile.ProgramHeaderCount(); Index++){
            auto &Header = ElfFile.ProgramHeader(Index);
            DCartridgeFlash->StoreData(Header.DPhysicalAddress,Header.DPayload.data(),Header.DFileSize);
        }
        DCartridgeFlash->WriteEnabled(false);
        DCPUCache->Reset(); // Chear in case the application changed

        if(CurrentState == to_underlying(EThreadState::Run)){
            // System was running, start it up again, mark cartridge entry
            DChipset->SetInterruptPending(CRISCVConsoleChipset::EInterruptSource::Cartridge);
            SystemRun();
        }
        return true;
    }
    return false;
}
        