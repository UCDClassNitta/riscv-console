#include "VideoControllerAllocator.h"
#include "Model01VideoController.h"
#include "Model02VideoController.h"

uint32_t CVideoControllerAllocator::MinModel(){
    return 1;
}

uint32_t CVideoControllerAllocator::MaxModel(){
    return 2;
}

std::shared_ptr<CVideoController> CVideoControllerAllocator::Allocate(uint32_t model, std::shared_ptr<CGraphicFactory> graphicfactory){
    switch(model){
        case 1:     return std::make_shared<CModel01VideoController>(graphicfactory);
        default:    return std::make_shared<CModel02VideoController>(graphicfactory);
    }
    
}
