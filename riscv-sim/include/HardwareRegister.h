#ifndef HARDWAREREGISTER_H
#define HARDWAREREGISTER_H

#include <atomic>
#include <memory>
#include <cstdint>


template<class T>
class CHardwareRegister{
    public:
        CHardwareRegister() noexcept = default;
        CHardwareRegister(T val) noexcept{
            
        };

        virtual ~CHardwareRegister(){};

        virtual T operator= (T val) noexcept{
            return 0;
        };

        virtual operator T() const noexcept{
            return 0;
        };

        virtual void store (T val) noexcept{
            
        };

        virtual T load() noexcept{
            return 0;
        };

        virtual T exchange(T val) noexcept{
            return 0;
        };

        virtual T fetch_add(T val) noexcept{
            return 0;
        };

        virtual T fetch_sub(T val) noexcept{
            return 0;
        };

        virtual T fetch_and(T val) noexcept{
            return 0;
        };

        virtual T fetch_or(T val) noexcept{
            return 0;
        };

        virtual T fetch_xor(T val) noexcept{
            return 0;
        };

        virtual T operator++() noexcept{
            return 0;
        };

        virtual T operator++(int) noexcept{
            return 0;
        };

        virtual T operator--() noexcept{
            return 0;
        };

        virtual T operator--(int) noexcept{
            return 0;
        };

        virtual void reset() noexcept{
            
        };
};

template<class T>
class CReadWriteHardwareRegister : public CHardwareRegister<T>{
    protected:
        std::atomic<T> DRegister;
        T DDefault = 0;
    public:
        CReadWriteHardwareRegister() noexcept = default;
        CReadWriteHardwareRegister(T val) noexcept : DRegister(val), DDefault(val){
            
        };

        virtual ~CReadWriteHardwareRegister(){};

        virtual T operator= (T val) noexcept{
            return DRegister = val;
        };

        virtual operator T() const noexcept{
            return DRegister;
        };

        virtual void store (T val) noexcept{
            DRegister.store(val);
        };

        virtual T load() noexcept{
            return DRegister.load();
        };

        virtual T exchange(T val) noexcept{
            return DRegister.exchange(val);
        };

        virtual T fetch_add(T val) noexcept{
            return DRegister.fetch_add(val);
        };

        virtual T fetch_sub(T val) noexcept{
            return DRegister.fetch_sub(val);
        };

        virtual T fetch_and(T val) noexcept{
            return DRegister.fetch_and(val);
        };

        virtual T fetch_or(T val) noexcept{
            return DRegister.fetch_or(val);
        };

        virtual T fetch_xor(T val) noexcept{
            return DRegister.fetch_xor(val);
        };

        virtual T operator++() noexcept{
            return ++DRegister;
        };

        virtual T operator++(int) noexcept{
            return DRegister++;
        };

        virtual T operator--() noexcept{
            return --DRegister;
        };
        virtual T operator--(int) noexcept{
            return DRegister--;
        };

        virtual void reset() noexcept{
            DRegister.store(DDefault);
        };
};


template<class T>
class CReadOnlyHardwareRegister : public CReadWriteHardwareRegister<T>{
    protected:
        using CReadWriteHardwareRegister<T>::DRegister;
        using CReadWriteHardwareRegister<T>::DDefault;
    public:
        CReadOnlyHardwareRegister() noexcept = default;
        CReadOnlyHardwareRegister(T val) noexcept : CReadWriteHardwareRegister<T>(val){
            
        };

        virtual ~CReadOnlyHardwareRegister(){};

        virtual T operator= (T val) noexcept{
            return DRegister;
        };

        virtual void store (T val) noexcept{

        };

        virtual T exchange(T val) noexcept{
            return DRegister;
        };

        virtual T fetch_add(T val) noexcept{
            return DRegister;
        };

        virtual T fetch_sub(T val) noexcept{
            return DRegister;
        };

        virtual T fetch_and(T val) noexcept{
            return DRegister;
        };

        virtual T fetch_or(T val) noexcept{
            return DRegister;
        };

        virtual T fetch_xor(T val) noexcept{
            return DRegister;
        };

        virtual T operator++() noexcept{
            return DRegister;
        };

        virtual T operator++(int) noexcept{
            return DRegister;
        };

        virtual T operator--() noexcept{
            return DRegister;
        };

        virtual T operator--(int) noexcept{
            return DRegister;
        };

        virtual void reset() noexcept{

        };
};

template<class T>
class CMaskedReadWriteHardwareRegister : public CReadWriteHardwareRegister<T>{
    using CReadWriteHardwareRegister<T>::DRegister;
    using CReadWriteHardwareRegister<T>::DDefault;
    protected:
        T DMask;
    public:
        CMaskedReadWriteHardwareRegister() noexcept = delete;
        CMaskedReadWriteHardwareRegister(T val, T mask) noexcept : CReadWriteHardwareRegister<T>(val & mask){
            DMask = mask;
        };

        virtual ~CMaskedReadWriteHardwareRegister(){};

        virtual T operator= (T val) noexcept{
            return DRegister = val & DMask;
        };

        virtual void store (T val) noexcept{
            DRegister.store(val & DMask);
        };

        virtual T exchange(T val) noexcept{
            return DRegister.exchange(val & DMask);
        };

