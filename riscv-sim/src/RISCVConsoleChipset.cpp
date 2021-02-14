#include "RISCVConsoleChipset.h"
#include <cstdio>

const uint32_t CRISCVConsoleChipset::DDMAChannelCount = 2;
const uint32_t CRISCVConsoleChipset::DDMAChannelActive = 0x80000000;
const uint32_t CRISCVConsoleChipset::DDMAChannelSourceError = 0x40000000;
const uint32_t CRISCVConsoleChipset::DDMAChannelDestinationError = 0x20000000;
const uint32_t CRISCVConsoleChipset::DDMAChannelTransferCancelled = 0x10000000;
const uint32_t CRISCVConsoleChipset::DDMAChannelSizeMask = 0x00FFFFFF;
template<class T>
class CRISCVConsoleChipset::CInterruptCheckRegister : public CReadWriteHardwareRegister<T> {
    protected:
        using CReadWriteHardwareRegister<T>::DRegister;
        using CReadWriteHardwareRegister<T>::DDefault;
        CRISCVConsoleChipset &DChipset;
        bool DIsTimer;
    public:
        CInterruptCheckRegister() noexcept = delete;
        CInterruptCheckRegister(CRISCVConsoleChipset &chipset, bool istimer, T val) noexcept : CReadWriteHardwareRegister<T>(val), DChipset(chipset){
            DIsTimer = istimer;
        };

        virtual ~CInterruptCheckRegister(){};

        virtual T operator= (T val) noexcept{
            T Result = CReadWriteHardwareRegister<T>::operator=(val);
            DChipset.CheckInterrupt(DIsTimer);
            return Result;
        };

        virtual void store (T val) noexcept{
            CReadWriteHardwareRegister<T>::store(val);
            DChipset.CheckInterrupt(DIsTimer);
        };

        virtual T fetch_add(T val) noexcept{
            auto Result = CReadWriteHardwareRegister<T>::fetch_add(val);
            DChipset.CheckInterrupt(DIsTimer);
            return Result;
        };

        virtual T fetch_sub(T val) noexcept{
            auto Result = CReadWriteHardwareRegister<T>::fetch_sub(val);
            DChipset.CheckInterrupt(DIsTimer);
            return Result;
        };

        virtual T fetch_and(T val) noexcept{
            auto Result = CReadWriteHardwareRegister<T>::fetch_and(val);
            DChipset.CheckInterrupt(DIsTimer);
            return Result;
        };

        virtual T fetch_or(T val) noexcept{
            auto Result = CReadWriteHardwareRegister<T>::fetch_or(val);
            DChipset.CheckInterrupt(DIsTimer);
            return Result;
        };

        virtual T fetch_xor(T val) noexcept{
            auto Result = CReadWriteHardwareRegister<T>::fetch_xor(val);
            DChipset.CheckInterrupt(DIsTimer);
            return Result;
        };

        virtual T operator++() noexcept{
            auto Result = CReadWriteHardwareRegister<T>::operator++();
            DChipset.CheckInterrupt(DIsTimer);
            return Result;
        };

        virtual T operator++(int) noexcept{
            auto Result = CReadWriteHardwareRegister<T>::operator++(0);
            DChipset.CheckInterrupt(DIsTimer);
            return Result;
        };

        virtual T operator--() noexcept{
            auto Result = CReadWriteHardwareRegister<T>::operator--();
            DChipset.CheckInterrupt(DIsTimer);
            return Result;
        };

        virtual T operator--(int) noexcept{
            auto Result = CReadWriteHardwareRegister<T>::operator--(0);
            DChipset.CheckInterrupt(DIsTimer);
            return Result;
        };

        virtual void reset() noexcept{
            CReadWriteHardwareRegister<T>::store(DDefault);
            DChipset.CheckInterrupt(DIsTimer);
        };
};

class CRISCVConsoleChipset::CInterruptPendingRegister : public CInterruptCheckRegister<uint32_t>{
    using CReadWriteHardwareRegister<uint32_t>::DRegister;
    using CReadWriteHardwareRegister<uint32_t>::DDefault;
    using CInterruptCheckRegister<uint32_t>::DChipset;
    public:
        CInterruptPendingRegister() noexcept = delete;
        CInterruptPendingRegister(CRISCVConsoleChipset &chipset, uint32_t val) noexcept : CInterruptCheckRegister<uint32_t>(chipset,false,val){
        };

        virtual ~CInterruptPendingRegister(){};

        virtual uint32_t operator= (uint32_t val) noexcept{
            uint32_t Result = CReadWriteHardwareRegister<uint32_t>::fetch_and(~val);
            DChipset.CheckInterrupt(false);
            return Result;
        };

