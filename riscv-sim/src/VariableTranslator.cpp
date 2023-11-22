#include "VariableTranslator.h"
#include <sstream>
#include <iomanip>
#include <limits>
#include <cctype>

const std::string CVariableTranslator::DCompilationUnitTypeString = "__COMPILATION_UNIT__";
const std::string CVariableTranslator::DFunctionTypeString = "__FUNCTION__";
const std::string CVariableTranslator::DBlockTypeString = "__BLOCK__";
const std::string CVariableTranslator::DDecimalString = "dec";
const std::string CVariableTranslator::DHexadecimalString = "hex";
const std::string CVariableTranslator::DCharacterString = "char";
const std::string CVariableTranslator::DFloatString = "float";
const std::string CVariableTranslator::DDoubleString = "double";
const std::string CVariableTranslator::DEnumTypeString = "enum";
const std::string CVariableTranslator::DUnknownEnumString = "/** UNKNOWN **/";

std::string CVariableTranslator::CProgramValue::Default() const{
    return DDefault;
}

std::string CVariableTranslator::CProgramValue::DefaultConversion() const{
    return DDefaultConversion;
}

size_t CVariableTranslator::CProgramValue::AlternativeCount() const{
    return DAlternatives.size();
}

bool CVariableTranslator::CProgramValue::GetAlternativeConversion(size_t index, std::string &value, std::string &conv) const{
    if((0 <= index)&&(index < DAlternatives.size())){
        value = DAlternatives[index];
        conv = DAlternativeConversions[index];
        return true;
    }
    return false;
}

std::string CVariableTranslator::CProgramState::Name() const{
    return DName;
}

std::string CVariableTranslator::CProgramState::Type() const{
    return DType;
}

CVariableTranslator::CProgramValue CVariableTranslator::CProgramState::Value() const{
    return DValue;
}

size_t CVariableTranslator::CProgramState::ChildCount() const{
    return DChildren.size();
}

std::shared_ptr<CVariableTranslator::CProgramState> CVariableTranslator::CProgramState::Child(size_t index) const{
    if((0 <= index)&&(index < DChildren.size())){
        return DChildren[index];
    }
    return nullptr;
}
    
CVariableTranslator::CVariableTranslator(std::shared_ptr< CRISCVCPU > cpu, std::shared_ptr< CMemoryDevice > memory){
    DCPU = cpu;
    DMemory = memory;
    DMaxPointerDepth = 1;
}

size_t CVariableTranslator::MaxPointerDepth() const{
    return DMaxPointerDepth;
}

size_t CVariableTranslator::MaxPointerDepth(size_t depth){
    DMaxPointerDepth = depth;
    return DMaxPointerDepth;
}


// Based off of https://codereview.stackexchange.com/questions/90565/converting-a-double-to-a-stdstring-without-scientific-notation
std::string CVariableTranslator::ConvertDoubleToString(double val){
    std::ostringstream OutStringStream;
    OutStringStream<<val;
    std::string DefaultString = OutStringStream.str();
    OutStringStream.str(std::string());
    OutStringStream.precision(std::numeric_limits<double>::digits10);
    OutStringStream<<std::fixed<<val;
    std::string PaddingRemovedString = OutStringStream.str();

    // Remove padding
    // This must be done in two steps because of numbers like 700.00
    auto Position = PaddingRemovedString.find_last_not_of("0");
    if(std::string::npos != Position){
        PaddingRemovedString.erase(Position+1);
    }
    Position = PaddingRemovedString.find_last_not_of(".");
    if(std::string::npos != Position){
        PaddingRemovedString.erase(Position+1);
    }

    return DefaultString.length() < PaddingRemovedString.length() ? DefaultString : PaddingRemovedString;
}

std::string CVariableTranslator::ConvertCharToString(int64_t val){
    if(isprint(val)){
        char Buffer[] = "' '";
        Buffer[1] = val;
        return Buffer;
    }
    else{
        std::unordered_map<int64_t,std::string> Conversions = {{0x00,"'\\0'"},{0x07,"'\\a'"},{0x08,"'\\b'"},{0x09,"'\\t'"},
                                                                {0x0A,"'\\n'"},{0x0B,"'\\v'"},{0x0C,"'\\f'"},{0x0D,"'\\r'"}};
        auto Search = Conversions.find(val);
        if(Conversions.end() != Search){
            return Search->second;
        }
        std::stringstream OutStream;
        OutStream<<"'\\x"<<std::hex<<std::setw(2)<<std::setfill('0')<<val<<"'";
        return OutStream.str();
    }
}

