#include "GraphicFactoryCairo.h"
#include "GraphicSurfaceCairo.h"
#include <vector>

std::shared_ptr<CGraphicFactory> CGraphicFactoryCairo::DInstance;

CGraphicFactoryCairo::CGraphicFactoryCairo(const SPrivateConstructorKey &key){

}

CGraphicFactoryCairo::~CGraphicFactoryCairo(){

}

std::shared_ptr<CGraphicFactory> CGraphicFactoryCairo::Instance(){
    if(!DInstance){
        DInstance = std::make_shared<CGraphicFactoryCairo>(SPrivateConstructorKey{});
    }
    return DInstance;
}

std::shared_ptr<CGraphicSurface> CGraphicFactoryCairo::CreateSurface(int width, int height, ESurfaceFormat format){
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

cairo_status_t CGraphicFactoryCairo::DataRead(void *closure, unsigned char *data, unsigned int length){
    CDataSource *DataSource = static_cast< CDataSource * >(closure);
    
    return DataSource->Read(data, length) ? CAIRO_STATUS_SUCCESS : CAIRO_STATUS_READ_ERROR;
}

cairo_status_t CGraphicFactoryCairo::DataWrite(void *closure, const unsigned char *data, unsigned int length){
    CDataSink *DataSink = static_cast< CDataSink * >(closure);
    
    return length == (unsigned int)DataSink->Write(data, length) ? CAIRO_STATUS_SUCCESS : CAIRO_STATUS_READ_ERROR;
}

std::shared_ptr<CGraphicSurface> CGraphicFactoryCairo::LoadSurface(std::shared_ptr< CDataSource > source){
    return std::make_shared<CGraphicSurfaceCairo>(cairo_image_surface_create_from_png_stream(DataRead, source.get()));
}

bool CGraphicFactoryCairo::StoreSurface(std::shared_ptr< CDataSink > sink, std::shared_ptr<CGraphicSurface> surface){
    std::shared_ptr<CGraphicSurfaceCairo> CairoSourceSurface;
    
    if(!surface){
        return false;
    }
    CairoSourceSurface = std::dynamic_pointer_cast<CGraphicSurfaceCairo>(surface);
    return cairo_surface_write_to_png_stream(CairoSourceSurface->Surface(), DataWrite, sink.get()) == CAIRO_STATUS_SUCCESS;
}

