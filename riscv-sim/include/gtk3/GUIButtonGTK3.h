#ifndef GUIBUTTONGTK3_H
#define GUIBUTTONGTK3_H
#include "GUIButton.h"
#include "GUIContainerGTK3.h"
#include <gtk/gtk.h>

class CGUIButtonGTK3 : public virtual CGUIButton, public CGUIContainerGTK3{
    public:
        CGUIButtonGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIButtonGTK3();
        
        virtual std::string GetLabel() const override;
        virtual void SetLabel(const std::string &label) override;
};

#endif