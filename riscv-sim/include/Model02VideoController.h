#ifndef MODEL02VIDEOCONTROLLER_H
#define MODEL02VIDEOCONTROLLER_H

#include "VideoController.h"

class CModel02VideoController : public CVideoController{
    public:
        struct SLargeSpriteControl{
            uint32_t DPalette : 2;
            uint32_t DXOffset : 10;
            uint32_t DYOffset : 9;
            uint32_t DZ : 3;
            uint32_t DIndex : 6;
            uint32_t DReserved : 2;
        };
        struct SMediumSpriteControl{
            uint32_t DPalette : 2;
            uint32_t DXOffset : 10;
            uint32_t DYOffset : 9;
            uint32_t DZ : 3;
            uint32_t DIndex : 6;
            uint32_t DReserved : 2;
        };
        struct SSmallSpriteControl{
            uint32_t DPalette : 2;
            uint32_t DXOffset : 10;
            uint32_t DYOffset : 9;
            uint32_t DZ : 3;
            uint32_t DIndex : 8;
        };
        struct SBackgroundControl{
            uint32_t DPalette : 2;
            uint32_t DXOffset : 10;
            uint32_t DYOffset : 10;
            uint32_t DZ : 3;
            uint32_t DIndex : 6;
            uint32_t DMode : 1;
        };
        struct SBackgroundControlPixelMode{
            uint32_t DPalette : 2;
            uint32_t DXOffset : 10;
            uint32_t DYOffset : 10;
            uint32_t DZ : 3;
            uint32_t DReserved : 4;
            uint32_t DIndex : 2;
            uint32_t DMode : 1;
        };
        struct SBackgroundControlTileMode{
            uint32_t DPalette : 2;
            uint32_t DXOffset : 10;
            uint32_t DYOffset : 10;
            uint32_t DZ : 3;
            uint32_t DSubIndex : 3;
            uint32_t DIndex : 3;
            uint32_t DMode : 1;
        };
        struct SVideoControllerMode{
            uint32_t DMode : 1;
            uint32_t DRefresh : 7;
            uint32_t DReserved : 24;
        };
    protected:
        struct SSurfaceRefreshData{
            const uint32_t *DPalette;
            const uint8_t *DVideoMemory;
        };

        struct SSurfaceRefreshTileData{
            const uint32_t *DPalette;
            const uint8_t *DTileMemory;
            const uint8_t *DVideoMemory;
        };

        struct SRenderedSurface{
            int DX;
            int DY;
            std::shared_ptr< CGraphicSurface > DSurface;
        };

        std::shared_ptr< CRAMMemoryDevice > DVideoRAM;
        std::shared_ptr<CGraphicFactory> DGraphicFactory;
        std::vector< std::shared_ptr<CGraphicSurface> > DBackgrounds;
        std::vector< std::shared_ptr<CGraphicSurface> > DLargeSprites;
        std::vector< std::shared_ptr<CGraphicSurface> > DMediumSprites;
        std::vector< std::shared_ptr<CGraphicSurface> > DSmallSprites;
        std::vector< const uint8_t * > DBackgroundBases;
        std::vector< const uint8_t * > DBackgroundTileSetBases;
        std::vector< const uint8_t * > DBackgroundTiledImageBases;
        std::vector< const uint8_t * > DLargeSpriteBases;
        std::vector< const uint8_t * > DMediumSpriteBases;
        std::vector< const uint8_t * > DSmallSpriteBases;
        std::vector< const uint32_t * > DBackgroundPalettes;
        std::vector< const uint32_t * > DLargeSpritePalettes;
        std::vector< const uint32_t * > DMediumSpritePalettes;
        std::vector< const uint32_t * > DSmallSpritePalettes;
        std::vector< uint32_t > DSegmentBases;

        const SBackgroundControl *DBackgroundControls;
        const SLargeSpriteControl *DLargeSpriteControls;
        const SMediumSpriteControl *DMediumSpriteControls;
        const SSmallSpriteControl *DSmallSpriteControls;

        const uint8_t *DFontBase;
        const uint8_t *DTextBase;
        const uint8_t *DTextColorBase;
        const uint32_t *DTextForegroundPaletteBase;
        const uint32_t *DTextBackgroundPaletteBase;

        const SVideoControllerMode *DModeControl;
        uint32_t DRefreshCounter;

        static const uint32_t DScreenWidth;
        static const uint32_t DScreenHeight;
        static const uint32_t DBackgroundTileWidth;
        static const uint32_t DBackgroundTileHeight;
        static const uint32_t DBackgroundTileCount;
        static const uint32_t DBackgroundTileGroupPerData;
        static const uint32_t DBackgroundTileImagePerGroup;
        static const uint32_t DLargeSpriteWidth;
        static const uint32_t DLargeSpriteHeight;
        static const uint32_t DMediumSpriteWidth;
        static const uint32_t DMediumSpriteHeight;
        static const uint32_t DSmallSpriteWidth;
        static const uint32_t DSmallSpriteHeight;
        static const uint32_t DPaletteCount;
        static const uint32_t DPaletteEntryCount;
        static const uint32_t DZLevels;
        static const uint32_t DTextWidth;
        static const uint32_t DTextHeight;
        static const uint32_t DCharacterCount;
        static const uint32_t DCharacterWidth;
        static const uint32_t DCharacterHeight;
        static const uint32_t DCharacterStride;
        static const uint32_t DTextPaletteEntryCount;
        static const uint32_t DDefaultTextPalette[];
        static const uint32_t DBackgroundDataCount;
        static const uint32_t DLargeSpriteDataCount;
        static const uint32_t DMediumSpriteDataCount;
        static const uint32_t DSmallSpriteDataCount;
        static const uint32_t DBackgroundCount;
        static const uint32_t DLargeSpriteCount;
        static const uint32_t DMediumSpriteCount;
        static const uint32_t DSmallSpriteCount;
        static const uint32_t DRefreshBaseCount;
        static const uint32_t DColorBlack;
        static const uint32_t DColorWhite;
        static const uint32_t DColorBlue;
        static const uint32_t DColorGreen;
        static const uint32_t DColorRed;
        static const uint32_t DColorCyan;
        static const uint32_t DColorMagenta;
        static const uint32_t DColorBrown;
        static const uint32_t DColorLightGray;
        static const uint32_t DColorDarkGray;
        static const uint32_t DColorBrightBlue;
        static const uint32_t DColorBrightGreen;
        static const uint32_t DColorBrightRed;
        static const uint32_t DColorBrightCyan;
        static const uint32_t DColorBrightMagenta;
        static const uint32_t DColorYellow;
        


        static bool RefreshTextModeCallback(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height);
        static bool RefreshGraphicsSurfaceRefreshCallback(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height);
        static bool RefreshGraphicsSurfaceRefreshTileCallback(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height);

        bool RefreshTextMode(uint8_t *data, ESurfaceFormat format, int stride, int width, int height);
        bool RefreshGraphicsMode(std::shared_ptr<CGraphicSurface> surface);

    public:
        CModel02VideoController(std::shared_ptr<CGraphicFactory> graphicfactory);
        ~CModel02VideoController(){};

        uint32_t ScreenWidth() const override{
            return DScreenWidth;
        };

        uint32_t ScreenHeight() const override{
            return DScreenHeight;
        };

        std::shared_ptr< CRAMMemoryDevice > VideoRAM() const override{
            return DVideoRAM;
        };

        std::vector< uint32_t > SegmentBases() const override{
            return DSegmentBases;
        };

        void Refresh(std::shared_ptr<CGraphicSurface> surface) override;

        bool Tick() override;

        void Reset() override;
};

#endif
