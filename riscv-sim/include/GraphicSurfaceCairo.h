
#ifndef GRAPHICSURFACECAIRO_H
#define GRAPHICSURFACECAIRO_H
#include "GraphicSurface.h"
#include "GraphicResourceContextCairo.h"
#include "GraphicFactoryCairo.h"
#include <gtk/gtk.h>

class CGraphicSurfaceCairo : public CGraphicSurface, public std::enable_shared_from_this<CGraphicSurface>{
    friend class CGraphicResourceContextCairo;
    protected:
        cairo_surface_t *DSurface;

    public:
        CGraphicSurfaceCairo(cairo_surface_t *surface, bool reference = false) : DSurface(surface){
            if(reference){
                cairo_surface_reference(DSurface);
            }
        };
        virtual ~CGraphicSurfaceCairo(){
            if(DSurface){
                cairo_surface_destroy(DSurface);
            }
        };

        cairo_surface_t *Surface() const{
            return DSurface;
        };

        int Width() const override{
            if(DSurface){
                return cairo_image_surface_get_width(DSurface);
            }
            return 0;
        };
        int Height() const override{
            if(DSurface){
                return cairo_image_surface_get_height(DSurface);
            }
            return 0;
        };

        ESurfaceFormat Format() const override{
            if(DSurface){
                switch(cairo_image_surface_get_format(DSurface)){
                    case CAIRO_FORMAT_ARGB32:   return ESurfaceFormat::ARGB32;
                    case CAIRO_FORMAT_RGB24:    return ESurfaceFormat::RGB24;
                    case CAIRO_FORMAT_A8:       return ESurfaceFormat::A8;
                    default:                    return ESurfaceFormat::A1;
                }
            }
            return ESurfaceFormat::A1;
        };

        uint32_t PixelAt(int xpos, int ypos) override;

        void Clear(int xpos = 0, int ypos = 0, int width = -1, int height = -1) override;
        std::shared_ptr<CGraphicSurface> Duplicate() override;

        std::shared_ptr<CGraphicResourceContext> CreateResourceContext() override;

        void Draw(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos) override;
        void Copy(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos) override;
        void CopyMaskSurface(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, std::shared_ptr<CGraphicSurface> masksurface, int sxpos, int sypos) override;
        void Transform(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos, void *calldata, TGraphicSurfaceTransformCallback callback) override;
        void AccessRawData(void *calldata, TGraphicSurfaceAccessRawDataCallback callback) override;
};

#endif

