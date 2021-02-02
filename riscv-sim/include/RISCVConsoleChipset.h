#ifndef RISCVCCONSOLECHIPSET_H
#define RISCVCCONSOLECHIPSET_H

#include "RISCVCPU.h"

class CRISCVConsoleChipset{
    public:
        enum class EInterruptSource : uint32_t {Cartridge = 0x1, Video = 0x2, Command = 0x4};
    protected:
        template <class T> class CInterruptCheckRegister;
        class CInterruptPendingRegister;

        std::shared_ptr< CRISCVCPU > DCPU;
        std::shared_ptr< CHardwareRegister< uint32_t > > DInterruptEnable;
        std::shared_ptr< CHardwareRegister< uint32_t > > DInterruptPending;
        std::shared_ptr< CHardwareRegister< uint64_t > > DMachineTime;
        std::shared_ptr< CHardwareRegister< uint64_t > > DMachineTimeCompare;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineTimeLow;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineTimeHigh;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineTimeCompareLow;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineTimeCompareHigh;
        std::shared_ptr< CHardwareRegister< uint32_t > > DControllerState;
        std::shared_ptr< CHardwareRegister< uint32_t > > DCartridgeState;

        void CheckInterrupt(bool istimer);

    public:
        CRISCVConsoleChipset(std::shared_ptr< CRISCVCPU > cpu);

        void SetInterruptPending(EInterruptSource source);
        void ClearInterruptPending(EInterruptSource source);

        std::shared_ptr< CHardwareRegister< uint32_t > > InterruptEnable(){
            return DInterruptEnable;
        };

        std::shared_ptr< CHardwareRegister< uint32_t > > InterruptPending(){
            return DInterruptPending;
        };

        void IncrementTimer();

        std::shared_ptr< CHardwareRegister< uint32_t > > MachineTimeLow(){
            return DMachineTimeLow;
        };

        std::shared_ptr< CHardwareRegister< uint32_t > > MachineTimeHigh(){
            return DMachineTimeHigh;
        };

        std::shared_ptr< CHardwareRegister< uint32_t > > MachineTimeCompareLow(){
            return DMachineTimeCompareLow;
        };

        std::shared_ptr< CHardwareRegister< uint32_t > > MachineTimeCompareHigh(){
            return DMachineTimeCompareHigh;
        };

        std::shared_ptr< CHardwareRegister< uint32_t > > ControllerState(){
            return DControllerState;
        };

        std::shared_ptr< CHardwareRegister< uint32_t > > CartridgeState(){
            return DCartridgeState;
        };

        void ControllerPress(uint32_t bitfield);

        void ControllerRelease(uint32_t bitfield);

        void ControllerCommandPress();

        void InsertCartridge(uint32_t entry);

        void RemoveCartridge();

        void Reset();
};

#endif
