#include "JSONElement.h"

CJSONElement::EType CJSONElement::Type() const{
    return DType;
}

void CJSONElement::Type(EType type){
    DMembers.clear();
    DType = type;
}

std::string CJSONElement::PrimitiveText() const{
    switch(DType){
        case EType::Number:
        case EType::Boolean:
        case EType::String: {
                                auto Search = DMembers.begin();
                                if(DMembers.end() != Search){
                                    return Search->first;
                                }
                            }
        default:            break;
    }
    return std::string();
}

void CJSONElement::PrimitiveText(const std::string &text){
    if((EType::Object == DType) || (EType::Array == DType)){
        return;
    }
    DMembers.clear();
    DMembers[text] = nullptr;
}

void CJSONElement::AssignMember(const std::string &key, std::shared_ptr< CJSONElement > element){
    if(EType::Object == DType){
        DMembers[key] = element;
    }
}

void CJSONElement::AssignMember(const std::string &key, const std::string &str){
    auto TempElement = std::make_shared< CJSONElement >();
    TempElement->Type(EType::String);
    TempElement->PrimitiveText(str);
    AssignMember(key,TempElement);
}

void CJSONElement::AssignMember(const std::string &key, double val){
    auto TempElement = std::make_shared< CJSONElement >();
    TempElement->Type(EType::Number);
    TempElement->PrimitiveText(std::to_string(val));
    AssignMember(key,TempElement);
}

void CJSONElement::AssignMember(const std::string &key, uint64_t val){
    auto TempElement = std::make_shared< CJSONElement >();
    TempElement->Type(EType::Number);
    TempElement->PrimitiveText(std::to_string(val));
    AssignMember(key,TempElement);
}

void CJSONElement::AssignMember(const std::string &key, int64_t val){
    auto TempElement = std::make_shared< CJSONElement >();
    TempElement->Type(EType::Number);
    TempElement->PrimitiveText(std::to_string(val));
    AssignMember(key,TempElement);
}

void CJSONElement::AssignMember(const std::string &key, uint32_t val){
    auto TempElement = std::make_shared< CJSONElement >();
    TempElement->Type(EType::Number);
    TempElement->PrimitiveText(std::to_string(val));
    AssignMember(key,TempElement);
}

void CJSONElement::AssignMember(const std::string &key, int32_t val){
    auto TempElement = std::make_shared< CJSONElement >();
    TempElement->Type(EType::Number);
    TempElement->PrimitiveText(std::to_string(val));
    AssignMember(key,TempElement);
}

void CJSONElement::AssignMember(const std::string &key, uint16_t val){
    auto TempElement = std::make_shared< CJSONElement >();
    TempElement->Type(EType::Number);
    TempElement->PrimitiveText(std::to_string(val));
    AssignMember(key,TempElement);
}

void CJSONElement::AssignMember(const std::string &key, int16_t val){
    auto TempElement = std::make_shared< CJSONElement >();
    TempElement->Type(EType::Number);
    TempElement->PrimitiveText(std::to_string(val));
    AssignMember(key,TempElement);
}

void CJSONElement::AssignMember(const std::string &key, bool tf){
    auto TempElement = std::make_shared< CJSONElement >();
    TempElement->Type(EType::Boolean);
    TempElement->PrimitiveText(tf ? std::string("true") : std::string("false"));
    AssignMember(key,TempElement);
}

void CJSONElement::PushBack(std::shared_ptr< CJSONElement > element){
    if(EType::Array == DType){
        DMembers[std::to_string(DMembers.size())] = element;
    }
}

size_t CJSONElement::ChildCount() const{
    if((EType::Object == DType)||(EType::Array == DType)){
        return DMembers.size();
    }
    return 0;
}

void CJSONElement::MemberKeys(std::vector< std::string > &keys) const{
    keys.clear();
    if(EType::Object == DType){
        for(auto &KeyValue : DMembers){
            keys.push_back(KeyValue.first);
        }
    }
}

std::shared_ptr< const CJSONElement >CJSONElement:: Member(const std::string &member) const{
    if(EType::Object == DType){
        auto Search = DMembers.find(member);
        if(DMembers.end() != Search){
            return Search->second;
        }
    }
    return nullptr;
}

std::shared_ptr< const CJSONElement > CJSONElement::Index(size_t index) const{
    if(EType::Array == DType){
        auto Search = DMembers.find(std::to_string(index));
        if(DMembers.end() != Search){
            return Search->second;
        }
    }
    return nullptr;
}
