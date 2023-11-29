#ifndef GUIMENUGTK3_H
#define GUIMENUGTK3_H

#include "GUIMenu.h"
#include "GUIMenuShellGTK3.h"
#include <gtk/gtk.h>

class CGUIMenuGTK3 : public virtual CGUIMenu, public CGUIMenuShellGTK3{
    public:
        CGUIMenuGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuGTK3();

        void PopupAtWidget(std::shared_ptr< CGUIWidget > widget, EGUIOriginAnchor widgetanchor, EGUIOriginAnchor menuanchor);
        void PopupAtPointer();
};

#endif