uint64_t CVariableTranslator::GetUnsignedFromBytes(std::vector< uint8_t > &bytes, size_t bitsize, size_t bitoffset){
    uint64_t Value = 0;
    for(auto Iter = bytes.rbegin(); Iter != bytes.rend(); Iter++){
        Value <<= 8;
        Value |= *Iter;
    }
    if(bitsize != bytes.size() * 8){
        Value >>= bitoffset;
        Value &= ((uint64_t(0x1)<<bitsize)-1); 
    }
    return Value;
}

int64_t CVariableTranslator::GetSignedFromBytes(std::vector< uint8_t > &bytes, size_t bitsize, size_t bitoffset){
    int64_t Value = bytes.back() & 0x80 ? -1 : 0;
    for(auto Iter = bytes.rbegin(); Iter != bytes.rend(); Iter++){
        Value <<= 8;
        Value |= *Iter;
    }
    if(bitsize != bytes.size() * 8){
        Value >>= bitoffset;
        Value &= ((int64_t(0x1)<<bitsize)-1); 
        if(Value & (int64_t(0x1)<<(bitsize-1))){
            // Sign extend
            Value |= int64_t(-1) & (~((int64_t(0x1)<<bitsize)-1));
        }
    }
    return Value;
}

std::string CVariableTranslator::CreateScopeName(std::shared_ptr< CDwarfStructures::SProgrammaticScope > scope){
    std::stringstream OutStream;

    OutStream<<"{"<<std::hex<<std::setw(8)<<std::setfill('0')<<scope->DPCRange.DLowPC<<"-"<<std::setw(8)<<std::setfill('0')<<scope->DPCRange.DHighPC<<"}";
    return OutStream.str();
}

bool CVariableTranslator::GetVariableData(const CDwarfStructures::SObjectLocation &location, size_t size, std::vector< uint8_t > &bytes){
    const uint8_t *MemoryBase;
    bytes.clear();
    switch(location.DType){
        case DW_OP::addr:           
        case DW_OP::deref:      // system dependent
                                MemoryBase = DMemory->LoadData(location.DAddress,size);
                                if(MemoryBase){
                                    bytes.assign(MemoryBase, MemoryBase + size);
                                    return true;
                                }
                                return false;
                                break;

        case DW_OP::const1u:
        case DW_OP::const2u:
        case DW_OP::const4u:
        case DW_OP::const8u:
        case DW_OP::constu:     // Iterate through bytes of DAddress
                                for(size_t Index = 0; Index < size; Index++){
                                    bytes.push_back(location.DAddress>>(Index * 8));
                                }
                                break;
        case DW_OP::const1s:
        case DW_OP::const2s:    
        case DW_OP::const4s:    
        case DW_OP::const8s:
        case DW_OP::consts:     // Iterate through bytes of DOffset
                                for(size_t Index = 0; Index < size; Index++){
                                    bytes.push_back(location.DOffset>>(Index * 8));
                                }
                                break;
        case DW_OP::breg0:
        case DW_OP::breg1:
        case DW_OP::breg2:
        case DW_OP::breg3:
        case DW_OP::breg4:
        case DW_OP::breg5:
        case DW_OP::breg6:
        case DW_OP::breg7:
        case DW_OP::breg8:
        case DW_OP::breg9:
        case DW_OP::breg10:
        case DW_OP::breg11:
        case DW_OP::breg12:
        case DW_OP::breg13:
        case DW_OP::breg14:
        case DW_OP::breg15:
        case DW_OP::breg16:
        case DW_OP::breg17:
        case DW_OP::breg18:
        case DW_OP::breg19:
        case DW_OP::breg20:
        case DW_OP::breg21:
        case DW_OP::breg22:
        case DW_OP::breg23:
        case DW_OP::breg24:
        case DW_OP::breg25:
        case DW_OP::breg26:
        case DW_OP::breg27:
        case DW_OP::breg28:
        case DW_OP::breg29:
        case DW_OP::breg30:
        case DW_OP::breg31:
        case DW_OP::fbreg:     {
                                    // frame pointer is register 8
                                    uint32_t RegisterNumber = DW_OP::fbreg == location.DType ? 8 : uint32_t(location.DType) - uint32_t(DW_OP::breg0);
                                    uint32_t Address = DCPU->Register(RegisterNumber) + location.DOffset;
                                    MemoryBase = DMemory->LoadData(Address,size);
                                    if(MemoryBase){
                                        bytes.assign(MemoryBase, MemoryBase + size);
                                        return true;
                                    }
                                    return false;
                                }
                                break;     

        default:                break;
                    
    }
    return false;
}

