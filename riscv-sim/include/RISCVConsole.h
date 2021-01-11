#ifndef RISCVCCONSOLE_H
#define RISCVCCONSOLE_H

#include "RISCVCPU.h"
#include "RISCVConsoleChipset.h"
#include "RegisterBlockMemoryDevice.h"
#include "MemoryDevice.h"
#include "FlashMemoryDevice.h"
#include "VideoController.h"
#include "DataSource.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

class CRISCVConsole{
    public:
        enum class EDirection : uint32_t {Left = 0x1, Up = 0x2, Down = 0x4, Right = 0x8};
        enum class EButtonNumber : uint32_t {Button1 = 0x10, Button2 = 0x20, Button3 = 0x40, Button4 = 0x80};
    protected:
        enum class EThreadState : uint32_t {Stop = 0, Run = 1, Terminate = 2};
        std::shared_ptr< CRISCVCPU > DCPU;
        std::shared_ptr< CRISCVCPU::CInstructionCache > DCPUCache;
        std::shared_ptr< CMemoryDevice > DMemoryController;
        std::shared_ptr< CMemoryDevice > DMainMemory;
        std::shared_ptr< CRegisterBlockMemoryDevice > DRegisterBlock;
        std::shared_ptr< CFlashMemoryDevice > DFirmwareFlash;
        std::shared_ptr< CFlashMemoryDevice > DCartridgeFlash;
        std::shared_ptr< CVideoController > DVideoController;
        std::shared_ptr< CRISCVConsoleChipset > DChipset;
        std::shared_ptr< CHardwareRegister< uint32_t > > DControllerState;

        std::atomic<uint32_t> DSystemCommand;
        std::atomic<uint32_t> DCPUAcknowledge;
        std::atomic<uint32_t> DTimerAcknowledge;
        std::shared_ptr< std::thread > DCPUThread;
        std::shared_ptr< std::thread > DTimerThread;
        std::mutex DCPUMutex;
        std::mutex DTimerMutex;
        std::condition_variable DCPUConditionVariable;
        std::condition_variable DTimerConditionVariable;

        std::chrono::steady_clock::time_point DSystemStartTime;
        uint64_t DCPUStartInstructionCount;
        

        static const uint32_t DMainMemorySize;
        static const uint32_t DMainMemoryBase;
        static const uint32_t DFirmwareMemorySize;
        static const uint32_t DFirmwareMemoryBase;
        static const uint32_t DCartridgeMemorySize;
        static const uint32_t DCartridgeMemoryBase;
        static const uint32_t DVideoMemoryBase;
        static const uint32_t DRegisterMemoryBase;

        bool SystemNotStop();
        void CPUThreadExecute();
        void TimerThreadExecute();
        void SystemRun();
        void SystemStop();
        void SystemTerminate();
        void ResetComponents();

    public:
        CRISCVConsole();
        ~CRISCVConsole();

        uint32_t ScreenWidth(){
            return DVideoController->ScreenWidth();
        };

        uint32_t ScreenHeight(){
            return DVideoController->ScreenHeight();
        };

        void Reset();

        void PowerOn();

        void PowerOff();

        void PressDirection(EDirection dir);
        void ReleaseDirection(EDirection dir);

        void PressButton(EButtonNumber button);
        void ReleaseButton(EButtonNumber button);

        void PressCommand();

        bool VideoTimerTick(std::shared_ptr<CGraphicSurface> screensurface);

        bool ProgramFirmware(std::shared_ptr< CDataSource > elfsrc);

        bool InsertCartridge(std::shared_ptr< CDataSource > elfsrc);
        
};

#endif
