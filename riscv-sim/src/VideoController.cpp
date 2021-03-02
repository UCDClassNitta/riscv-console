#include "VideoController.h"
#include "MSXFont.h"
#include "GraphicFactory.h"
#include "FileDataSink.h"
#include <chrono>
#include <cstring>

const uint32_t CVideoController::DScreenWidth = 512;    // Provides 16:9 aspect ratio with 512 width
const uint32_t CVideoController::DScreenHeight = 288;
const uint32_t CVideoController::DLargeSpriteWidth = 64;
const uint32_t CVideoController::DLargeSpriteHeight = 64;
const uint32_t CVideoController::DSmallSpriteWidth = 16;
const uint32_t CVideoController::DSmallSpriteHeight = 16;
const uint32_t CVideoController::DPaletteCount = 4;
const uint32_t CVideoController::DZLevels = 8;
const uint32_t CVideoController::DTextWidth = DScreenWidth / MSXFontWidth;
const uint32_t CVideoController::DTextHeight = DScreenHeight / MSXFontHeight;
const uint32_t CVideoController::DCharacterWidth = (MSXFontWidth + 7)/8;
const uint32_t CVideoController::DCharacterHeight = MSXFontHeight;
const uint32_t CVideoController::DCharacterStride = DCharacterWidth * DCharacterHeight;
const uint32_t CVideoController::DBackgroundCount = 5;
const uint32_t CVideoController::DLargeSpriteCount = 64;
const uint32_t CVideoController::DSmallSpriteCount = 128;
const uint32_t CVideoController::DRefreshBaseCount = 10;

CVideoController::CVideoController(){
    DVideoRAM = std::make_shared<CRAMMemoryDevice>(0x100000);
    uint8_t *MemoryBase = DVideoRAM->Data().data();
    uint32_t FontSize = 256 * ((MSXFontWidth + 7)/8) * MSXFontHeight;
    for(uint32_t BackgroundIndex = 0; BackgroundIndex < DBackgroundCount; BackgroundIndex++){
        DBackgrounds.push_back(CGraphicFactory::CreateSurface(DScreenWidth, DScreenHeight, ESurfaceFormat::ARGB32));
        DBackgroundBases.push_back(MemoryBase);
        MemoryBase += DScreenWidth * DScreenHeight;
    }
    for(uint32_t LargeSpriteIndex = 0; LargeSpriteIndex < DLargeSpriteCount; LargeSpriteIndex++){
        DLargeSprites.push_back(CGraphicFactory::CreateSurface(DLargeSpriteWidth, DLargeSpriteHeight, ESurfaceFormat::ARGB32));
        DLargeSpriteBases.push_back(MemoryBase);
        MemoryBase += DLargeSpriteWidth * DLargeSpriteHeight;
    }
    for(uint32_t SmallSpriteIndex = 0; SmallSpriteIndex < DSmallSpriteCount; SmallSpriteIndex++){
        DSmallSprites.push_back(CGraphicFactory::CreateSurface(DSmallSpriteWidth, DSmallSpriteHeight, ESurfaceFormat::ARGB32));
        DSmallSpriteBases.push_back(MemoryBase);
        MemoryBase += DSmallSpriteWidth * DSmallSpriteHeight;
    }
    for(uint32_t PaletteIndex = 0; PaletteIndex < DPaletteCount; PaletteIndex++){
        DBackgroundPalettes.push_back((uint32_t *)MemoryBase);
        MemoryBase += sizeof(uint32_t) * 256;
    }
    for(uint32_t PaletteIndex = 0; PaletteIndex < DPaletteCount; PaletteIndex++){
        DSpritePalettes.push_back((uint32_t *)MemoryBase);
        MemoryBase += sizeof(uint32_t) * 256;
    }
    DFontBase = MemoryBase;
    MemoryBase += FontSize;
    DTextBase = MemoryBase;
    MemoryBase += DTextWidth * DTextHeight;
    DBackgroundControls = (SBackgroundControl *)MemoryBase;
    MemoryBase += sizeof(SBackgroundControl) * DBackgroundCount;
    DLargeSpriteControls = (SLargeSpriteControl *)MemoryBase;
    MemoryBase += sizeof(SLargeSpriteControl) * DLargeSpriteCount;
    DSmallSpriteControls = (SSmallSpriteControl *)MemoryBase;
    MemoryBase += sizeof(SSmallSpriteControl) * DSmallSpriteCount;
    DModeControl = (const SVideoControllerMode *)MemoryBase;
    memcpy((char *)DFontBase,MSXFontData,FontSize);
    DRefreshCounter = DRefreshBaseCount;
    //strcpy((char *)DTextBase,"Hello World!");
}