bool CVariableTranslator::SetVariableValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth){
    if(datatype->IsConst() || datatype->IsVolatile()){
        // Just skip the qualifiers
        return SetVariableValue(state, datatype->DReferencedType,bytes,pointerdepth);
    }
    else if(datatype->IsPointer()){
        return SetVariablePointerValue(state,datatype,bytes,pointerdepth);
    }
    else if(datatype->IsTypedef()){
        return SetVariableValue(state,datatype->DReferencedType,bytes,pointerdepth);
    }
    else if(datatype->IsStruct()){
        return SetVariableStructValue(state,datatype,bytes,pointerdepth);
    }
    else if(datatype->IsUnion()){
        return true;
    }
    else if(datatype->IsEnum()){
        return SetVariableEnumValue(state,datatype,bytes,pointerdepth);
    }
    else if(datatype->IsArray()){
        return SetVariableArrayValue(state,datatype,bytes,pointerdepth);
    }
    else{
        return SetVariableBaseValue(state,datatype,bytes,pointerdepth);
    }
}

bool CVariableTranslator::SetVariableBaseValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth){
    switch(datatype->DBaseEncoding){
        case DW_ATE::address:       {
                                        std::stringstream OutStream;
                                        OutStream<<std::hex;
                                        for(auto Iter = bytes.rbegin(); Iter != bytes.rend(); Iter++){
                                            OutStream<<std::setw(2)<<std::setfill('0')<<uint32_t(*Iter);
                                        }
                                        state->DValue.DDefault = OutStream.str();
                                        state->DValue.DDefaultConversion = DHexadecimalString;
                                        return true;
                                    }
        case DW_ATE::signed_:       {
                                        auto Value = GetSignedFromBytes(bytes,datatype->DBitSize,datatype->DBitOffset);
                                        state->DValue.DDefault = std::to_string(Value);
                                        state->DValue.DDefaultConversion = DDecimalString;

                                        std::stringstream HexStream;
                                        HexStream<<std::hex<<std::setw((datatype->DBitSize + 7)>>3)<<std::setfill('0')<<Value;
                                        state->DValue.DAlternatives.push_back(HexStream.str());
                                        state->DValue.DAlternativeConversions.push_back(DHexadecimalString);
                                        return true;
                                    }
        case DW_ATE::signed_char:   {
                                        auto Value = GetSignedFromBytes(bytes,datatype->DBitSize,datatype->DBitOffset);
                                        state->DValue.DDefault = ConvertCharToString(Value);
                                        state->DValue.DDefaultConversion = DCharacterString;

                                        state->DValue.DAlternatives.push_back(std::to_string(Value));
                                        state->DValue.DAlternativeConversions.push_back(DDecimalString);
                                        std::stringstream HexStream;
                                        HexStream<<std::hex<<std::setw((datatype->DBitSize + 7)>>3)<<std::setfill('0')<<Value;
                                        state->DValue.DAlternatives.push_back(HexStream.str());
                                        state->DValue.DAlternativeConversions.push_back(DHexadecimalString);
                                        return true;
                                    }
        case DW_ATE::unsigned_:     {
                                        auto Value = GetUnsignedFromBytes(bytes,datatype->DBitSize,datatype->DBitOffset);
                                        state->DValue.DDefault = std::to_string(Value);
                                        state->DValue.DDefaultConversion = DDecimalString;

                                        std::stringstream HexStream;
                                        HexStream<<std::hex<<std::setw((datatype->DBitSize + 7)>>3)<<std::setfill('0')<<Value;
                                        state->DValue.DAlternatives.push_back(HexStream.str());
                                        state->DValue.DAlternativeConversions.push_back(DHexadecimalString);
                                        return true;
                                    }
        case DW_ATE::unsigned_char: {
                                        auto Value = GetUnsignedFromBytes(bytes,datatype->DBitSize,datatype->DBitOffset);
                                        state->DValue.DDefault = ConvertCharToString(Value);
                                        state->DValue.DDefaultConversion = DCharacterString;

                                        state->DValue.DAlternatives.push_back(std::to_string(Value));
                                        state->DValue.DAlternativeConversions.push_back(DDecimalString);
                                        std::stringstream HexStream;
                                        HexStream<<std::hex<<std::setw((datatype->DBitSize + 7)>>3)<<std::setfill('0')<<Value;
                                        state->DValue.DAlternatives.push_back(HexStream.str());
                                        state->DValue.DAlternativeConversions.push_back(DHexadecimalString);
                                        return true;
                                    }
        case DW_ATE::float_:        {
                                        double DoubleValue;
                                        if(bytes.size() == sizeof(float)){
                                            uint32_t IntValue = 0;    
                                            for(auto Iter = bytes.rbegin(); Iter != bytes.rend(); Iter++){
                                                IntValue <<= 8;
                                                IntValue |= *Iter;
                                            }
                                            DoubleValue = *(float *)&IntValue;
                                        }
                                        else if(bytes.size() == sizeof(double)){
                                            uint64_t IntValue = 0;    
                                            for(auto Iter = bytes.rbegin(); Iter != bytes.rend(); Iter++){
                                                IntValue <<= 8;
                                                IntValue |= *Iter;
                                            }
                                            DoubleValue = *(double *)&IntValue;
                                        }
                                        else{
                                            return false;
                                        }
                                        state->DValue.DDefault = ConvertDoubleToString(DoubleValue);
                                        state->DValue.DDefaultConversion = bytes.size() == sizeof(float) ? DFloatString : DDoubleString;

                                        std::stringstream HexStream;
                                        HexStream<<std::hex;
                                        for(auto Iter = bytes.rbegin(); Iter != bytes.rend(); Iter++){
                                            HexStream<<std::setw(2)<<std::setfill('0')<<uint32_t(*Iter);
                                        }
                                        state->DValue.DAlternatives.push_back(HexStream.str());
                                        state->DValue.DAlternativeConversions.push_back(DHexadecimalString);
                                        return true;
                                    }
        default:                    break;
    }
    return false;
}

