
#include "GraphicFactoryCairo.h"
#include <vector>

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

std::shared_ptr<CGraphicResourceContext> CGraphicSurfaceCairo::CreateResourceContext(){
    if(DSurface){
        return std::shared_ptr<CGraphicResourceContext>(new CGraphicResourceContextCairo(cairo_create(DSurface)));
    }
    return nullptr;
}

uint32_t CGraphicSurfaceCairo::PixelAt(int xpos, int ypos){
    int Stride;
    unsigned char *SrcPixels;
    uint32_t *Pixel;
    
    if((0 > xpos)||(0 > ypos)||(xpos >= Width())||(ypos >= Height())){
        return 0;   
    }
    cairo_surface_flush(DSurface);
    SrcPixels = cairo_image_surface_get_data(DSurface);
    Stride = cairo_image_surface_get_stride(DSurface);
    switch(cairo_image_surface_get_format(DSurface)){
        case CAIRO_FORMAT_ARGB32:   Pixel = (uint32_t *)(SrcPixels + (Stride * ypos));
                                    Pixel += xpos;
                                    return *Pixel;
        case CAIRO_FORMAT_RGB24:    Pixel = (uint32_t *)(SrcPixels + (Stride * ypos));
                                    Pixel += xpos;
                                    return (*Pixel) | 0xFF000000;
        default:                    return 0;
    }
}

void CGraphicSurfaceCairo::Clear(int xpos, int ypos, int width, int height){
    if(Width() < xpos + width){
        width = Width() - xpos;   
    }
    if(0 > width){
        width = Width() - xpos;
        width = 0 > width ? 0 : width;
    }
    if(Height() < ypos + height){
        height = Height() - ypos;   
    }
    if(0 > height){
        height = Height() - ypos;
        height = 0 > height ? 0 : height;
    }
    std::shared_ptr<CGraphicResourceContextCairo> CairoResourceContext = std::dynamic_pointer_cast<CGraphicResourceContextCairo>(CreateResourceContext());
    
    cairo_save(CairoResourceContext->DResourceContext);
    cairo_rectangle(CairoResourceContext->DResourceContext, xpos, ypos, width, height);
    cairo_set_operator(CairoResourceContext->DResourceContext, CAIRO_OPERATOR_CLEAR);
    cairo_fill(CairoResourceContext->DResourceContext);
    cairo_restore(CairoResourceContext->DResourceContext);
}

std::shared_ptr<CGraphicSurface> CGraphicSurfaceCairo::Duplicate(){
    if(DSurface){
        cairo_format_t Format = cairo_image_surface_get_format(DSurface);
        std::shared_ptr<CGraphicSurface> NewSurface(new CGraphicSurfaceCairo(cairo_image_surface_create(Format, Width(), Height())));

        NewSurface->Copy(shared_from_this(), 0, 0, Width(), Height(), 0, 0);
        
        return NewSurface;
    }
    return nullptr;
}

void CGraphicSurfaceCairo::Draw(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos){
    std::shared_ptr<CGraphicResourceContext> ResourceContext;
    std::shared_ptr<CGraphicResourceContextCairo> CairoResourceContext; 
    std::shared_ptr<CGraphicSurfaceCairo> CairoSourceSurface; 
    
    if(!srcsurface){
        return;
    }
    CairoSourceSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(srcsurface);
    if(!CairoSourceSurface){
        return;
    }
    ResourceContext = CreateResourceContext();
    if(!ResourceContext){
        return;    
    }
    CairoResourceContext = std::dynamic_pointer_cast<CGraphicResourceContextCairo>(ResourceContext);
    if(!CairoResourceContext){
        return;    
    }
    
    if(CairoSourceSurface->Width() < sxpos + width){
        width = CairoSourceSurface->Width() - sxpos;   
    }
    if(0 > width){
        width = CairoSourceSurface->Width() - sxpos;
        width = 0 > width ? 0 : width;
    }
    if(CairoSourceSurface->Height() < sypos + height){
        height = srcsurface->Height() - sypos;   
    }
    if(0 > height){
        height = CairoSourceSurface->Height() - sypos;
        height = 0 > height ? 0 : height;
    }
    
    cairo_set_source_surface(CairoResourceContext->DResourceContext, CairoSourceSurface->DSurface, dxpos - sxpos, dypos - sypos);
    cairo_rectangle(CairoResourceContext->DResourceContext, dxpos, dypos, width, height);
    cairo_fill(CairoResourceContext->DResourceContext);    
}

