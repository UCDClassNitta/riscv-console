#include "GraphicResourceContextCairo.h"

void CGraphicResourceContextCairo::SetSourceRGB(uint32_t rgb){
    SetSourceRGBA(0xFF000000 | rgb);
}

void CGraphicResourceContextCairo::SetSourceRGB(double r, double g, double b){
    cairo_set_source_rgb(DResourceContext, r, g, b);
}

void CGraphicResourceContextCairo::SetSourceRGBA(uint32_t rgba){
    cairo_set_source_rgba(DResourceContext, ((double)((rgba>>16)&0xFF))/255.0, ((double)((rgba>>8)&0xFF))/255.0, ((double)(rgba&0xFF))/255.0, ((double)((rgba>>24)&0xFF))/255.0);
}

void CGraphicResourceContextCairo::SetSourceRGBA(double r, double g, double b, double a){
    cairo_set_source_rgba(DResourceContext, r, g, b, a);
}

void CGraphicResourceContextCairo::SetLineWidth(double width){
    cairo_set_line_width(DResourceContext, width);
}

void CGraphicResourceContextCairo::SetSourceSurface(std::shared_ptr<CGraphicSurface> srcsurface, int xpos, int ypos){
    std::shared_ptr<CGraphicSurfaceCairo> CairoSourceSurface;
    
    if(!srcsurface){
        return;
    }
    CairoSourceSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(srcsurface);
    cairo_set_source_surface(DResourceContext, CairoSourceSurface->DSurface, xpos, ypos);
}

void CGraphicResourceContextCairo::SetLineCap(ELineCap cap){
    cairo_line_cap_t CapType;
    switch(cap){
        case ELineCap::Round:   CapType = CAIRO_LINE_CAP_ROUND;
                                break;
        case ELineCap::Square:  CapType = CAIRO_LINE_CAP_SQUARE;
                                break;
        case ELineCap::Butt:    
        default:                CapType = CAIRO_LINE_CAP_BUTT;
                                break;
    }
    cairo_set_line_cap(DResourceContext, CapType);
}

void CGraphicResourceContextCairo::SetLineJoin(ELineJoin join){
    cairo_line_join_t JoinType;
    switch(join){
        case ELineJoin::Round:  JoinType = CAIRO_LINE_JOIN_ROUND;
                                break;
        case ELineJoin::Bevel:  JoinType = CAIRO_LINE_JOIN_BEVEL;
                                break;
        case ELineJoin::Miter:    
        default:                JoinType = CAIRO_LINE_JOIN_MITER;
                                break;
    }
    cairo_set_line_join(DResourceContext, JoinType);
}

void CGraphicResourceContextCairo::Scale(double sx, double sy){
    cairo_scale(DResourceContext, sx, sy);
}

void CGraphicResourceContextCairo::Paint(){
    cairo_paint(DResourceContext);
}

void CGraphicResourceContextCairo::PaintWithAlpha(double alpha){
    cairo_paint_with_alpha(DResourceContext, alpha);
}

void CGraphicResourceContextCairo::Fill(){
    cairo_fill(DResourceContext);
}

void CGraphicResourceContextCairo::Stroke(){
    cairo_stroke(DResourceContext);
}

void CGraphicResourceContextCairo::StrokePreserve(){
    cairo_stroke_preserve(DResourceContext);
}

void CGraphicResourceContextCairo::Rectangle(int xpos, int ypos, int width, int height){
    cairo_rectangle(DResourceContext, xpos+ 0.5, ypos + 0.5, width, height);    
}

void CGraphicResourceContextCairo::MoveTo(int xpos, int ypos){
    cairo_move_to(DResourceContext, xpos + 0.5, ypos + 0.5);
}

void CGraphicResourceContextCairo::LineTo(int xpos, int ypos){
    cairo_line_to(DResourceContext, xpos + 0.5, ypos + 0.5);
}

void CGraphicResourceContextCairo::Clip(){
    cairo_clip(DResourceContext);
}

void CGraphicResourceContextCairo::MaskSurface(std::shared_ptr<CGraphicSurface> srcsurface, int xpos, int ypos){
    std::shared_ptr<CGraphicSurfaceCairo> CairoSourceSurface; 
    
    if(!srcsurface){
        return;
    }
    CairoSourceSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(srcsurface);
    cairo_mask_surface(DResourceContext, CairoSourceSurface->DSurface, xpos, ypos);
}


std::shared_ptr<CGraphicSurface> CGraphicResourceContextCairo::GetTarget(){
    return std::make_shared<CGraphicSurfaceCairo>(cairo_get_target(DResourceContext), true);   
}

void CGraphicResourceContextCairo::Save(){
    cairo_save(DResourceContext);
}

void CGraphicResourceContextCairo::Restore(){
    cairo_restore(DResourceContext);
}

void CGraphicResourceContextCairo::DrawSurface(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos){
    std::shared_ptr<CGraphicSurfaceCairo> CairoSourceSurface; 
    
    if(!srcsurface){
        return;
    }
    CairoSourceSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(srcsurface);   
    if(srcsurface->Width() < sxpos + width){
        width = srcsurface->Width() - sxpos;   
    }
    if(0 > width){
        width = srcsurface->Width() - sxpos;
        width = 0 > width ? 0 : width;
    }
    if(srcsurface->Height() < sypos + height){
        height = srcsurface->Height() - sypos;   
    }
    if(0 > height){
        height = srcsurface->Height() - sypos;
        height = 0 > height ? 0 : height;
    }
    cairo_set_source_surface(DResourceContext, CairoSourceSurface->DSurface, dxpos - sxpos, dypos - sypos);
    cairo_rectangle(DResourceContext, dxpos, dypos, width, height);
    cairo_fill(DResourceContext);    
}

void CGraphicResourceContextCairo::CopySurface(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos){
    std::shared_ptr<CGraphicSurfaceCairo> CairoSourceSurface; 
    
    if(!srcsurface){
        return;
    }
    CairoSourceSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(srcsurface);
    if(srcsurface->Width() < sxpos + width){
        width = srcsurface->Width() - sxpos;   
    }
    if(0 > width){
        width = srcsurface->Width() - sxpos;
        width = 0 > width ? 0 : width;
    }
    if(srcsurface->Height() < sypos + height){
        height = srcsurface->Height() - sypos;   
    }
    if(0 > height){
        height = srcsurface->Height() - sypos;
        height = 0 > height ? 0 : height;
    }
    Save();
    cairo_set_source_surface(DResourceContext, CairoSourceSurface->DSurface, dxpos - sxpos, dypos - sypos);
    cairo_rectangle(DResourceContext, dxpos, dypos, width, height);
    cairo_set_operator(DResourceContext, CAIRO_OPERATOR_SOURCE);
    cairo_fill(DResourceContext);    
    Restore();
}