bool CVariableTranslator::SetVariableEnumValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth){
    switch(datatype->DBaseEncoding){
        case DW_ATE::signed_:       {
                                        auto Value = GetSignedFromBytes(bytes,datatype->DBitSize,datatype->DBitOffset);
                                        state->DValue.DDefault = DUnknownEnumString;
                                        state->DValue.DDefaultConversion = DEnumTypeString;
                                        for(auto Child : datatype->DChildren){
                                            if(Child->DConstant.GetINT64() == Value){
                                                state->DValue.DDefault = Child->DName;
                                                break;
                                            }
                                        }
                                        state->DValue.DAlternatives.push_back(std::to_string(Value));
                                        state->DValue.DAlternativeConversions.push_back(DDecimalString);
                                        std::stringstream HexStream;
                                        HexStream<<std::hex;
                                        for(auto Iter = bytes.rbegin(); Iter != bytes.rend(); Iter++){
                                            HexStream<<std::setw(2)<<std::setfill('0')<<uint32_t(*Iter);
                                        }
                                        state->DValue.DAlternatives.push_back(HexStream.str());
                                        state->DValue.DAlternativeConversions.push_back(DHexadecimalString);
                                        return true;
                                    }
        case DW_ATE::unsigned_:     {
                                        auto Value = GetUnsignedFromBytes(bytes,datatype->DBitSize,datatype->DBitOffset);
                                        state->DValue.DDefault = DUnknownEnumString;
                                        state->DValue.DDefaultConversion = DEnumTypeString;
                                        for(auto Child : datatype->DChildren){
                                            if(Child->DConstant.GetUINT64() == Value){
                                                state->DValue.DDefault = Child->DName;
                                                break;
                                            }
                                        }
                                        state->DValue.DAlternatives.push_back(std::to_string(Value));
                                        state->DValue.DAlternativeConversions.push_back(DDecimalString);
                                        std::stringstream HexStream;
                                        HexStream<<std::hex;
                                        for(auto Iter = bytes.rbegin(); Iter != bytes.rend(); Iter++){
                                            HexStream<<std::setw(2)<<std::setfill('0')<<uint32_t(*Iter);
                                        }
                                        state->DValue.DAlternatives.push_back(HexStream.str());
                                        state->DValue.DAlternativeConversions.push_back(DHexadecimalString);
                                        return true;
                                    }
        default:                    {
                                        state->DValue.DAlternatives.push_back(std::to_string(GetUnsignedFromBytes(bytes,datatype->DBitSize,datatype->DBitOffset)));
                                        state->DValue.DAlternativeConversions.push_back(DDecimalString);
                                        std::stringstream HexStream;
                                        HexStream<<std::hex;
                                        for(auto Iter = bytes.rbegin(); Iter != bytes.rend(); Iter++){
                                            HexStream<<std::setw(2)<<std::setfill('0')<<uint32_t(*Iter);
                                        }
                                        state->DValue.DAlternatives.push_back(HexStream.str());
                                        state->DValue.DAlternativeConversions.push_back(DHexadecimalString);
                                        return true;
                                    }
                                    return true;
    }
}

