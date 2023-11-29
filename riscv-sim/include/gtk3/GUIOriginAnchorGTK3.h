
#ifndef GUIORIGINANCHORGTK3_H
#define GUIORIGINANCHORGTK3_H
#include "GUIOriginAnchor.h"
#include <glib.h>
#include <gdk/gdk.h>
#include <unordered_map>

struct SGUIOriginAnchorGTK3{
    static EGUIOriginAnchor TranslateFromGTK3Type(GdkGravity gtk3type);
    static GdkGravity TranslateToGTK3Type(EGUIOriginAnchor guitype);
    private:
        static const std::unordered_map<GdkGravity, EGUIOriginAnchor> DFromGTK3Mapping;
        static const std::unordered_map<EGUIOriginAnchor, GdkGravity> DToGTK3Mapping;
};

#endif
