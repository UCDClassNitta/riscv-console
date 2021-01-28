#ifndef GUIJUSTIFICATION_H
#define GUIJUSTIFICATION_H
#include <cstdint>


struct SGUIJustificationType{
    uint32_t DType;

    SGUIJustificationType(uint32_t just){
        DType = just;
    };
    SGUIJustificationType() = default;
    SGUIJustificationType(const SGUIJustificationType &just) = default;
    SGUIJustificationType &operator=(const SGUIJustificationType &just) = default;
    
    static const uint32_t Left;
    static const uint32_t Right;
    static const uint32_t Center;
    static const uint32_t Fill;
};

#endif