bool CVariableTranslator::SetVariableArrayValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth){
    auto ReferencedType = datatype->DReferencedType;
    auto Elements = datatype->DByteSize / ReferencedType->DByteSize;
    for(auto Index = 0; Index < Elements; Index++){
        auto NewVariableState = std::make_shared<CProgramState>();
        NewVariableState->DName = state->DName + std::string("[") + std::to_string(Index) + std::string("]");
        NewVariableState->DType = ReferencedType->DName;
        state->DChildren.push_back(NewVariableState);
        auto BeginData = bytes.begin() + Index * ReferencedType->DByteSize;
        std::vector< uint8_t > ElementBytes(BeginData, BeginData + ReferencedType->DByteSize);
        SetVariableValue(NewVariableState, ReferencedType, ElementBytes, pointerdepth);
    }
    return true;
}

bool CVariableTranslator::SetVariableStructValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth){
    size_t Offset = 0;
    std::vector< uint8_t > CurrentData;
    for(auto Child : datatype->DChildren){
        auto NewVariableState = std::make_shared<CProgramState>();
        NewVariableState->DName = Child->DName;
        NewVariableState->DType = Child->DReferencedType->DName;
        state->DChildren.push_back(NewVariableState);
        if(!Child->DBitOffset){
            CurrentData.assign(bytes.begin() + Offset, bytes.begin() + Offset + Child->DByteSize);
            Offset += Child->DByteSize;
        }
        if(Child->DBitSize != Child->DByteSize * 8){
            SetVariableBaseValue(NewVariableState, Child, CurrentData, pointerdepth);
        }
        else{
            SetVariableValue(NewVariableState, Child, CurrentData, pointerdepth);
        }
    }
    return true;
}

bool CVariableTranslator::SetVariablePointerValue(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SDataType > datatype, std::vector< uint8_t > &bytes, size_t pointerdepth){
    auto Address = GetUnsignedFromBytes(bytes,datatype->DBitSize,datatype->DBitOffset);
    auto ReferencedType = datatype->DReferencedType;
    std::stringstream HexStream;
    HexStream<<std::hex<<std::setw(8)<<std::setfill('0')<<Address;
    state->DValue.DDefault = HexStream.str();
    state->DValue.DDefaultConversion = DHexadecimalString;
    if((MaxPointerDepth() > pointerdepth)&&ReferencedType){
        CDwarfStructures::SObjectLocation VariableLocation;
        std::vector< uint8_t > DataAtLocation;
        VariableLocation.DAddress = Address;
        VariableLocation.DType = DW_OP::addr;

        if(GetVariableData(VariableLocation, ReferencedType->DByteSize, DataAtLocation)){
            auto NewVariableState = std::make_shared<CProgramState>();
            NewVariableState->DName = std::string("*") + state->DName;
            NewVariableState->DType = ReferencedType->DName;
            state->DChildren.push_back(NewVariableState);
            SetVariableValue(NewVariableState, ReferencedType, DataAtLocation, pointerdepth + 1);
        }
    }
    return true;
}

