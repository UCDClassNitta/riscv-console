
#ifndef GUIEVENTGTK3_H
#define GUIEVENTGTK3_H
#include "GUIEvent.h"
#include <unordered_map>

struct SGUIButtonEventTypeGTK3{
    static SGUIButtonEventType TranslateFromGTK3Type(uint32_t gtk3type);
    static uint32_t TranslateToGTK3Type(SGUIButtonEventType guitype);
    private:
        static const std::unordered_map<uint32_t, SGUIButtonEventType::EType> DFromGTK3Mapping;
        static const std::unordered_map<SGUIButtonEventType::EType, uint32_t> DToGTK3Mapping;
};

struct SGUIModifierTypeGTK3{
    static SGUIModifierType TranslateFromGTK3Type(uint32_t gtk3type);
    static uint32_t TranslateToGTK3Type(SGUIModifierType guitype);
    private:
        static const std::unordered_map<uint32_t, SGUIModifierType::EType> DFromGTK3Mapping;
        static const std::unordered_map<SGUIModifierType::EType, uint32_t> DToGTK3Mapping;
};
  
struct SGUIKeyTypeGTK3{
    static SGUIKeyType TranslateFromGTK3Type(uint32_t gtk3type);
    static uint32_t TranslateToGTK3Type(SGUIKeyType guitype);
    private:
        static const std::unordered_map<uint32_t, SGUIKeyType::EValue> DFromGTK3Mapping;
        static const std::unordered_map<SGUIKeyType::EValue, uint32_t> DToGTK3Mapping;
};

#endif