bool CVideoController::RefreshTextModeCallback(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height){
    CVideoController *VideoController = static_cast<CVideoController *>(calldata);
    return VideoController->RefreshTextMode(data,format,stride,width,height);
}

bool CVideoController::RefreshGraphicsSurfaceRefreshCallback(void *calldata, uint8_t *data, ESurfaceFormat format, int stride, int width, int height){
    const uint32_t *Palette = static_cast<SSurfaceRefreshData *>(calldata)->DPalette;
    const uint8_t *VideoMemory = static_cast<SSurfaceRefreshData *>(calldata)->DVideoMemory;

    uint32_t *SurfaceMemory = (uint32_t *)data;
    uint32_t PixelCount = width * height;
    while(PixelCount--){
        *SurfaceMemory++ = Palette[*VideoMemory++];
    }
    return true;
}

void CVideoController::Refresh(std::shared_ptr<CGraphicSurface> surface){
    if(DModeControl->DMode){
        RefreshGraphicsMode(surface);
    }
    else{
        surface->AccessRawData(this,RefreshTextModeCallback);
    }
}

bool CVideoController::Tick(){
    if(--DRefreshCounter){
        return false;
    }
    DRefreshCounter = DModeControl->DRefresh ? DModeControl->DRefresh : DRefreshBaseCount;
    return true;
}

bool CVideoController::RefreshTextMode(uint8_t *data, ESurfaceFormat format, int stride, int width, int height){
    //auto Start = std::chrono::steady_clock::now();
    for(uint32_t RowIndex = 0; RowIndex < DTextHeight; RowIndex++){
        for(uint32_t ColIndex = 0; ColIndex < DTextWidth; ColIndex++){
            uint8_t OutChar = DTextBase[RowIndex * DTextWidth + ColIndex];
            for(uint32_t CharRow = 0; CharRow < DCharacterHeight; CharRow++){
                uint32_t *RowPointer = (uint32_t *)(data + stride * (RowIndex * DCharacterHeight + CharRow));
                RowPointer += ColIndex * DCharacterWidth * 8;
                for(uint32_t CharRowByteIndex = 0; CharRowByteIndex < DCharacterWidth; CharRowByteIndex++){
                    uint8_t BitMask = 0x80;
                    uint8_t CharRowByte = DFontBase[DCharacterStride * OutChar + CharRow * DCharacterWidth + CharRowByteIndex];
                    while(BitMask){
                        *RowPointer++ = CharRowByte & BitMask ? 0xFFFFFFFF : 0xFF000000;                        
                        BitMask >>= 1;
                    }
                }
            }
        }
    }
    for(uint32_t RowIndex = DTextHeight * DCharacterHeight; RowIndex < uint32_t(height); RowIndex++){
        uint32_t *RowPointer = (uint32_t *)(data + stride * RowIndex);
        for(uint32_t ColIndex = 0; ColIndex < uint32_t(width); ColIndex++){
            *RowPointer++ = 0xFF000000;
        }
    }
    //auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()-Start); 
    //printf("   %lldus => %lfHz\n",Duration.count(), 1000000.0/Duration.count());
    return true;
}