void CVariableTranslator::TranslateVariable(std::shared_ptr<CProgramState> state, std::shared_ptr< CDwarfStructures::SVariable > variable){
    auto NewVariableState = std::make_shared<CProgramState>();

    NewVariableState->DName = variable->DName;
    NewVariableState->DType = variable->DType->DName;
    state->DChildren.push_back(NewVariableState);
    
    std::vector< uint8_t > Data;
    if(variable->DType->DByteSize && GetVariableData(variable->DLocation, variable->DType->DByteSize, Data)){
        SetVariableValue(NewVariableState, variable->DType, Data);
    }
}

void CVariableTranslator::PrintState(std::shared_ptr<CProgramState> state, size_t depth){
    for(size_t Depth = 0; Depth < depth; Depth++){
        printf("  ");
    }
    printf("%s of type %s with value %s\n",state->DName.c_str(), state->DType.c_str(), state->DValue.Default().c_str());
    for(auto Child : state->DChildren){
        PrintState(Child,depth+1);
    }
    
}

void CVariableTranslator::TranslateVariables(std::vector< std::shared_ptr<CProgramState> > &state, const std::vector< std::shared_ptr< CDwarfStructures::SProgrammaticScope > > &scopes){
    size_t CurrentModuleIndex = scopes.size();
    state.clear();
    // Find the scope index goes from inner to outer followed by global of others
    for(size_t Index = 0; Index < scopes.size(); Index++){
        if(scopes[Index]->DParent.expired()){
            CurrentModuleIndex = Index;
            break;
        }
        if(auto Parent = scopes[Index]->DParent.lock()){
            if(Parent->DParent.expired()){
                CurrentModuleIndex = Index;
                break;
            }
        }
    }
    if(CurrentModuleIndex && (CurrentModuleIndex  < scopes.size())){
        std::shared_ptr<CProgramState> CurrentScopeState;
        for(size_t Index = 0; Index <= CurrentModuleIndex; Index++){
            auto Scope = scopes[CurrentModuleIndex-Index];
            auto NewScopeState = std::make_shared<CProgramState>();
            if(!Scope->DName.empty()){
                // Assume named function or module
                auto Parent = Scope->DParent.lock();
                NewScopeState->DName = Scope->DName;
                NewScopeState->DType = !Parent || Parent->DParent.expired() ? DCompilationUnitTypeString : DFunctionTypeString;
                if(CurrentScopeState){
                    CurrentScopeState->DChildren.push_back(NewScopeState);
                }
                else{
                    state.push_back(NewScopeState);
                }
            }
            else{
                NewScopeState->DName = CreateScopeName(Scope);
                NewScopeState->DType = DBlockTypeString;
                CurrentScopeState->DChildren.push_back(NewScopeState);
            }
            CurrentScopeState = NewScopeState;
            for(auto Variable : Scope->DVariables){
                // Add Variable
                TranslateVariable(CurrentScopeState,Variable);
            }
        }
    }
    // Do other module's global scope
    for(size_t Index = CurrentModuleIndex ? CurrentModuleIndex + 1 : 0; Index < scopes.size(); Index++){
        auto Scope = scopes[Index];
        auto NewScopeState = std::make_shared<CProgramState>();
        NewScopeState->DName = Scope->DName;
        NewScopeState->DType = DCompilationUnitTypeString;
        state.push_back(NewScopeState);
        for(auto Variable : Scope->DVariables){
            // Add Variable
            TranslateVariable(NewScopeState,Variable);
        }
    }
    /*
    printf("In %s @ %d\n",__FILE__,__LINE__);
    printf("----------- SCOPES BEGIN -----------\n");
    for(size_t Index = 0; Index < scopes.size(); Index++){
        auto Scope = scopes[Index];
        printf("%zu: %s\n",Index, Scope->DName.c_str());
    }
    printf("-----------  SCOPES END  -----------\n");
    printf("----------- STATE BEGIN ------------\n");
    for(auto State : state){
        PrintState(State, 0);
    }
    printf("-----------  STATE END  ------------\n");
    */
}
