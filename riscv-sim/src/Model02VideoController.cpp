#include "Model02VideoController.h"
#include "MSXFont.h"
#include "GraphicFactory.h"
#include "FileDataSink.h"
#include <chrono>
#include <cstring>

const uint32_t CModel02VideoController::DScreenWidth = 512;    // Provides 16:9 aspect ratio with 512 width
const uint32_t CModel02VideoController::DScreenHeight = 288;
const uint32_t CModel02VideoController::DBackgroundTileWidth = 16;
const uint32_t CModel02VideoController::DBackgroundTileHeight = 16;
const uint32_t CModel02VideoController::DBackgroundTileCount = 256;
const uint32_t CModel02VideoController::DBackgroundTileGroupPerData = 2;
const uint32_t CModel02VideoController::DBackgroundTileImagePerGroup = 8;
const uint32_t CModel02VideoController::DLargeSpriteWidth = 64;
const uint32_t CModel02VideoController::DLargeSpriteHeight = 64;
const uint32_t CModel02VideoController::DMediumSpriteWidth = 32;
const uint32_t CModel02VideoController::DMediumSpriteHeight = 32;
const uint32_t CModel02VideoController::DSmallSpriteWidth = 16;
const uint32_t CModel02VideoController::DSmallSpriteHeight = 16;
const uint32_t CModel02VideoController::DPaletteCount = 4;
const uint32_t CModel02VideoController::DPaletteEntryCount = 256;
const uint32_t CModel02VideoController::DZLevels = 8;
const uint32_t CModel02VideoController::DTextWidth = DScreenWidth / MSXFontWidth;
const uint32_t CModel02VideoController::DTextHeight = DScreenHeight / MSXFontHeight;
const uint32_t CModel02VideoController::DCharacterCount = 256;
const uint32_t CModel02VideoController::DCharacterWidth = (MSXFontWidth + 7)/8;
const uint32_t CModel02VideoController::DCharacterHeight = MSXFontHeight;
const uint32_t CModel02VideoController::DCharacterStride = DCharacterWidth * DCharacterHeight;
const uint32_t CModel02VideoController::DTextPaletteEntryCount = 16;
const uint32_t CModel02VideoController::DBackgroundDataCount = 4;
const uint32_t CModel02VideoController::DLargeSpriteDataCount = 64;
const uint32_t CModel02VideoController::DMediumSpriteDataCount = 64;
const uint32_t CModel02VideoController::DSmallSpriteDataCount = 256;
const uint32_t CModel02VideoController::DBackgroundCount = 64;
const uint32_t CModel02VideoController::DLargeSpriteCount = 256;
const uint32_t CModel02VideoController::DMediumSpriteCount = 256;
const uint32_t CModel02VideoController::DSmallSpriteCount = 256;
const uint32_t CModel02VideoController::DRefreshBaseCount = 10;
const uint32_t CModel02VideoController::DColorBlack = 0xFF000000;
const uint32_t CModel02VideoController::DColorWhite = 0xFFFFFFFF;
const uint32_t CModel02VideoController::DColorBlue = 0xFF100B9B;
const uint32_t CModel02VideoController::DColorGreen = 0xFF00A024;
const uint32_t CModel02VideoController::DColorRed = 0xFFA8000E;
const uint32_t CModel02VideoController::DColorCyan = 0xFF00A09F;
const uint32_t CModel02VideoController::DColorMagenta = 0xFFA9039C;
const uint32_t CModel02VideoController::DColorBrown = 0xFFA64A16;
const uint32_t CModel02VideoController::DColorLightGray = 0xFFD0D0D0;
const uint32_t CModel02VideoController::DColorDarkGray = 0xFFA0A0A0;
const uint32_t CModel02VideoController::DColorBrightBlue = 0xFF4F4DF8;
const uint32_t CModel02VideoController::DColorBrightGreen = 0xFF00FF5D;
const uint32_t CModel02VideoController::DColorBrightRed = 0xFFFF494E;
const uint32_t CModel02VideoController::DColorBrightCyan = 0xFF06FFFE;
const uint32_t CModel02VideoController::DColorBrightMagenta = 0xFFFF4BF9;
const uint32_t CModel02VideoController::DColorYellow = 0xFFFEFE60;
const uint32_t CModel02VideoController::DDefaultTextPalette[] = {DColorWhite, DColorBlack, DColorLightGray, DColorDarkGray, 
                                                                DColorBlue, DColorGreen, DColorCyan, DColorRed, 
                                                                DColorMagenta, DColorBrown, DColorBrightBlue, DColorBrightGreen, 
                                                                DColorBrightCyan, DColorBrightRed, DColorBrightMagenta, DColorYellow};

