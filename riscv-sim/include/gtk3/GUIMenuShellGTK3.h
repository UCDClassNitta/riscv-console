#ifndef GUIMENUSHELLGTK3_H
#define GUIMENUSHELLGTK3_H

#include "GUIMenuShell.h"
#include "GUIContainerGTK3.h"
#include <gtk/gtk.h>


class CGUIMenuShellGTK3 : public virtual CGUIMenuShell, public CGUIContainerGTK3{
    public:
        CGUIMenuShellGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuShellGTK3();
        
        void Append(std::shared_ptr<CGUIWidget> widget) override;
};

#endif
