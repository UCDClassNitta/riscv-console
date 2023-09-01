#ifndef GUIMENUITEMGTK3_H
#define GUIMENUITEMGTK3_H
#include "GUIContainerGTK3.h"
#include "GUILabelGTK3.h"
#include "GUIMenuItem.h"
#include <gtk/gtk.h>

class CGUIMenuItemGTK3 : public virtual CGUIMenuItem, public CGUIContainerGTK3{
    public:
        CGUIMenuItemGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuItemGTK3();
        
        std::shared_ptr<CGUILabel> GetLabel() override;
        void SetSubMenu(std::shared_ptr<CGUIWidget> widget) override;
};

#endif