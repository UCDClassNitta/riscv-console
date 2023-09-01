#ifndef GUITOGGLEBUTTONGTK3_H
#define GUITOGGLEBUTTONGTK3_H

#include "GUIToggleButton.h"
#include "GUIButtonGTK3.h"
#include <gtk/gtk.h>

class CGUIToggleButtonGTK3 : public virtual CGUIToggleButton, public CGUIButtonGTK3{
    public:
        CGUIToggleButtonGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIToggleButtonGTK3();

        virtual bool GetActive() const;
        virtual void SetActive(bool active);
};

#endif