CModel02VideoController::CModel02VideoController(std::shared_ptr<CGraphicFactory> graphicfactory){
    DGraphicFactory = graphicfactory;
    DVideoRAM = std::make_shared<CRAMMemoryDevice>(0x100000);
    uint8_t *MemoryBase = DVideoRAM->Data().data();
    uint32_t FontSize = DCharacterCount * DCharacterStride;
    // Setup memory locations
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    for(uint32_t BackgroundIndex = 0; BackgroundIndex < DBackgroundDataCount; BackgroundIndex++){       
        DBackgroundBases.push_back(MemoryBase);
        for(uint32_t TileGroupIndex = 0; TileGroupIndex < DBackgroundTileGroupPerData; TileGroupIndex++){
            uint8_t *TileGroupMemoryBase = MemoryBase + (TileGroupIndex * DScreenWidth * DScreenHeight / DBackgroundTileGroupPerData);
            DBackgroundTileSetBases.push_back(TileGroupMemoryBase);
            uint8_t *TileGroupImageMemoryBase = TileGroupMemoryBase + DBackgroundTileWidth * DBackgroundTileHeight * DBackgroundTileCount;
            for(uint32_t TileGroupImageIndex = 0; TileGroupImageIndex < DBackgroundTileImagePerGroup; TileGroupImageIndex++){
                DBackgroundTiledImageBases.push_back(TileGroupImageMemoryBase);
                TileGroupImageMemoryBase += DScreenWidth * DScreenHeight / (DBackgroundTileWidth * DBackgroundTileHeight);
            }
        }
        MemoryBase += DScreenWidth * DScreenHeight;
    }
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    for(uint32_t LargeSpriteIndex = 0; LargeSpriteIndex < DLargeSpriteDataCount; LargeSpriteIndex++){
        DLargeSpriteBases.push_back(MemoryBase);
        MemoryBase += DLargeSpriteWidth * DLargeSpriteHeight;
    }
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    for(uint32_t MediumSpriteIndex = 0; MediumSpriteIndex < DMediumSpriteDataCount; MediumSpriteIndex++){
        DMediumSpriteBases.push_back(MemoryBase);
        MemoryBase += DMediumSpriteWidth * DMediumSpriteHeight;
    }
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    for(uint32_t SmallSpriteIndex = 0; SmallSpriteIndex < DSmallSpriteDataCount; SmallSpriteIndex++){
        DSmallSpriteBases.push_back(MemoryBase);
        MemoryBase += DSmallSpriteWidth * DSmallSpriteHeight;
    }
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    for(uint32_t PaletteIndex = 0; PaletteIndex < DPaletteCount; PaletteIndex++){
        DBackgroundPalettes.push_back((uint32_t *)MemoryBase);
        MemoryBase += sizeof(uint32_t) * DPaletteEntryCount;
    }
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    for(uint32_t PaletteIndex = 0; PaletteIndex < DPaletteCount; PaletteIndex++){
        DLargeSpritePalettes.push_back((uint32_t *)MemoryBase);
        MemoryBase += sizeof(uint32_t) * DPaletteEntryCount;
    }
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    for(uint32_t PaletteIndex = 0; PaletteIndex < DPaletteCount; PaletteIndex++){
        DMediumSpritePalettes.push_back((uint32_t *)MemoryBase);
        MemoryBase += sizeof(uint32_t) * DPaletteEntryCount;
    }
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    for(uint32_t PaletteIndex = 0; PaletteIndex < DPaletteCount; PaletteIndex++){
        DSmallSpritePalettes.push_back((uint32_t *)MemoryBase);
        MemoryBase += sizeof(uint32_t) * DPaletteEntryCount;
    }
    // Font memory 
    DFontBase = MemoryBase;
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    MemoryBase += FontSize;
    // Text memory
    DTextBase = MemoryBase;
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    MemoryBase += DTextWidth * DTextHeight;
    // Text color memory
    DTextColorBase = MemoryBase;
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    MemoryBase += DTextWidth * DTextHeight;
    // Background controls
    DBackgroundControls = (SBackgroundControl *)MemoryBase;
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    MemoryBase += sizeof(SBackgroundControl) * DBackgroundCount;
    // Large sprite controls
    DLargeSpriteControls = (SLargeSpriteControl *)MemoryBase;
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    MemoryBase += sizeof(SLargeSpriteControl) * DLargeSpriteCount;
    // Medium sprite controls
    DMediumSpriteControls = (SMediumSpriteControl *)MemoryBase;
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    MemoryBase += sizeof(SMediumSpriteControl) * DLargeSpriteCount;
    // Small sprite controls
    DSmallSpriteControls = (SSmallSpriteControl *)MemoryBase;
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    MemoryBase += sizeof(SSmallSpriteControl) * DSmallSpriteCount;
    // Foreground palette
    DTextForegroundPaletteBase = (uint32_t *)MemoryBase;
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    MemoryBase += sizeof(uint32_t) * DTextPaletteEntryCount;
    // Background palette
    DTextBackgroundPaletteBase = (uint32_t *)MemoryBase;
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    MemoryBase += sizeof(uint32_t) * DTextPaletteEntryCount;
    // Controller mode
    DModeControl = (const SVideoControllerMode *)MemoryBase;
    DSegmentBases.push_back(MemoryBase - DVideoRAM->Data().data());
    DRefreshCounter = DRefreshBaseCount;

    for(uint32_t BackgroundIndex = 0; BackgroundIndex < DBackgroundCount; BackgroundIndex++){
        DBackgrounds.push_back(DGraphicFactory->CreateSurface(DScreenWidth, DScreenHeight, ESurfaceFormat::ARGB32));
    }
    for(uint32_t LargeSpriteIndex = 0; LargeSpriteIndex < DLargeSpriteCount; LargeSpriteIndex++){
        DLargeSprites.push_back(DGraphicFactory->CreateSurface(DLargeSpriteWidth, DLargeSpriteHeight, ESurfaceFormat::ARGB32));
    }
    for(uint32_t MediumSpriteIndex = 0; MediumSpriteIndex < DMediumSpriteCount; MediumSpriteIndex++){
        DMediumSprites.push_back(DGraphicFactory->CreateSurface(DMediumSpriteWidth, DMediumSpriteHeight, ESurfaceFormat::ARGB32));
    }
    for(uint32_t SmallSpriteIndex = 0; SmallSpriteIndex < DSmallSpriteCount; SmallSpriteIndex++){
        DSmallSprites.push_back(DGraphicFactory->CreateSurface(DSmallSpriteWidth, DSmallSpriteHeight, ESurfaceFormat::ARGB32));
    }
}

