#ifndef RISCVCCONSOLE_H
#define RISCVCCONSOLE_H

#include "RISCVCPU.h"
#include "RISCVConsoleChipset.h"
#include "ElfLoad.h"
#include "MemoryDevice.h"
#include "FlashMemoryDevice.h"
#include "VideoController.h"
#include "DataSource.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <set>

using CRISCVConsoleBreakpointCalldata = void *;
using CRISCVConsoleBreakpointCallback = void (*)(CRISCVConsoleBreakpointCalldata);

class CRISCVConsole{
    public:
        enum class EDirection : uint32_t {Left = 0x1, Up = 0x2, Down = 0x4, Right = 0x8};
        enum class EButtonNumber : uint32_t {Button1 = 0x10, Button2 = 0x20, Button3 = 0x40, Button4 = 0x80};
    protected:
        enum class EThreadState : uint32_t {Stop = 0, Run = 1, Breakpoint = 2};
        bool DDebugMode;
        uint32_t DTimerDelayUS;
        uint32_t DVideoDelayMS;
        uint32_t DDebugCPUFreq;
        uint32_t DVideoTicks;
        uint32_t DTimerTicks;
        std::atomic< bool > DRefreshScreenBuffer;
        size_t DPendingReleaseBuffer;
        std::shared_ptr< CRISCVCPU > DCPU;
        std::shared_ptr< CRISCVCPU::CInstructionCache > DCPUCache;
        std::shared_ptr< CMemoryDevice > DMemoryController;
        std::shared_ptr< CMemoryDevice > DMainMemory;
        std::shared_ptr< CRegisterBlockMemoryDevice > DRegisterBlock;
        std::shared_ptr< CFlashMemoryDevice > DFirmwareFlash;
        std::shared_ptr< CFlashMemoryDevice > DCartridgeFlash;
        std::shared_ptr< CVideoController > DVideoController;
        std::shared_ptr< CRISCVConsoleChipset > DChipset;

        std::atomic<uint32_t> DSystemCommand;
        std::atomic<uint32_t> DCPUAcknowledge;
        std::atomic<uint32_t> DTimerAcknowledge;
        std::atomic<uint32_t> DSystemAcknowledge;
        std::shared_ptr< std::thread > DCPUThread;
        std::shared_ptr< std::thread > DTimerThread;
        std::shared_ptr< std::thread > DSystemThread;

        std::chrono::steady_clock::time_point DSystemStartTime;
        uint64_t DCPUStartInstructionCount;
        
        std::vector< std::string > DFirmwareInstructionStrings;
        std::unordered_map< uint32_t, size_t > DFirmwareAddressesToIndices;
        std::vector< std::string > DFirmwareInstructionLabels;
        std::vector< size_t > DFirmwareInstructionLabelIndices;
        std::vector< uint32_t > DFirmwareInstructionLabelAddresses;

        std::vector< std::string > DCartridgeInstructionStrings;
        std::unordered_map< uint32_t, size_t > DCartridgeAddressesToIndices;
        std::vector< std::string > DCartridgeInstructionLabels;
        std::vector< size_t > DCartridgeInstructionLabelIndices;
        std::vector< uint32_t > DCartridgeInstructionLabelAddresses;

        std::vector< std::string > DInstructionStrings;
        std::unordered_map< uint32_t, size_t > DInstructionAddressesToIndices;
        std::vector< std::string > DInstructionLabels;
        std::vector< size_t > DInstructionLabelIndices;
        std::vector< uint32_t > DInstructionLabelAddresses;

        std::set< uint32_t > DBreakpoints;
        CRISCVConsoleBreakpointCalldata DBreakpointCalldata;
        CRISCVConsoleBreakpointCallback DBreakpointCallback;

        std::shared_ptr< CDwarfStructures::SProgrammaticScope > DFirmwareScope;
        std::shared_ptr< CDwarfStructures::SProgrammaticScope > DCartridgeScope;


        static const uint32_t DMainMemorySize;
        static const uint32_t DMainMemoryBase;
        static const uint32_t DFirmwareMemorySize;
        static const uint32_t DFirmwareMemoryBase;
        static const uint32_t DCartridgeMemorySize;
        static const uint32_t DCartridgeMemoryBase;
        static const uint32_t DVideoMemoryBase;
        static const uint32_t DRegisterMemoryBase;

