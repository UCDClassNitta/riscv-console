#ifndef GUIMENUBARGTK3_H
#define GUIMENUBARGTK3_H
#include "GUIMenuShellGTK3.h"
#include "GUIMenuBar.h"
#include <gtk/gtk.h>

class CGUIMenuBarGTK3 : public virtual CGUIMenuBar, public CGUIMenuShellGTK3{
    public:
        CGUIMenuBarGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuBarGTK3();
        
};

#endif