void CGraphicSurfaceCairo::Copy(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos){
    std::shared_ptr<CGraphicResourceContext> ResourceContext;
    std::shared_ptr<CGraphicResourceContextCairo> CairoResourceContext; 
    std::shared_ptr<CGraphicSurfaceCairo> CairoSourceSurface; 
    
    if(!srcsurface){
        return;
    }
    CairoSourceSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(srcsurface);
    if(!CairoSourceSurface){
        return;
    }
    ResourceContext = CreateResourceContext();
    if(!ResourceContext){
        return;    
    }
    CairoResourceContext = std::dynamic_pointer_cast<CGraphicResourceContextCairo>(ResourceContext);
    if(!CairoResourceContext){
        return;    
    }
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
    
    cairo_save(CairoResourceContext->DResourceContext);
    cairo_set_source_surface(CairoResourceContext->DResourceContext, CairoSourceSurface->DSurface, dxpos - sxpos, dypos - sypos);
    cairo_rectangle(CairoResourceContext->DResourceContext, dxpos, dypos, width, height);
    cairo_set_operator(CairoResourceContext->DResourceContext, CAIRO_OPERATOR_SOURCE);
    cairo_fill(CairoResourceContext->DResourceContext);    
    cairo_restore(CairoResourceContext->DResourceContext);
}

void CGraphicSurfaceCairo::CopyMaskSurface(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, std::shared_ptr<CGraphicSurface> masksurface, int sxpos, int sypos){
    std::shared_ptr<CGraphicResourceContext> ResourceContext;
    std::shared_ptr<CGraphicResourceContextCairo> CairoResourceContext; 
    std::shared_ptr<CGraphicSurfaceCairo> CairoSourceSurface; 
    std::shared_ptr<CGraphicSurfaceCairo> CairoMaskSurface; 
    
    if(!srcsurface){
        return;
    }
    CairoSourceSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(srcsurface);
    if(!CairoSourceSurface){
        return;
    }
    if(!masksurface){
        return;
    }
    CairoMaskSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(masksurface);
    if(!CairoMaskSurface){
        return;
    }
    ResourceContext = CreateResourceContext();
    if(!ResourceContext){
        return;    
    }
    CairoResourceContext = std::dynamic_pointer_cast<CGraphicResourceContextCairo>(ResourceContext);
    if(!CairoResourceContext){
        return;    
    }
    
    cairo_save(CairoResourceContext->DResourceContext);
    cairo_set_source_surface(CairoResourceContext->DResourceContext, CairoSourceSurface->DSurface, dxpos - sxpos, dypos - sypos);
    cairo_mask_surface(CairoResourceContext->DResourceContext, CairoMaskSurface->DSurface, dxpos, dypos);
    cairo_set_operator(CairoResourceContext->DResourceContext, CAIRO_OPERATOR_SOURCE);
    cairo_fill(CairoResourceContext->DResourceContext);    
    cairo_restore(CairoResourceContext->DResourceContext);
}

void CGraphicSurfaceCairo::Transform(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos, void *calldata, TGraphicSurfaceTransformCallback callback){
    std::shared_ptr<CGraphicSurfaceCairo> CairoSourceSurface; 
    std::vector< std::vector < uint32_t > > BufferedPixels;
    int Stride;
    unsigned char *SrcPixels;
    unsigned char *DestPixels;
    
    
    if(!srcsurface){
        return;
    }
    CairoSourceSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(srcsurface);
    if(!CairoSourceSurface){
        return;
    }
    if(!DSurface){
        return;   
    }
    if(!CairoSourceSurface->DSurface){
        return;   
    }
    if(CairoSourceSurface->Width() < sxpos + width){
        width = CairoSourceSurface->Width() - sxpos;   
    }
    if(0 > width){
        width = CairoSourceSurface->Width() - sxpos;
        width = 0 > width ? 0 : width;
    }
    if(Width() < dxpos + width){
        width = Width() - dxpos;   
    }
    if(CairoSourceSurface->Height() < sypos + height){
        height = CairoSourceSurface->Height() - sypos;   
    }
    if(0 > height){
        height = CairoSourceSurface->Height() - sypos;
        height = 0 > height ? 0 : height;
    }
    if(Height() < dypos + height){
        height = Height() - dypos;   
    }
    cairo_surface_flush(CairoSourceSurface->DSurface);
    SrcPixels = cairo_image_surface_get_data(CairoSourceSurface->DSurface);
    Stride = cairo_image_surface_get_stride(CairoSourceSurface->DSurface);
    BufferedPixels.resize(height);
    switch(cairo_image_surface_get_format(CairoSourceSurface->DSurface)){
        case CAIRO_FORMAT_ARGB32:   for(int Row = 0; Row < height; Row++){
                                        uint32_t *Pixel = (uint32_t *)(SrcPixels + (Stride * (sypos + Row)));
                                        Pixel += sxpos;
                                        for(int Col = 0; Col < width; Col++){
                                            BufferedPixels[Row].push_back(callback(calldata, *Pixel));
                                            Pixel++;
                                        }
                                    }
                                    break;
        case CAIRO_FORMAT_RGB24:    for(int Row = 0; Row < height; Row++){
                                        uint32_t *Pixel = (uint32_t *)(SrcPixels + (Stride * (sypos + Row)));
                                        Pixel += sxpos;
                                        for(int Col = 0; Col < width; Col++){
                                            BufferedPixels[Row].push_back(callback(calldata, (*Pixel) | 0xFF000000));
                                            Pixel++;
                                        }
                                    }
                                    break;
        default:                    return;
    }

    
    cairo_surface_flush(DSurface);
    DestPixels = cairo_image_surface_get_data(DSurface);
    Stride = cairo_image_surface_get_stride(DSurface);
    switch(cairo_image_surface_get_format(CairoSourceSurface->DSurface)){
        case CAIRO_FORMAT_ARGB32:
        case CAIRO_FORMAT_RGB24:    for(int Row = 0; Row < height; Row++){
                                        uint32_t *Pixel = (uint32_t *)(DestPixels + (Stride * (dypos + Row)));
                                        Pixel += dxpos;
                                        for(uint32_t Value : BufferedPixels[Row]){
                                            *Pixel = Value;
                                            Pixel++;
                                        }
                                    }
                                    break;
        default:                    return;
    }
    
    cairo_surface_mark_dirty(DSurface);
    
}

