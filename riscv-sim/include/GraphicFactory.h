
#ifndef GRAPHICFACTORY_H
#define GRAPHICFACTORY_H
#include "GraphicSurface.h"
#include "DataSource.h"
#include "DataSink.h"

class CGraphicFactory{
    public:
        static std::shared_ptr<CGraphicSurface> CreateSurface(int width, int height, ESurfaceFormat format);
        static std::shared_ptr<CGraphicSurface> LoadSurface(std::shared_ptr< CDataSource > source);
        static bool StoreSurface(std::shared_ptr< CDataSink > sink, std::shared_ptr<CGraphicSurface> surface);
};

#endif

