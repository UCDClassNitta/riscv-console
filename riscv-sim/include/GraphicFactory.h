
#ifndef GRAPHICFACTORY_H
#define GRAPHICFACTORY_H
#include "GraphicSurface.h"
#include "DataSource.h"
#include "DataSink.h"

class CGraphicFactory{
    public:
        virtual ~CGraphicFactory() = default;
        virtual std::shared_ptr<CGraphicSurface> CreateSurface(int width, int height, ESurfaceFormat format) = 0;
        virtual std::shared_ptr<CGraphicSurface> LoadSurface(std::shared_ptr< CDataSource > source) = 0;
        virtual bool StoreSurface(std::shared_ptr< CDataSink > sink, std::shared_ptr<CGraphicSurface> surface) = 0;
};

#endif

