#ifndef VARIABLETRANSLATOR_H
#define VARIABLETRANSLATOR_H

#include <memory>
#include "MemoryDevice.h"
#include "RISCVCPU.h"
#include "DwarfStructures.h"

class CVariableTranslator{
    public:
        class CProgramValue{
            friend class CVariableTranslator;
            protected:
                std::string DDefault;
                std::string DDefaultConversion;
                std::vector< std::string > DAlternatives;
                std::vector< std::string > DAlternativeConversions;

            public:
                std::string Default() const;
                std::string DefaultConversion() const;
                size_t AlternativeCount() const;
                bool GetAlternativeConversion(size_t index, std::string &value, std::string &conv) const;
        };

        class CProgramState : public std::enable_shared_from_this<CProgramState>{
            friend class CVariableTranslator;
            protected:
                std::string DName;
                std::string DType;
                CProgramValue DValue;
                std::vector< std::shared_ptr<CProgramState> > DChildren;
            public:
                std::string Name() const;
                std::string Type() const;
                CProgramValue Value() const;

                size_t ChildCount() const;
                std::shared_ptr<CProgramState> Child(size_t index) const;
        };

    protected:
        std::shared_ptr< CRISCVCPU > DCPU;
        std::shared_ptr< CMemoryDevice > DMemory;
        size_t DMaxPointerDepth;

        static std::string ConvertDoubleToString(double val);
        static std::string ConvertCharToString(int64_t val);
        static uint64_t GetUnsignedFromBytes(std::vector< uint8_t > &bytes, size_t bitsize, size_t bitoffset);
        static int64_t GetSignedFromBytes(std::vector< uint8_t > &bytes, size_t bitsize, size_t bitoffset);
        static std::string CreateScopeName(std::shared_ptr< CDwarfStructures::SProgrammaticScope > scope);

        bool GetVariableData(const CDwarfStructures::SObjectLocation &location, size_t size, std::vector< uint8_t > &bytes);
        bool SetVariableValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth = 0);
        bool SetVariableBaseValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth);
        bool SetVariableEnumValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth);
        bool SetVariableArrayValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth);
        bool SetVariableStructValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth);
        bool SetVariablePointerValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth);
        void TranslateVariable(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SVariable > variable);

        void PrintState(std::shared_ptr<CProgramState> state, size_t depth);

    public:
        CVariableTranslator(std::shared_ptr< CRISCVCPU > cpu, std::shared_ptr< CMemoryDevice > memory);
        ~CVariableTranslator() = default;

        static const std::string DCompilationUnitTypeString;
        static const std::string DFunctionTypeString;
        static const std::string DBlockTypeString;
        static const std::string DDecimalString;
        static const std::string DHexadecimalString;
        static const std::string DCharacterString;
        static const std::string DFloatString;
        static const std::string DDoubleString;
        static const std::string DEnumTypeString;
        static const std::string DUnknownEnumString;

        size_t MaxPointerDepth() const;
        size_t MaxPointerDepth(size_t depth);

        void TranslateVariables(std::vector< std::shared_ptr<CProgramState> > &state, const std::vector< std::shared_ptr< CDwarfStructures::SProgrammaticScope > > &scopes);

};

#endif
