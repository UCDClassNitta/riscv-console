#ifndef VIDEOCONTROLLERALLOCATOR_H
#define VIDEOCONTROLLERALLOCATOR_H

#include "VideoController.h"

class CVideoControllerAllocator{
    public:
        static uint32_t MinModel();
        static uint32_t MaxModel();
        static std::shared_ptr<CVideoController> Allocate(uint32_t model, std::shared_ptr<CGraphicFactory> graphicfactory);

};

#endif