        virtual T fetch_add(T val) noexcept{
            return DRegister.load();
        };

        virtual T fetch_sub(T val) noexcept{
            return DRegister.load();
        };

        virtual T fetch_or(T val) noexcept{
            return DRegister.fetch_or(val & DMask);
        };

        virtual T fetch_xor(T val) noexcept{
            return DRegister.fetch_xor(val & DMask);
        };

        virtual T operator++() noexcept{
            return DRegister.load();
        };

        virtual T operator++(int) noexcept{
            return DRegister.load();
        };

        virtual T operator--() noexcept{
            return DRegister.load();
        };

        virtual T operator--(int) noexcept{
            return DRegister.load();
        };
};

template<class H, class T>
class CReadWriteHardwareRegisterLow;

template<class H, class T>
class CReadWriteHardwareRegisterHigh;

template<>
class CReadWriteHardwareRegisterLow<uint32_t, uint64_t> : public CHardwareRegister< uint32_t >{
    protected:
        CHardwareRegister< uint64_t > &DMainRegister;
    public:

        CReadWriteHardwareRegisterLow(CHardwareRegister< uint64_t > &mainreg) : DMainRegister(mainreg){

        };

        virtual uint32_t operator= (uint32_t val) noexcept{
            uint64_t Original = DMainRegister.load();
            DMainRegister.store((Original & 0xFFFFFFFF00000000ULL) | val);
            return Original;
        };

        virtual operator uint32_t() const noexcept{
            return DMainRegister.load();
        };

        virtual void store (uint32_t val) noexcept{
            uint64_t Original = DMainRegister.load();
            DMainRegister.store((Original & 0xFFFFFFFF00000000ULL) | val);
        };

        virtual uint32_t load() noexcept{
            return DMainRegister.load();
        };

        virtual uint32_t exchange(uint32_t val) noexcept{
            uint64_t Original = DMainRegister.load();
            DMainRegister.store((Original & 0xFFFFFFFF00000000ULL) | val);
            return Original;
        };

        virtual uint32_t fetch_add(uint32_t val) noexcept{
            return DMainRegister.load();
        };

        virtual uint32_t fetch_sub(uint32_t val) noexcept{
            return DMainRegister.load();
        };

        virtual uint32_t fetch_and(uint32_t val) noexcept{
            return DMainRegister.fetch_and(0xFFFFFFFF00000000ULL | val);
        };

        virtual uint32_t fetch_or(uint32_t val) noexcept{
            return DMainRegister.fetch_or(val);
        };

        virtual uint32_t fetch_xor(uint32_t val) noexcept{
            return DMainRegister.fetch_xor(val);
        };

        virtual uint32_t operator++() noexcept{
            return DMainRegister.load();
        };

        virtual uint32_t operator++(int) noexcept{
            return DMainRegister.load();
        };

        virtual uint32_t operator--() noexcept{
            return DMainRegister.load();
        };

        virtual uint32_t operator--(int) noexcept{
            return DMainRegister.load();
        };

        virtual void reset() noexcept{
            
        };
};

template<>
class CReadWriteHardwareRegisterHigh<uint32_t, uint64_t> : public CHardwareRegister< uint32_t >{
    protected:
        CHardwareRegister< uint64_t > &DMainRegister;
    public:

        CReadWriteHardwareRegisterHigh(CHardwareRegister< uint64_t > &mainreg) : DMainRegister(mainreg){

        };

        virtual uint32_t operator= (uint32_t val) noexcept{
            uint64_t Original = DMainRegister.load();
            DMainRegister.store((Original & 0xFFFFFFFFULL) | (uint64_t(val)<<32));
            return Original>>32;
        };

        virtual operator uint32_t() const noexcept{
            return DMainRegister.load()>>32;
        };

        virtual void store (uint32_t val) noexcept{
            uint64_t Original = DMainRegister.load();
            DMainRegister.store((Original & 0xFFFFFFFFULL) | (uint64_t(val)<<32));
        };

        virtual uint32_t load() noexcept{
            return DMainRegister.load()>>32;
        };

        virtual uint32_t exchange(uint32_t val) noexcept{
            uint64_t Original = DMainRegister.load();
            DMainRegister.store((Original & 0xFFFFFFFFULL) | (uint64_t(val)<<32));
            return Original>>32;
        };

        virtual uint32_t fetch_add(uint32_t val) noexcept{
            return DMainRegister.load()>>32;
        };

        virtual uint32_t fetch_and(uint32_t val) noexcept{
            return DMainRegister.fetch_and(0xFFFFFFFFULL | (uint64_t(val)<<32));
        };

        virtual uint32_t fetch_or(uint32_t val) noexcept{
            return DMainRegister.fetch_or((uint64_t(val)<<32));
        };

        virtual uint32_t fetch_xor(uint32_t val) noexcept{
            return DMainRegister.fetch_xor((uint64_t(val)<<32));
        };

        virtual uint32_t operator++() noexcept{
            return DMainRegister.load()>>32;
        };

        virtual uint32_t operator++(int) noexcept{
            return DMainRegister.load()>>32;
        };

        virtual uint32_t operator--() noexcept{
            return DMainRegister.load()>>32;
        };

        virtual uint32_t operator--(int) noexcept{
            return DMainRegister.load()>>32;
        };

        virtual void reset() noexcept{
            
        };
};


#endif
