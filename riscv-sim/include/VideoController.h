#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include "RAMMemoryDevice.h"
#include "HardwareRegister.h"
#include "GraphicFactory.h"
#include "GraphicSurface.h"

class CVideoController{
    public:
        virtual ~CVideoController(){};

        virtual uint32_t ScreenWidth() const = 0;

        virtual uint32_t ScreenHeight() const = 0;

        virtual std::shared_ptr< CRAMMemoryDevice > VideoRAM() const = 0;

        virtual std::vector< uint32_t > SegmentBases() const = 0;

        virtual void Refresh(std::shared_ptr<CGraphicSurface> surface) = 0;

        virtual bool Tick() = 0;

        virtual void Reset() = 0;
};

#endif
