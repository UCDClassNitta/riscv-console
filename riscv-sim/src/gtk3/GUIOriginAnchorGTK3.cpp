#include "GUIOriginAnchorGTK3.h"

const std::unordered_map<GdkGravity, EGUIOriginAnchor> SGUIOriginAnchorGTK3::DFromGTK3Mapping = {{GDK_GRAVITY_NORTH_WEST,EGUIOriginAnchor::NorthWest},
                                                                                                {GDK_GRAVITY_NORTH,EGUIOriginAnchor::North},
                                                                                                {GDK_GRAVITY_NORTH_EAST,EGUIOriginAnchor::NorthEast},
                                                                                                {GDK_GRAVITY_WEST,EGUIOriginAnchor::West},
                                                                                                {GDK_GRAVITY_CENTER,EGUIOriginAnchor::Center},
                                                                                                {GDK_GRAVITY_EAST,EGUIOriginAnchor::East},
                                                                                                {GDK_GRAVITY_SOUTH_WEST,EGUIOriginAnchor::SouthWest},
                                                                                                {GDK_GRAVITY_SOUTH,EGUIOriginAnchor::South},
                                                                                                {GDK_GRAVITY_SOUTH_EAST,EGUIOriginAnchor::SouthEast},
                                                                                                {GDK_GRAVITY_STATIC,EGUIOriginAnchor::Static}};
const std::unordered_map<EGUIOriginAnchor, GdkGravity> SGUIOriginAnchorGTK3::DToGTK3Mapping = {{EGUIOriginAnchor::NorthWest,GDK_GRAVITY_NORTH_WEST},
                                                                                                {EGUIOriginAnchor::North,GDK_GRAVITY_NORTH},
                                                                                                {EGUIOriginAnchor::NorthEast,GDK_GRAVITY_NORTH_EAST},
                                                                                                {EGUIOriginAnchor::West,GDK_GRAVITY_WEST},
                                                                                                {EGUIOriginAnchor::Center,GDK_GRAVITY_CENTER},
                                                                                                {EGUIOriginAnchor::East,GDK_GRAVITY_EAST},
                                                                                                {EGUIOriginAnchor::SouthWest,GDK_GRAVITY_SOUTH_WEST},
                                                                                                {EGUIOriginAnchor::South,GDK_GRAVITY_SOUTH},
                                                                                                {EGUIOriginAnchor::SouthEast,GDK_GRAVITY_SOUTH_EAST},
                                                                                                {EGUIOriginAnchor::Static,GDK_GRAVITY_STATIC}};

EGUIOriginAnchor SGUIOriginAnchorGTK3::TranslateFromGTK3Type(GdkGravity gtk3type){
    auto Search = DFromGTK3Mapping.find(gtk3type);
    if(DFromGTK3Mapping.end() == Search){
        return EGUIOriginAnchor::Static;
    }
    return Search->second;
}

GdkGravity SGUIOriginAnchorGTK3::TranslateToGTK3Type(EGUIOriginAnchor guitype){
    auto Search = DToGTK3Mapping.find(guitype);
    if(DToGTK3Mapping.end() == Search){
        return GDK_GRAVITY_STATIC;
    }
    return Search->second;
}