bool CVideoController::RefreshGraphicsMode(std::shared_ptr<CGraphicSurface> surface){
    std::vector< std::vector< uint32_t > > RenderingOrders(DZLevels);
    //auto Start = std::chrono::steady_clock::now();
    
    for(uint32_t Index = 0; Index < DBackgroundCount; Index++){
        SSurfaceRefreshData Data = {DBackgroundPalettes[DBackgroundControls[Index].DPalette], DBackgroundBases[Index]};
        DBackgrounds[Index]->AccessRawData(&Data,RefreshGraphicsSurfaceRefreshCallback);
        RenderingOrders[DBackgroundControls[Index].DZ].push_back(Index);
    }
    for(uint32_t Index = 0; Index < DLargeSpriteCount; Index++){
        SSurfaceRefreshData Data = {DSpritePalettes[DLargeSpriteControls[Index].DPalette], DLargeSpriteBases[Index]};
        DLargeSprites[Index]->AccessRawData(&Data,RefreshGraphicsSurfaceRefreshCallback);
        RenderingOrders[DZLevels/2].push_back(DBackgroundCount + Index);
    }
    for(uint32_t Index = 0; Index < DSmallSpriteCount; Index++){
        SSurfaceRefreshData Data = {DSpritePalettes[DSmallSpriteControls[Index].DPalette], DSmallSpriteBases[Index]};
        DSmallSprites[Index]->AccessRawData(&Data,RefreshGraphicsSurfaceRefreshCallback);
        RenderingOrders[DSmallSpriteControls[Index].DZ].push_back(DBackgroundCount + DLargeSpriteCount + Index);
    }
    
    auto TempRC = surface->CreateResourceContext();
    TempRC->SetSourceRGB(0x0);
    TempRC->SetLineWidth(1);
    TempRC->Rectangle(0,0,DScreenWidth,DScreenHeight);
    TempRC->Fill();
    
    for(auto &Renderings : RenderingOrders){
        for(auto Index : Renderings){
            if(Index < DBackgroundCount){
                if(DBackgroundControls[Index].DXOffset && DBackgroundControls[Index].DYOffset && (DBackgroundControls[Index].DYOffset < DScreenHeight * 2)){
                    // Some part is on screen
                    int DestX = DBackgroundControls[Index].DXOffset;
                    int DestY = DBackgroundControls[Index].DYOffset;
                    DestX -= DScreenWidth;
                    DestY -= DScreenHeight;
                    surface->Draw(DBackgrounds[Index],DestX,DestY,DScreenWidth,DScreenHeight,0,0);
                }
            }
            else if(Index < DBackgroundCount + DLargeSpriteCount){
                Index -= DBackgroundCount;
                if(DLargeSpriteControls[Index].DXOffset && (DLargeSpriteControls[Index].DXOffset < DScreenWidth + DLargeSpriteWidth) && DLargeSpriteControls[Index].DYOffset && (DLargeSpriteControls[Index].DYOffset < DScreenHeight + DLargeSpriteWidth)){
                    // Some part is on screen
                    int DestX = DLargeSpriteControls[Index].DXOffset;
                    int DestY = DLargeSpriteControls[Index].DYOffset;
                    DestX -= DLargeSpriteWidth;
                    DestY -= DLargeSpriteHeight;
                    surface->Draw(DLargeSprites[Index],DestX,DestY,DLargeSpriteControls[Index].DWidth+DLargeSpriteWidth/2+1,DLargeSpriteControls[Index].DHeight+DLargeSpriteHeight/2+1,0,0);
                }
            }
            else{
                Index -= DBackgroundCount + DLargeSpriteCount;
                if(DSmallSpriteControls[Index].DXOffset && (DSmallSpriteControls[Index].DXOffset < DScreenWidth + DSmallSpriteWidth) && DSmallSpriteControls[Index].DYOffset && (DSmallSpriteControls[Index].DYOffset < DScreenHeight + DSmallSpriteWidth)){
                    // Some part is on screen
                    int DestX = DSmallSpriteControls[Index].DXOffset;
                    int DestY = DSmallSpriteControls[Index].DYOffset;
                    DestX -= DSmallSpriteWidth;
                    DestY -= DSmallSpriteHeight;
                    surface->Draw(DSmallSprites[Index],DestX,DestY,DSmallSpriteControls[Index].DWidth+1,DSmallSpriteControls[Index].DHeight+1,0,0);
                }
            }
        }
    }
    //auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()-Start); 
    //printf("   %lldus => %lfHz\n",Duration.count(), 1000000.0/Duration.count());
    return true;
}

void CVideoController::Reset(){
    uint32_t FontSize = 256 * ((MSXFontWidth + 7)/8) * MSXFontHeight;
    memset(DVideoRAM->Data().data(),0,DVideoRAM->Data().size());
    memcpy((char *)DFontBase,MSXFontData,FontSize);
    DRefreshCounter = DRefreshBaseCount;
}
