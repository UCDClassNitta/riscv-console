#ifndef GUIBOXGTK3_H
#define GUIBOXGTK3_H

#include "GUIBox.h"
#include "GUIContainerGTK3.h"
#include <gtk/gtk.h>

class CGUIBoxGTK3 : public virtual CGUIBox, public CGUIContainerGTK3{
    public:
        CGUIBoxGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIBoxGTK3();
        
        void PackStart(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding) override;
        void PackEnd(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding) override;
};

#endif