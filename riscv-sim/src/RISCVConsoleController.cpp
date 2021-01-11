#include "RISCVConsoleController.h"

CRISCVConsoleController::CRISCVConsoleController(std::vector< std::shared_ptr< CGUIToggleButton > > &buttons){
    DControllerButtons = buttons;
}

uint32_t CRISCVConsoleController::load() noexcept{
    uint32_t Result = 0;
    uint32_t Mask = 1;
    for(auto &Button : DControllerButtons){
        if(Button->GetActive()){
            Result |= Mask;
        }
        Mask <<= 1;
    }
    return Result;
}
