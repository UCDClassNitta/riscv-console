#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include "RAMMemoryDevice.h"
#include "HardwareRegister.h"
#include "GraphicSurface.h"

class CVideoController{
    public:
        struct SLargeSpriteControl{
            uint32_t DPalette : 2;
            uint32_t DXOffset : 10;
            uint32_t DYOffset : 9;
            uint32_t DWidth : 5;
            uint32_t DHeight : 5;
            uint32_t DReserved : 1;
        };
        struct SSmallSpriteControl{
            uint32_t DPalette : 2;
            uint32_t DXOffset : 10;
            uint32_t DYOffset : 9;
            uint32_t DWidth : 4;
            uint32_t DHeight : 4;
            uint32_t DZ : 3;
        };
        struct SBackgroundControl{
            uint32_t DPalette : 2;
            uint32_t DXOffset : 10;
            uint32_t DYOffset : 10;
            uint32_t DZ : 3;
            uint32_t DReserved : 7;
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

        std::shared_ptr< CRAMMemoryDevice > DVideoRAM;
        std::vector< std::shared_ptr<CGraphicSurface> > DBackgrounds;
        std::vector< std::shared_ptr<CGraphicSurface> > DLargeSprites;
        std::vector< std::shared_ptr<CGraphicSurface> > DSmallSprites;
        std::vector< const uint8_t * > DBackgroundBases;
        std::vector< const uint8_t * > DLargeSpriteBases;
        std::vector< const uint8_t * > DSmallSpriteBases;
        std::vector< const uint32_t * > DBackgroundPalettes;
        std::vector< const uint32_t * > DSpritePalettes;

        const SBackgroundControl *DBackgroundControls;
        const SLargeSpriteControl *DLargeSpriteControls;
        const SSmallSpriteControl *DSmallSpriteControls;

        const uint8_t *DFontBase;
        const uint8_t *DTextBase;

        const SVideoControllerMode *DModeControl;
        uint32_t DRefreshCounter;

        static const uint32_t DScreenWidth;
        static const uint32_t DScreenHeight;
        static const uint32_t DLargeSpriteWidth;
        static const uint32_t DLargeSpriteHeight;
        static const uint32_t DSmallSpriteWidth;
        static const uint32_t DSmallSpriteHeight;
        static const uint32_t DPaletteCount;
        static const uint32_t DZLevels;
        static const uint32_t DTextWidth;
        static const uint32_t DTextHeight;
        static const uint32_t DCharacterWidth;
        static const uint32_t DCharacterHeight;
        static const uint32_t DCharacterStride;
        static const uint32_t DBackgroundCount;
        static const uint32_t DLargeSpriteCount;
        static const uint32_t DSmallSpriteCount;
        static const uint32_t DRefreshBaseCount;

        static bool RefreshTextModeCallback(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height);
        static bool RefreshGraphicsSurfaceRefreshCallback(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height);

        bool RefreshTextMode(uint8_t *data, ESurfaceFormat format, int stride, int width, int height);
        bool RefreshGraphicsMode(std::shared_ptr<CGraphicSurface> surface);

    public:
        CVideoController();

        uint32_t ScreenWidth() const{
            return DScreenWidth;
        };

        uint32_t ScreenHeight() const{
            return DScreenHeight;
        };

        std::shared_ptr< CRAMMemoryDevice > VideoRAM() const{
            return DVideoRAM;
        };

        void Refresh(std::shared_ptr<CGraphicSurface> surface);

        bool Tick();

        void Reset();
};

#endif