bool CModel02VideoController::RefreshTextModeCallback(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height){
    CModel02VideoController *VideoController = static_cast<CModel02VideoController *>(calldata);
    return VideoController->RefreshTextMode(data,format,stride,width,height);
}

bool CModel02VideoController::RefreshGraphicsSurfaceRefreshCallback(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height){
    const uint32_t *Palette = static_cast<SSurfaceRefreshData *>(calldata)->DPalette;
    const uint8_t *VideoMemory = static_cast<SSurfaceRefreshData *>(calldata)->DVideoMemory;

    uint32_t *SurfaceMemory = (uint32_t *)data;
    uint32_t PixelCount = width * height;
    while(PixelCount--){
        *SurfaceMemory++ = Palette[*VideoMemory++];
    }
    return true;
}

bool CModel02VideoController::RefreshGraphicsSurfaceRefreshTileCallback(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height){
    const uint32_t *Palette = static_cast<SSurfaceRefreshTileData *>(calldata)->DPalette;
    const uint8_t *TileMemory = static_cast<SSurfaceRefreshTileData *>(calldata)->DTileMemory;
    const uint8_t *VideoMemory = static_cast<SSurfaceRefreshTileData *>(calldata)->DVideoMemory;

    uint32_t *SurfaceMemory = (uint32_t *)data;
    uint32_t TilePixelCount = DBackgroundTileWidth * DBackgroundTileHeight;
    uint32_t XTiles = width / DBackgroundTileWidth;
    uint32_t YTiles = height / DBackgroundTileHeight;
    for(uint32_t YIndex = 0; YIndex < YTiles; YIndex++){
        for(uint32_t XIndex = 0; XIndex < XTiles; XIndex++){
            const uint8_t *TilePixels = TileMemory + (*VideoMemory) * TilePixelCount;
            uint32_t *BaseSurface = SurfaceMemory + XIndex * DBackgroundTileWidth;
            for(uint32_t YTileIndex = 0; YTileIndex < DBackgroundTileHeight; YTileIndex++){
                for(uint32_t XTileIndex = 0; XTileIndex < DBackgroundTileWidth; XTileIndex++){
                    *BaseSurface++ = Palette[*TilePixels++];
                }
                BaseSurface += width - DBackgroundTileWidth;
            }
            VideoMemory++;
        }
        SurfaceMemory += width * DBackgroundTileHeight;
    }
    return true;
}

