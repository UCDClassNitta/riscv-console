#ifndef RISCVCPU_H
#define RISCVCPU_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "HardwareRegister.h"
#include "MemoryDevice.h"

class CRISCVCPU : public std::enable_shared_from_this<CRISCVCPU> {
    public:
        enum class EInterruptSource : uint32_t {Software = 0x8, Timer = 0x80, External = 0x800};
        class CInstruction{
            protected:
                uint32_t DAddress;
                uint32_t DRawInstruction;

                static const std::string DRegisterNames[];
                static const uint32_t DInstructionAlignment;

            public:
                CInstruction(uint32_t addr, uint32_t raw){
                    DAddress = addr;
                    DRawInstruction = raw;
                };
                virtual ~CInstruction(){};

                uint32_t Address() const{
                    return DAddress;
                };

                uint32_t RawInstruction() const{
                    return DRawInstruction;
                };

                virtual bool Execute() = 0;

                virtual std::string ToString() const = 0;

                static std::string RegisterName(uint32_t index);

                static std::shared_ptr< CInstruction > DecodeInstruction(uint32_t addr, uint32_t raw, std::shared_ptr< CRISCVCPU > cpu);
        };

        class CInstructionCache{
            public:
                virtual ~CInstructionCache(){};
                virtual std::shared_ptr< CInstruction > Fetch(uint32_t addr) = 0;
                virtual void Insert(std::shared_ptr< CInstruction > inst) = 0;
                virtual void Reset() = 0;
                virtual void FlushRange(uint32_t addr, uint32_t size) = 0;
        };

    protected:
        std::shared_ptr< CHardwareRegister< uint32_t > > DProgramCounter;
        std::vector< std::shared_ptr< CHardwareRegister< uint32_t > > > DRegisters;
        std::unordered_map< uint32_t, std::shared_ptr< CHardwareRegister< uint32_t >  > > DControlStatusRegisters;
        std::unordered_map< uint32_t, std::string > DControlStatusRegisterNames;
        std::vector< uint32_t> DControlStatusRegisterKeys;
        std::shared_ptr< CHardwareRegister< uint64_t > > DCycleRegister;
        std::shared_ptr< CHardwareRegister< uint64_t > > DRetiredInstructionRegister;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineStatusRegister;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineInterruptPendingRegister;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineInterruptEnableRegister;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineExceptionProgramCounterRegister;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineCauseRegister;
        std::shared_ptr< CHardwareRegister< uint32_t > > DMachineTrapVectorRegister;
        std::shared_ptr< CMemoryDevice > DMemory;
        std::shared_ptr< CInstructionCache > DInstructionCache;
        static const size_t DRegisterCount;

        void CheckInterrupts();
        void InvokeTrap(uint32_t cause, bool interrupt);

    public:
        CRISCVCPU(std::shared_ptr< CMemoryDevice > memory, std::shared_ptr< CInstructionCache > icache = nullptr);

        uint32_t ProgramCounter() const;
        uint32_t ProgramCounter(uint32_t newpc);

        static size_t RegisterCount(){
            return DRegisterCount;
        };

        uint32_t Register(uint32_t index) const;

        uint64_t RetiredInstructionCount() const;

        bool ExecuteInstruction();

        void SetInterruptPending(EInterruptSource source);
        void ClearInterruptPending(EInterruptSource source);

        std::shared_ptr< CInstruction > DecodeInstruction(uint32_t addr);
        void OutputCSRs();

        const std::vector< uint32_t> &ControlStatusRegisterKeys() const{
            return DControlStatusRegisterKeys;
        };

        uint32_t ControlStatusRegister(uint32_t index) const;

        std::string ControlStatusRegisterName(uint32_t index) const;

        void Reset();
};

#endif
