
#ifndef GRAPHICTILESET_H
#define GRAPHICTILESET_H
#include <string>
#include <unordered_map>
#include <vector>
#include "GraphicSurface.h"
#include "DataSource.h"

class CGraphicTileset{
    protected:
        std::shared_ptr<CGraphicSurface> DSurfaceTileset;
        std::vector< std::shared_ptr<CGraphicSurface> > DClippingMasks;
        std::unordered_map< std::string, int > DMapping;
        std::vector< std::string > DTileNames;
        std::vector< std::string > DGroupNames;
        std::unordered_map< std::string, int > DGroupSteps;
        int DTileCount;
        int DTileWidth;
        int DTileHeight;
        int DTileHalfWidth;
        int DTileHalfHeight;
        
        static bool ParseGroupName(const std::string &tilename, std::string &aniname, int &anistep);
        void UpdateGroupNames();
        
    public:
        CGraphicTileset();
        virtual ~CGraphicTileset();

        int TileCount() const{
            return DTileCount;
        };
        int TileCount(int count);
        
        int TileWidth() const{
            return DTileWidth;
        };
        int TileHeight() const{
            return DTileHeight;
        };
        int TileHalfWidth() const{
            return DTileHalfWidth;
        };
        int TileHalfHeight() const{
            return DTileHalfHeight;
        };
        
        int FindTile(const std::string &tilename) const;
        int GroupCount() const;
        std::string GroupName(int index) const;
        int GroupSteps(int index) const;
        int GroupSteps(const std::string &Groupname) const;
                
        bool ClearTile(int index);
        bool DuplicateTile(int destindex, const std::string &tilename, int srcindex);
        bool DuplicateClippedTile(int destindex, const std::string &tilename, int srcindex, int clipindex);
        
        void CreateClippingMasks();
        
        bool LoadTileset(std::shared_ptr< CDataSource > source);
        
        void DrawTile(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int tileindex);
        
        void DrawClipped(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int tileindex, uint32_t rgb);
};

#endif