void CGraphicSurfaceCairo::AccessRawData(void *calldata, TGraphicSurfaceAccessRawDataCallback callback){
    if(!DSurface){
        return;
    }
    cairo_surface_flush(DSurface);
    uint8_t *PixelData = (uint8_t *)cairo_image_surface_get_data(DSurface);
    int Stride = cairo_image_surface_get_stride(DSurface);
    ESurfaceFormat Format; 
    switch(cairo_image_surface_get_format(DSurface)){
        case CAIRO_FORMAT_ARGB32:   Format = ESurfaceFormat::ARGB32;
                                    break;
        case CAIRO_FORMAT_RGB24:    Format = ESurfaceFormat::RGB24;
                                    break;
        case CAIRO_FORMAT_A8:       Format = ESurfaceFormat::A8;
                                    break;
        case CAIRO_FORMAT_A1:       Format = ESurfaceFormat::A1;
                                    break;
        default:                    return;
    }
    int Width = cairo_image_surface_get_width(DSurface);
    int Height = cairo_image_surface_get_height(DSurface);
    if(callback(calldata,PixelData,Format,Stride,Width,Height)){
        cairo_surface_mark_dirty(DSurface);
        cairo_surface_flush(DSurface);
        //printf("CAIRO_CONTENT_COLOR_ALPHA = %d, This = %d\n",CAIRO_CONTENT_COLOR_ALPHA,cairo_surface_get_content(DSurface));
    }
}

std::shared_ptr<CGraphicSurface> CGraphicFactory::CreateSurface(int width, int height, ESurfaceFormat format){
    cairo_format_t Format;
    switch(format){
        case ESurfaceFormat::ARGB32:    Format = CAIRO_FORMAT_ARGB32;
                                        break;
        case ESurfaceFormat::RGB24:     Format = CAIRO_FORMAT_RGB24;
                                        break;
        case ESurfaceFormat::A8:        Format = CAIRO_FORMAT_A8;
                                        break;
        case ESurfaceFormat::A1:        Format = CAIRO_FORMAT_A1;
                                        break;
        default:                        Format = CAIRO_FORMAT_ARGB32;
                                        break;
    }
    return std::shared_ptr<CGraphicSurface>(new CGraphicSurfaceCairo(cairo_image_surface_create(Format, width, height)));   
}

cairo_status_t GraphicFactoryCairoDataRead(void *closure, unsigned char *data, unsigned int length){
    CDataSource *DataSource = static_cast< CDataSource * >(closure);
    
    return DataSource->Read(data, length) ? CAIRO_STATUS_SUCCESS : CAIRO_STATUS_READ_ERROR;
}

std::shared_ptr<CGraphicSurface> CGraphicFactory::LoadSurface(std::shared_ptr< CDataSource > source){
    return std::shared_ptr<CGraphicSurface>(new CGraphicSurfaceCairo(cairo_image_surface_create_from_png_stream(GraphicFactoryCairoDataRead, source.get())));
}

cairo_status_t GraphicFactoryCairoDataWrite(void *closure, const unsigned char *data, unsigned int length){
    CDataSink *DataSink = static_cast< CDataSink * >(closure);
    
    return length == (unsigned int)DataSink->Write(data, length) ? CAIRO_STATUS_SUCCESS : CAIRO_STATUS_READ_ERROR;
}

bool CGraphicFactory::StoreSurface(std::shared_ptr< CDataSink > sink, std::shared_ptr<CGraphicSurface> surface){
    std::shared_ptr<CGraphicSurfaceCairo> CairoSourceSurface;
    
    if(!surface){
        return false;
    }
    CairoSourceSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(surface);
    return cairo_surface_write_to_png_stream(CairoSourceSurface->DSurface, GraphicFactoryCairoDataWrite, sink.get()) == CAIRO_STATUS_SUCCESS;
}

