#ifndef RISCVCCONSOLECHIPSET_H
#define RISCVCCONSOLECHIPSET_H

#include "RISCVCPU.h"
#include "RegisterBlockMemoryDevice.h"

class CRISCVConsoleChipset{
    public:
        enum class EInterruptSource : uint32_t {Cartridge = 0x1, Video = 0x2, Command = 0x4, DMA1 = 0x10, DMA2 = 0x20};
    protected:
        template <class T> class CInterruptCheckRegister;
        class CInterruptPendingRegister;
        class CDMACommandRegister;
        class CDMAStatusRegister;

        struct SDMAChannel{
            std::shared_ptr< CHardwareRegister< uint32_t > > DDMASource;
            std::shared_ptr< CHardwareRegister< uint32_t > > DDMADestination;
            std::shared_ptr< CDMACommandRegister > DDMACommand;
            std::shared_ptr< CDMAStatusRegister > DDMAStatus;
            uint32_t DNextSource;
            uint32_t DNextDestination;
            bool DUINT32Transfer;
        };

        std::shared_ptr< CRISCVCPU > DCPU;
        std::shared_ptr< CMemoryDevice > DMemory;
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
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineClockPeriod;
        std::shared_ptr< CHardwareRegister< uint32_t > > DVideoClockPeriod;
        std::vector< SDMAChannel > DDMAChannels;

        std::shared_ptr< CRegisterBlockMemoryDevice > DRegisterBlock;

        static const uint32_t DDMAChannelCount;
        static const uint32_t DDMAChannelActive;
        static const uint32_t DDMAChannelSourceError;
        static const uint32_t DDMAChannelDestinationError;
        static const uint32_t DDMAChannelTransferCancelled;
        static const uint32_t DDMAChannelSizeMask;

        void CheckInterrupt(bool istimer);
        bool DMACommandStore(uint32_t index, uint32_t val);

    public:
        CRISCVConsoleChipset(std::shared_ptr< CRISCVCPU > cpu, std::shared_ptr< CMemoryDevice > memory, uint32_t timerus, uint32_t videoms);

        void SetInterruptPending(EInterruptSource source);
        void ClearInterruptPending(EInterruptSource source);

        std::shared_ptr< CRegisterBlockMemoryDevice > RegisterBlock(){
            return DRegisterBlock;
        };

        void IncrementTimer();

        void IncrementDMA();

        void ControllerPress(uint32_t bitfield);

        void ControllerRelease(uint32_t bitfield);

        void ControllerCommandPress();

        void InsertCartridge(uint32_t entry);

        void RemoveCartridge();

        void Reset();
};

#endif
