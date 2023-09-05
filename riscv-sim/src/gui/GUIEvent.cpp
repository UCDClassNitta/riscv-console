#include "GUIEvent.h"
#include <cctype>
#include <unordered_map>

bool SGUIButtonEventType::IsButtonPress() const{
    return DType == ButtonPress;
}

bool SGUIButtonEventType::IsDoubleButtonPress() const{
    return DType == DoubleButtonPress;
}

bool SGUIButtonEventType::IsTripleButtonPress() const{
    return DType == TripleButtonPress;
}

bool SGUIButtonEventType::IsButtonRelease() const{
    return DType == ButtonRelease;
}

bool SGUIModifierType::ModifierIsSet(EType val) const{
    return DState & (1<<val);
}

void SGUIModifierType::SetModifier(EType val){
    DState |= (1<<val);
}

void SGUIModifierType::ClearModifier(EType val){
    DState &= ~(1<<val);
}

std::unordered_set<SGUIModifierType::EType> SGUIModifierType::AsSet() const{
    std::unordered_set<SGUIModifierType::EType> ReturnSet;
    auto TempState = DState;
    uint32_t Index = 0;
    while(TempState){
        if(TempState & 1){
            ReturnSet.insert(SGUIModifierType::EType(Index));
        }
        TempState >>= 1;
        Index++;
    }   
    return ReturnSet;
}

void SGUIKeyType::SetKey(EValue val){
    DValue = val;
}

bool SGUIKeyType::IsAlpha() const{
    return IsUpper() || IsLower();
}

bool SGUIKeyType::IsAlphaNumeric() const{
    return IsAlpha() || IsDigit();
}

bool SGUIKeyType::IsDigit() const{
    return ((Key0 <= DValue)&&(Key9 >= DValue));
}

bool SGUIKeyType::IsUpper() const{
    return ((KeyA <= DValue)&&(KeyZ >= DValue));
}

bool SGUIKeyType::IsLower() const{
    return ((Keya <= DValue)&&(Keyz >= DValue));
}

SGUIKeyType::EValue SGUIKeyType::ASCIIToValue(char ascii){
    std::unordered_map<char, EValue> NonAlphaNumMap = {{' ', Space},
                                                    {'.', Period},
                                                    {'\b', BackSpace},
                                                    {'-', Minus},
                                                    {'+', Plus},
                                                    {'=', Equal},
                                                    {',', Comma},
                                                    {':', Colon},
                                                    {';', SemiColon},
                                                    {'/', Slash},
                                                    {'?', QuestionMark},
                                                    {'!', Exclamation},
                                                    {'@', AtSymbol},
                                                    {'&', Ampersand},
                                                    {'#', Pound},
                                                    {'$', Dollar},
                                                    {'%', Percent},
                                                    {'^', Caret}};
    if(islower(ascii)){
        return EValue(Keya + (ascii - 'a'));
    }
    else if(isupper(ascii)){
        return EValue(KeyA + (ascii - 'A'));
    }
    else if(isdigit(ascii)){
        return EValue(Key0 + (ascii - '0'));
    }
    auto Search = NonAlphaNumMap.find(ascii);

    if(Search != NonAlphaNumMap.end()){
        return Search->second;
    }
    return Escape;
}