        virtual void store (uint32_t val) noexcept{
            CReadWriteHardwareRegister<uint32_t>::fetch_and(~val);
            DChipset.CheckInterrupt(false);
        };

        virtual uint32_t exchange(uint32_t val) noexcept{
            auto Result = CReadWriteHardwareRegister<uint32_t>::fetch_and(~val);
            DChipset.CheckInterrupt(false);
            return Result;
        };

        virtual uint32_t fetch_add(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t fetch_sub(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t operator++() noexcept{
            return load();
        };

        virtual uint32_t operator++(int) noexcept{
            return load();
        };

        virtual uint32_t operator--() noexcept{
            return load();
        };

        virtual uint32_t operator--(int) noexcept{
            return load();
        };
};

class CRISCVConsoleChipset::CDMACommandRegister : public CReadWriteHardwareRegister<uint32_t>{
    protected:
        using CReadWriteHardwareRegister<uint32_t>::DRegister;
        using CReadWriteHardwareRegister<uint32_t>::DDefault;
        CRISCVConsoleChipset &DChipset;
        uint32_t DIndex;
    public:
        CDMACommandRegister() noexcept = delete;
        CDMACommandRegister(CRISCVConsoleChipset &chipset, uint32_t index, uint32_t val) noexcept : CReadWriteHardwareRegister<uint32_t>(val), DChipset(chipset){
            DIndex = index;
        };

        virtual uint32_t operator= (uint32_t val) noexcept{
            if(DChipset.DMACommandStore(DIndex,val)){
                return CReadWriteHardwareRegister<uint32_t>::operator=(val & CRISCVConsoleChipset::DDMAChannelSizeMask);
            }
            return load();
        };

        virtual void store (uint32_t val) noexcept{
            if(DChipset.DMACommandStore(DIndex,val)){
                CReadWriteHardwareRegister<uint32_t>::store(val & CRISCVConsoleChipset::DDMAChannelSizeMask);
            }
        };

        virtual uint32_t fetch_add(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t fetch_sub(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t fetch_and(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t fetch_or(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t fetch_xor(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t operator++() noexcept{
            return load();
        };

        virtual uint32_t operator++(int) noexcept{
            return load();
        };

        virtual uint32_t operator--() noexcept{
            return load();
        };

        virtual uint32_t operator--(int) noexcept{
            return load();
        };

        virtual void reset() noexcept{
            CReadWriteHardwareRegister<uint32_t>::store(DDefault);

        };
};

class CRISCVConsoleChipset::CDMAStatusRegister : public CReadOnlyHardwareRegister<uint32_t> {
    protected:
        using CReadWriteHardwareRegister<uint32_t>::DRegister;

    public:
        CDMAStatusRegister() noexcept = delete;
        CDMAStatusRegister(uint32_t val) noexcept : CReadOnlyHardwareRegister<uint32_t>(val){
        };

        virtual void update(uint32_t val){
            DRegister.store(val);
        };
};


// Code form Effective Modern C++ by Scott Meyers (see Item 10)
template<typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E enumerator) noexcept{
    return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

CRISCVConsoleChipset::CRISCVConsoleChipset(std::shared_ptr< CRISCVCPU > cpu, std::shared_ptr< CMemoryDevice > memory, uint32_t timerus, uint32_t videoms){
    DCPU = cpu;
    DMemory = memory;
    DInterruptEnable = std::make_shared< CInterruptCheckRegister<uint32_t> >(*this,false,0);
    DInterruptPending = std::make_shared< CInterruptPendingRegister >(*this,0);
    DMachineTime = std::make_shared< CInterruptCheckRegister<uint64_t> >(*this, true, 0);
    DMachineTimeCompare = std::make_shared< CInterruptCheckRegister<uint64_t> >(*this, true, -1);
    DMachineTimeLow = std::make_shared< CReadWriteHardwareRegisterLow< uint32_t, uint64_t > >(*DMachineTime.get());
    DMachineTimeHigh = std::make_shared< CReadWriteHardwareRegisterHigh< uint32_t, uint64_t > >(*DMachineTime.get());
    DMachineTimeCompareLow = std::make_shared< CReadWriteHardwareRegisterLow< uint32_t, uint64_t > >(*DMachineTimeCompare.get());
    DMachineTimeCompareHigh = std::make_shared< CReadWriteHardwareRegisterHigh< uint32_t, uint64_t > >(*DMachineTimeCompare.get());
    DControllerState = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(0);
    DCartridgeState = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(0);
    DMachineClockPeriod = std::make_shared< CReadOnlyHardwareRegister< uint32_t > >(timerus);
    DVideoClockPeriod = std::make_shared< CReadOnlyHardwareRegister< uint32_t > >(videoms);

    DRegisterBlock = std::make_shared< CRegisterBlockMemoryDevice >();
    DRegisterBlock->AttachRegister(DInterruptEnable);
    DRegisterBlock->AttachRegister(DInterruptPending);
    DRegisterBlock->AttachRegister(DMachineTimeLow);
    DRegisterBlock->AttachRegister(DMachineTimeHigh);
    DRegisterBlock->AttachRegister(DMachineTimeCompareLow);
    DRegisterBlock->AttachRegister(DMachineTimeCompareHigh);
    DRegisterBlock->AttachRegister(DControllerState);
    DRegisterBlock->AttachRegister(DCartridgeState);

    DDMAChannels.resize(DDMAChannelCount);
    {
        uint32_t Index = 0;
        for(auto &DMAChannel : DDMAChannels){
            DMAChannel.DDMASource = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(0);
            DRegisterBlock->AttachRegister(DMAChannel.DDMASource);
            DMAChannel.DDMADestination = std::make_shared< CReadWriteHardwareRegister< uint32_t > >(0);
            DRegisterBlock->AttachRegister(DMAChannel.DDMADestination);
            DMAChannel.DDMACommand =  std::make_shared< CDMACommandRegister >(*this, Index, 0);
            DRegisterBlock->AttachRegister(DMAChannel.DDMACommand);
            DMAChannel.DDMAStatus = std::make_shared< CDMAStatusRegister >(0);
            DRegisterBlock->AttachRegister(DMAChannel.DDMAStatus);
            Index++;
        }
    }
    DRegisterBlock->AttachRegister(DMachineClockPeriod);
    DRegisterBlock->AttachRegister(DVideoClockPeriod);
}

void CRISCVConsoleChipset::CheckInterrupt(bool istimer){
    if(istimer){
        if(DMachineTime->load() >= DMachineTimeCompare->load()){
            DCPU->SetInterruptPending(CRISCVCPU::EInterruptSource::Timer);
        }
        else{
            DCPU->ClearInterruptPending(CRISCVCPU::EInterruptSource::Timer);
        }
    }
    else{
        if(DInterruptEnable->load() & DInterruptPending->load()){
            DCPU->SetInterruptPending(CRISCVCPU::EInterruptSource::External);
        }
        else{
            DCPU->ClearInterruptPending(CRISCVCPU::EInterruptSource::External);
        }
    }
}

bool CRISCVConsoleChipset::DMACommandStore(uint32_t index, uint32_t val){
    if(index < DDMAChannelCount){
        if(val & DDMAChannelActive){
            if(val & DDMAChannelSizeMask){
                if(0 == (DDMAChannels[index].DDMAStatus->load() & DDMAChannelActive)){
                    DDMAChannels[index].DNextSource = DDMAChannels[index].DDMASource->load();
                    DDMAChannels[index].DNextDestination = DDMAChannels[index].DDMADestination->load();
                    DDMAChannels[index].DDMAStatus->update(val & (DDMAChannelActive | DDMAChannelSizeMask));
                    DDMAChannels[index].DUINT32Transfer = 0 == ((DDMAChannels[index].DNextSource | DDMAChannels[index].DNextDestination | val) & (sizeof(uint32_t)-1));
                    return true;
                }
            }
            else{
                uint32_t NewStatus = DDMAChannels[index].DDMAStatus->load();
                if(NewStatus & DDMAChannelActive){
                    NewStatus |= DDMAChannelTransferCancelled;
                    NewStatus &= ~DDMAChannelActive;
                    DDMAChannels[index].DDMAStatus->update(NewStatus);
                    SetInterruptPending(static_cast<EInterruptSource>(to_underlying(EInterruptSource::DMA1)<<index));
                    return true;
                }
            }
        }
    }
    return false;
}

void CRISCVConsoleChipset::SetInterruptPending(EInterruptSource source){
    DInterruptPending->fetch_or(to_underlying(source));
}

void CRISCVConsoleChipset::ClearInterruptPending(EInterruptSource source){
    DInterruptPending->store(to_underlying(source));
}

void CRISCVConsoleChipset::IncrementTimer(){
    DMachineTime->operator++();
}

void CRISCVConsoleChipset::IncrementDMA(){
    for(uint32_t Index = 0; Index < DDMAChannelCount; Index++){
        if(DDMAChannels[Index].DDMAStatus->load() & DDMAChannelActive){
            bool ReadData = false;
            try{
                if(DDMAChannels[Index].DUINT32Transfer){
                    uint32_t Data = DMemory->LoadUINT32(DDMAChannels[Index].DNextSource);
                    ReadData = true;
                    DMemory->StoreUINT32(DDMAChannels[Index].DNextDestination, Data);
                    DDMAChannels[Index].DNextSource += sizeof(uint32_t);
                    DDMAChannels[Index].DNextDestination += sizeof(uint32_t);
                    uint32_t NewStatus = DDMAChannels[Index].DDMAStatus->load();
                    NewStatus -= sizeof(uint32_t);
                    if(0 == (NewStatus & DDMAChannelSizeMask)){
                        NewStatus &= ~DDMAChannelActive;
                        DDMAChannels[Index].DDMAStatus->update(NewStatus);
                        SetInterruptPending(static_cast<EInterruptSource>(to_underlying(EInterruptSource::DMA1)<<Index));
                    }
                    else{
                        DDMAChannels[Index].DDMAStatus->update(NewStatus);
                    }
                }
                else{
                    uint32_t NewStatus = DDMAChannels[Index].DDMAStatus->load();
                    if((NewStatus & DDMAChannelSizeMask) <= sizeof(uint32_t)){
                        // Do final transfer
                        const uint8_t *Data = DMemory->LoadData(DDMAChannels[Index].DNextSource, NewStatus & DDMAChannelSizeMask);
                        ReadData = true;
                        DMemory->StoreData(DDMAChannels[Index].DNextDestination, Data, NewStatus & DDMAChannelSizeMask);
                        DDMAChannels[Index].DDMAStatus->update(0);
                        SetInterruptPending(static_cast<EInterruptSource>(to_underlying(EInterruptSource::DMA1)<<Index));
                    }
                    else if(DDMAChannels[Index].DNextDestination & (sizeof(uint32_t)-1)){
                        // Align future transfers to destination uint32_t
                        uint32_t TransferSize = sizeof(uint32_t) - (DDMAChannels[Index].DNextDestination & (sizeof(uint32_t)-1));
                        const uint8_t *Data = DMemory->LoadData(DDMAChannels[Index].DNextSource, TransferSize);
                        ReadData = true;
                        DMemory->StoreData(DDMAChannels[Index].DNextDestination, Data, TransferSize);
                        DDMAChannels[Index].DNextSource += TransferSize;
                        DDMAChannels[Index].DNextDestination += TransferSize;
                        NewStatus -= TransferSize;
                        DDMAChannels[Index].DDMAStatus->update(NewStatus);
                        DDMAChannels[Index].DUINT32Transfer = 0 == ((DDMAChannels[Index].DNextSource | DDMAChannels[Index].DNextDestination | NewStatus) & (sizeof(uint32_t)-1));
                    }
                    else{
                        uint32_t Data = DMemory->LoadUINT32(DDMAChannels[Index].DNextSource);
                        ReadData = true;
                        DMemory->StoreUINT32(DDMAChannels[Index].DNextDestination, Data);
                        DDMAChannels[Index].DNextSource += sizeof(uint32_t);
                        DDMAChannels[Index].DNextDestination += sizeof(uint32_t);
                        NewStatus -= sizeof(uint32_t);
                        DDMAChannels[Index].DDMAStatus->update(NewStatus);
                    }
                }
            }
            catch(std::out_of_range &e){
                uint32_t NewStatus = DDMAChannels[Index].DDMAStatus->load();
                NewStatus |= ReadData ? DDMAChannelDestinationError : DDMAChannelSourceError;
                DDMAChannels[Index].DDMAStatus->update(NewStatus);
                SetInterruptPending(static_cast<EInterruptSource>(to_underlying(EInterruptSource::DMA1)<<Index));
            }
        }
    }
}

void CRISCVConsoleChipset::ControllerPress(uint32_t bitfield){
    DControllerState->fetch_or(bitfield);
}
    
void CRISCVConsoleChipset::ControllerRelease(uint32_t bitfield){
    DControllerState->fetch_and(~bitfield);
}

void CRISCVConsoleChipset::ControllerCommandPress(){
    SetInterruptPending(EInterruptSource::Command);
}

void CRISCVConsoleChipset::InsertCartridge(uint32_t entry){
    DCartridgeState->store(entry | 0x1);
    SetInterruptPending(EInterruptSource::Cartridge);
}

void CRISCVConsoleChipset::RemoveCartridge(){
    DCartridgeState->store(0x0);
    SetInterruptPending(EInterruptSource::Cartridge);
}

void CRISCVConsoleChipset::Reset(){
    DInterruptEnable->reset();
    DInterruptPending->reset();
    DMachineTime->reset();
    DMachineTimeCompare->reset();
}