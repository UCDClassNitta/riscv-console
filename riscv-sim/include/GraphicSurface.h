
#ifndef GRAPHICSURFACE_H
#define GRAPHICSURFACE_H
#include <cstdint>
#include "GraphicResourceContext.h"

enum class ESurfaceFormat{ARGB32, RGB24, A8, A1};

using TGraphicSurfaceTransformCallback = uint32_t (*)(void *calldata, uint32_t src);
using TGraphicSurfaceAccessRawDataCallback = bool (*)(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height);

class CGraphicSurface{
    public:
        CGraphicSurface(){};
        virtual ~CGraphicSurface(){};
        
        virtual int Width() const = 0;
        virtual int Height() const = 0;
        
        virtual ESurfaceFormat Format() const = 0;
        
        virtual uint32_t PixelAt(int xpos, int ypos) = 0;
        
        virtual void Clear(int xpos = 0, int ypos = 0, int width = -1, int height = -1) = 0;
        virtual std::shared_ptr<CGraphicSurface> Duplicate() = 0;
        
        virtual std::shared_ptr<CGraphicResourceContext> CreateResourceContext() = 0;
        
        virtual void Draw(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos) = 0;
        virtual void Copy(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos) = 0;
        virtual void CopyMaskSurface(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, std::shared_ptr<CGraphicSurface> masksurface, int sxpos, int sypos) = 0;
        virtual void Transform(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos, void *calldata, TGraphicSurfaceTransformCallback callback) = 0;
        virtual void AccessRawData(void *calldata, TGraphicSurfaceAccessRawDataCallback callback) = 0;
};

#endif