        void CPUThreadExecute();
        void TimerThreadExecute();
        void SystemThreadExecute();
        void SystemRun();
        void SystemStop();
        bool SystemStep();
        void ResetComponents();

        void LoadElfSourceFiles(CElfLoad &elffile, std::shared_ptr<CDataContainer> elfcontainer, std::vector< std::vector< std::string > > &filelines);
        void ConstructInstructionStrings(CElfLoad &elffile, std::shared_ptr<CDataContainer> elfcontainer, std::vector< std::string > &strings, std::unordered_map< uint32_t, size_t > &translations, std::vector< std::string > &labels, std::vector< size_t > &labelindices, std::vector< uint32_t > &labeladdresses);
        void ConstructFirmwareStrings(CElfLoad &elffile, std::shared_ptr<CDataContainer> elfcontainer);
        void ConstructCartridgeStrings(CElfLoad &elffile, std::shared_ptr<CDataContainer> elfcontainer);
        void MarkBreakpointStrings();

    public:
        CRISCVConsole(uint32_t timerus, uint32_t videoms, uint32_t cpufreq, std::shared_ptr<CVideoController> videocontroller);
        ~CRISCVConsole();

        uint32_t ScreenWidth(){
            return DVideoController->ScreenWidth();
        };

        uint32_t ScreenHeight(){
            return DVideoController->ScreenHeight();
        };

        std::shared_ptr< CRISCVCPU > CPU(){
            return DCPU;
        };

        std::shared_ptr< CMemoryDevice > Memory(){
            return DMemoryController;
        };

        void SetDebugMode(bool debug);

        void Reset();

        void PowerOn();

        void PowerOff();

        void Run();

        void Stop();

        void Step();

        uint64_t PressDirection(EDirection dir);
        uint64_t ReleaseDirection(EDirection dir);

        uint64_t PressButton(EButtonNumber button);
        uint64_t ReleaseButton(EButtonNumber button);

        uint64_t PressCommand();

        bool VideoTimerTick(std::shared_ptr<CGraphicSurface> screensurface);

        bool ProgramFirmware(std::shared_ptr< CDataSource > elfsrc);

        uint64_t InsertCartridge(std::shared_ptr< CDataSource > elfsrc);

        uint64_t RemoveCartridge();

        void AddBreakpoint(uint32_t addr);

        void RemoveBreakpoint(uint32_t addr);

        void SetBreakcpointCallback(CRISCVConsoleBreakpointCalldata calldata, CRISCVConsoleBreakpointCallback callback);

        const std::set< uint32_t > &Breakpoints() const{
            return DBreakpoints;
        };

        void ClearBreakpoints();

        const std::vector< std::string > &InstructionStrings() const{
            return DInstructionStrings;
        }

        size_t InstructionAddressesToIndices(uint32_t addr) const{
            auto Search = DInstructionAddressesToIndices.find(addr);
            if(Search != DInstructionAddressesToIndices.end()){
                return Search->second;
            }
            return -1;
        }

        const std::vector< std::string > &InstructionLabels() const{
            return DInstructionLabels;
        }

        const std::vector< size_t > &InstructionLabelIndices() const{
            return DInstructionLabelIndices;
        }
        
        const std::vector< uint32_t > &InstructionLabelAddresses() const{
            return DInstructionLabelAddresses;
        };

        uint32_t MainMemorySize(){
            return DMainMemorySize;
        };

        uint32_t MainMemoryBase(){
            return DMainMemoryBase;
        };

        uint32_t FirmwareMemorySize(){
            return DFirmwareMemorySize;
        };

        uint32_t FirmwareMemoryBase(){
            return DFirmwareMemoryBase;
        };

        uint32_t CartridgeMemorySize(){
            return DCartridgeMemorySize;
        };

        uint32_t CartridgeMemoryBase(){
            return DCartridgeMemoryBase;
        };

        uint32_t VideoMemorySize(){
            return DVideoController->VideoRAM()->MemorySize();
        };

        uint32_t VideoMemoryBase(){
            return DVideoMemoryBase;
        };

        std::vector< uint32_t > VideoMemorySegmentBases() const{
            return DVideoController->SegmentBases();
        };

        uint32_t RegisterMemorySize(){
            return DRegisterBlock->MemorySize();
        };

        uint32_t RegisterMemoryBase(){
            return DRegisterMemoryBase;
        };

        bool GetActiveScopes(std::vector< std::shared_ptr< CDwarfStructures::SProgrammaticScope > > &scopes) const;

};

#endif
