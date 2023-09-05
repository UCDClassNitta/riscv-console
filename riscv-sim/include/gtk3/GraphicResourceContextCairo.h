
#ifndef GRAPHICRESOURCECONTEXTCAIRO_H
#define GRAPHICRESOURCECONTEXTCAIRO_H
#include "GraphicResourceContext.h"
#include "GraphicSurfaceCairo.h"
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

#endif