void CModel02VideoController::Refresh(std::shared_ptr<CGraphicSurface> surface){
    if(DModeControl->DMode){
        RefreshGraphicsMode(surface);
    }
    else{
        surface->AccessRawData(this,RefreshTextModeCallback);
    }
}

bool CModel02VideoController::Tick(){
    if(--DRefreshCounter){
        return false;
    }
    DRefreshCounter = DModeControl->DRefresh ? DModeControl->DRefresh : DRefreshBaseCount;
    return true;
}

bool CModel02VideoController::RefreshTextMode(uint8_t *data, ESurfaceFormat format, int stride, int width, int height){
    for(uint32_t RowIndex = 0; RowIndex < DTextHeight; RowIndex++){
        for(uint32_t ColIndex = 0; ColIndex < DTextWidth; ColIndex++){
            uint8_t OutChar = DTextBase[RowIndex * DTextWidth + ColIndex];
            uint8_t OutColor = DTextColorBase[RowIndex * DTextWidth + ColIndex];
            uint32_t ForegroundColor = DTextForegroundPaletteBase[OutColor & (DTextPaletteEntryCount-1)];
            uint32_t BackgroundColor = DTextBackgroundPaletteBase[(OutColor/DTextPaletteEntryCount) & (DTextPaletteEntryCount-1)];
            for(uint32_t CharRow = 0; CharRow < DCharacterHeight; CharRow++){
                uint32_t *RowPointer = (uint32_t *)(data + stride * (RowIndex * DCharacterHeight + CharRow));
                RowPointer += ColIndex * DCharacterWidth * 8;
                for(uint32_t CharRowByteIndex = 0; CharRowByteIndex < DCharacterWidth; CharRowByteIndex++){
                    uint8_t BitMask = 0x80;
                    uint8_t CharRowByte = DFontBase[DCharacterStride * OutChar + CharRow * DCharacterWidth + CharRowByteIndex];
                    while(BitMask){
                        *RowPointer++ = CharRowByte & BitMask ? ForegroundColor : BackgroundColor;
                        BitMask >>= 1;
                    }
                }
            }
        }
    }
    for(uint32_t RowIndex = DTextHeight * DCharacterHeight; RowIndex < uint32_t(height); RowIndex++){
        uint32_t *RowPointer = (uint32_t *)(data + stride * RowIndex);
        for(uint32_t ColIndex = 0; ColIndex < uint32_t(width); ColIndex++){
            *RowPointer++ = DColorBlack;
        }
    }
    return true;
}

