#include "RISCVConsoleChipset.h"
#include <cstdio>

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

// Code form Effective Modern C++ by Scott Meyers (see Item 10)
template<typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E enumerator) noexcept{
    return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

CRISCVConsoleChipset::CRISCVConsoleChipset(std::shared_ptr< CRISCVCPU > cpu){
    DCPU = cpu;
    DInterruptEnable = std::make_shared< CInterruptCheckRegister<uint32_t> >(*this,false,0);
    DInterruptPending = std::make_shared< CInterruptPendingRegister >(*this,0);
    DMachineTime = std::make_shared< CInterruptCheckRegister<uint64_t> >(*this, true, 0);
    DMachineTimeCompare = std::make_shared< CInterruptCheckRegister<uint64_t> >(*this, true, -1);
    DMachineTimeLow = std::make_shared< CReadWriteHardwareRegisterLow< uint32_t, uint64_t > >(*DMachineTime.get());
    DMachineTimeHigh = std::make_shared< CReadWriteHardwareRegisterHigh< uint32_t, uint64_t > >(*DMachineTime.get());
    DMachineTimeCompareLow = std::make_shared< CReadWriteHardwareRegisterLow< uint32_t, uint64_t > >(*DMachineTimeCompare.get());
    DMachineTimeCompareHigh = std::make_shared< CReadWriteHardwareRegisterHigh< uint32_t, uint64_t > >(*DMachineTimeCompare.get());
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

void CRISCVConsoleChipset::SetInterruptPending(EInterruptSource source){
    DInterruptPending->fetch_or(to_underlying(source));
}

void CRISCVConsoleChipset::ClearInterruptPending(EInterruptSource source){
    DInterruptPending->store(to_underlying(source));
}

void CRISCVConsoleChipset::IncrementTimer(){
    DMachineTime->operator++();
}

void CRISCVConsoleChipset::Reset(){
    DInterruptEnable->reset();
    DInterruptPending->reset();
    DMachineTime->reset();
    DMachineTimeCompare->reset();
}