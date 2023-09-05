
#ifndef GRAPHICFACTORYCAIRO_H
#define GRAPHICFACTORYCAIRO_H
#include "GraphicFactory.h"
#include <gtk/gtk.h>

class CGraphicFactoryCairo : public virtual CGraphicFactory, public std::enable_shared_from_this<CGraphicFactory>{
    private:
        struct SPrivateConstructorKey{};
        static std::shared_ptr<CGraphicFactory> DInstance;

        static cairo_status_t DataRead(void *closure, unsigned char *data, unsigned int length);
        static cairo_status_t DataWrite(void *closure, const unsigned char *data, unsigned int length);

    public:
        explicit CGraphicFactoryCairo(const SPrivateConstructorKey &key);
        virtual ~CGraphicFactoryCairo();

        static std::shared_ptr<CGraphicFactory> Instance();

        virtual std::shared_ptr<CGraphicSurface> CreateSurface(int width, int height, ESurfaceFormat format) override;
        virtual std::shared_ptr<CGraphicSurface> LoadSurface(std::shared_ptr< CDataSource > source) override;
        virtual bool StoreSurface(std::shared_ptr< CDataSink > sink, std::shared_ptr<CGraphicSurface> surface) override;
};

#endif

