#ifndef GUIEVENTBOXGTK_H
#define GUIEVENTBOXGTK_H

#include "GUIEventBox.h"
#include "GUIContainerGTK3.h"
#include <gtk/gtk.h>

class CGUIEventBoxGTK3 : public virtual CGUIEventBox, public CGUIContainerGTK3{
    public:
        CGUIEventBoxGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIEventBoxGTK3();
};

#endif