bool CModel02VideoController::RefreshGraphicsMode(std::shared_ptr<CGraphicSurface> surface){
    std::vector< std::vector< SRenderedSurface > > RenderedSurfaces(DZLevels);

    for(uint32_t Index = 0; Index < DBackgroundCount; Index++){
        if(DBackgroundControls[Index].DXOffset && DBackgroundControls[Index].DYOffset && (DBackgroundControls[Index].DYOffset < DScreenHeight * 2)){
            // Render the background
            if(DBackgroundControls[Index].DMode){
                // Do tile mode
                SBackgroundControlTileMode *BackgroundTileMode = (SBackgroundControlTileMode *)&DBackgroundControls[Index];
                SSurfaceRefreshTileData Data = {DBackgroundPalettes[BackgroundTileMode->DPalette],  DBackgroundTileSetBases[BackgroundTileMode->DIndex], DBackgroundTiledImageBases[BackgroundTileMode->DIndex*DBackgroundTileImagePerGroup + BackgroundTileMode->DSubIndex]};
                DBackgrounds[Index]->AccessRawData(&Data,RefreshGraphicsSurfaceRefreshTileCallback);
                SRenderedSurface RenderedSurface;
                RenderedSurface.DX = int(BackgroundTileMode->DXOffset) - DScreenWidth;
                RenderedSurface.DY = int(BackgroundTileMode->DYOffset) - DScreenHeight;
                RenderedSurface.DSurface = DBackgrounds[Index];
                RenderedSurfaces[BackgroundTileMode->DZ].push_back(RenderedSurface);
            }
            else{
                // Do pixel mode
                SBackgroundControlPixelMode *BackgroundPixelMode = (SBackgroundControlPixelMode *)&DBackgroundControls[Index];
                SSurfaceRefreshData Data = {DBackgroundPalettes[BackgroundPixelMode->DPalette], DBackgroundBases[BackgroundPixelMode->DIndex]};
                DBackgrounds[Index]->AccessRawData(&Data,RefreshGraphicsSurfaceRefreshCallback);
                SRenderedSurface RenderedSurface;
                RenderedSurface.DX = int(BackgroundPixelMode->DXOffset) - DScreenWidth;
                RenderedSurface.DY = int(BackgroundPixelMode->DYOffset) - DScreenHeight;
                RenderedSurface.DSurface = DBackgrounds[Index];
                RenderedSurfaces[BackgroundPixelMode->DZ].push_back(RenderedSurface);
            }
        }
    }
    for(uint32_t Index = 0; Index < DLargeSpriteCount; Index++){
        if(DLargeSpriteControls[Index].DXOffset && (DLargeSpriteControls[Index].DXOffset < DScreenWidth + DLargeSpriteWidth) && DLargeSpriteControls[Index].DYOffset && (DLargeSpriteControls[Index].DYOffset < DScreenHeight + DLargeSpriteWidth)){
            // Render the sprite
            SSurfaceRefreshData Data = {DLargeSpritePalettes[DLargeSpriteControls[Index].DPalette], DLargeSpriteBases[DLargeSpriteControls[Index].DIndex]};
            DLargeSprites[Index]->AccessRawData(&Data,RefreshGraphicsSurfaceRefreshCallback);
            SRenderedSurface RenderedSurface;
            RenderedSurface.DX = int(DLargeSpriteControls[Index].DXOffset) - DLargeSpriteWidth;
            RenderedSurface.DY = int(DLargeSpriteControls[Index].DYOffset) - DLargeSpriteHeight;
            RenderedSurface.DSurface = DLargeSprites[Index];
            RenderedSurfaces[DLargeSpriteControls[Index].DZ].push_back(RenderedSurface);
        }
    }
    for(uint32_t Index = 0; Index < DMediumSpriteCount; Index++){
        if(DMediumSpriteControls[Index].DXOffset && (DMediumSpriteControls[Index].DXOffset < DScreenWidth + DMediumSpriteWidth) && DMediumSpriteControls[Index].DYOffset && (DMediumSpriteControls[Index].DYOffset < DScreenHeight + DMediumSpriteWidth)){
            // Render the sprite
            SSurfaceRefreshData Data = {DMediumSpritePalettes[DMediumSpriteControls[Index].DPalette], DMediumSpriteBases[DMediumSpriteControls[Index].DIndex]};
            DMediumSprites[Index]->AccessRawData(&Data,RefreshGraphicsSurfaceRefreshCallback);
            SRenderedSurface RenderedSurface;
            RenderedSurface.DX = int(DMediumSpriteControls[Index].DXOffset) - DMediumSpriteWidth;
            RenderedSurface.DY = int(DMediumSpriteControls[Index].DYOffset) - DMediumSpriteHeight;
            RenderedSurface.DSurface = DMediumSprites[Index];
            RenderedSurfaces[DMediumSpriteControls[Index].DZ].push_back(RenderedSurface);
        }
    }
    for(uint32_t Index = 0; Index < DSmallSpriteCount; Index++){
        if(DSmallSpriteControls[Index].DXOffset && (DSmallSpriteControls[Index].DXOffset < DScreenWidth + DSmallSpriteWidth) && DSmallSpriteControls[Index].DYOffset && (DSmallSpriteControls[Index].DYOffset < DScreenHeight + DSmallSpriteWidth)){
            // Render the sprite
            SSurfaceRefreshData Data = {DSmallSpritePalettes[DSmallSpriteControls[Index].DPalette], DSmallSpriteBases[DSmallSpriteControls[Index].DIndex]};
            DSmallSprites[Index]->AccessRawData(&Data,RefreshGraphicsSurfaceRefreshCallback);
            SRenderedSurface RenderedSurface;
            RenderedSurface.DX = int(DSmallSpriteControls[Index].DXOffset) - DSmallSpriteWidth;
            RenderedSurface.DY = int(DSmallSpriteControls[Index].DYOffset) - DSmallSpriteHeight;
            RenderedSurface.DSurface = DSmallSprites[Index];
            RenderedSurfaces[DSmallSpriteControls[Index].DZ].push_back(RenderedSurface);
        }
    }
    
    auto TempRC = surface->CreateResourceContext();
    TempRC->SetSourceRGB(0x0);
    TempRC->SetLineWidth(1);
    TempRC->Rectangle(0,0,DScreenWidth,DScreenHeight);
    TempRC->Fill();
    
    for(auto &ZRenderedSurfaces : RenderedSurfaces){
        for(auto &RenderedSurface : ZRenderedSurfaces){
            surface->Draw(RenderedSurface.DSurface,RenderedSurface.DX,RenderedSurface.DY,-1,-1,0,0);
        }
    }

    return true;
}

void CModel02VideoController::Reset(){
    uint32_t FontSize = 256 * ((MSXFontWidth + 7)/8) * MSXFontHeight;
    memset(DVideoRAM->Data().data(),0,DVideoRAM->Data().size());
    memcpy((char *)DFontBase,MSXFontData,FontSize);
    memcpy((char *)DTextForegroundPaletteBase,DDefaultTextPalette,sizeof(DDefaultTextPalette));
    memcpy((char *)DTextBackgroundPaletteBase,DDefaultTextPalette,sizeof(DDefaultTextPalette));
    // Swap background W & B
    ((uint32_t *)DTextBackgroundPaletteBase)[0] = DTextForegroundPaletteBase[1];
    ((uint32_t *)DTextBackgroundPaletteBase)[1] = DTextForegroundPaletteBase[0];
    DRefreshCounter = DRefreshBaseCount;
}
