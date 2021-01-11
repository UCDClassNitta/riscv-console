
#ifndef GRAPHICFACTORYCAIRO_H
#define GRAPHICFACTORYCAIRO_H
#include "GraphicFactory.h"
#include <gtk/gtk.h>

class CGraphicResourceContextCairo : public CGraphicResourceContext{
    friend class CGraphicSurfaceCairo;
    protected:
        cairo_t *DResourceContext;
        GdkWindow *DWindow;
#if GTK_CHECK_VERSION(3, 22, 0) //(GDK_VERSION_MAX_ALLOWED >= GDK_VERSION_3_22)
        GdkDrawingContext *DDrawingContext;
#endif
    public:
        CGraphicResourceContextCairo(cairo_t *rc, bool reference = false) : DResourceContext(rc), DWindow(nullptr){
            if(reference){
                cairo_reference(DResourceContext);  
            }
        };
#if GTK_CHECK_VERSION(3, 22, 0) //(GDK_VERSION_MAX_ALLOWED >= GDK_VERSION_3_22)
        CGraphicResourceContextCairo(GdkWindow *window, GdkDrawingContext *drawingcontext) : DWindow(window), DDrawingContext(drawingcontext){
            DResourceContext = gdk_drawing_context_get_cairo_context(DDrawingContext);
        };
#endif
        ~CGraphicResourceContextCairo(){
            if(DResourceContext){
                if(DWindow != nullptr){
#if GTK_CHECK_VERSION(3, 22, 0) //(GDK_VERSION_MAX_ALLOWED >= GDK_VERSION_3_22)
                    gdk_window_end_draw_frame(DWindow, DDrawingContext);
#endif
                }
                else{
                    cairo_destroy(DResourceContext);   
                }
            }
        };  
        
        void SetSourceRGB(uint32_t rgb) override;
        void SetSourceRGB(double r, double g, double b) override;
        void SetSourceRGBA(uint32_t rgba) override;
        void SetSourceRGBA(double r, double g, double b, double a) override;
        void SetSourceSurface(std::shared_ptr<CGraphicSurface> srcsurface, int xpos, int ypos) override;
        void SetLineWidth(double width) override;
        void SetLineCap(ELineCap cap) override;
        void SetLineJoin(ELineJoin join) override;
        void Scale(double sx, double sy) override;
        void Paint() override;
        void PaintWithAlpha(double alpha) override;
        void Fill() override;
        void Stroke() override;
        void StrokePreserve() override;
        void Rectangle(int xpos, int ypos, int width, int height) override;
        void MoveTo(int xpos, int ypos) override;
        void LineTo(int xpos, int ypos) override;
        void Clip() override;
        void MaskSurface(std::shared_ptr<CGraphicSurface> srcsurface, int xpos, int ypos) override;
        
        std::shared_ptr<CGraphicSurface> GetTarget() override;
        void Save() override;
        void Restore() override;
        void DrawSurface(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos) override;
        void CopySurface(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos) override;
};

class CGraphicSurfaceCairo : public CGraphicSurface, public std::enable_shared_from_this<CGraphicSurface>{
    friend class CGraphicResourceContextCairo;
    friend class CGraphicFactory;
    protected:
        cairo_surface_t *DSurface;
        
    public:
        CGraphicSurfaceCairo(cairo_surface_t *surface, bool reference = false) : DSurface(surface){
            if(reference){
                cairo_surface_reference(DSurface);  
            }
        };
        ~CGraphicSurfaceCairo(){
            if(DSurface){
                cairo_surface_destroy(DSurface);   
            }
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

