#ifndef GUIFRAMEGTK3_H
#define GUIFRAMEGTK3_H

#include "GUIFrame.h"
#include "GUIContainerGTK3.h"

#include <string>
#include <gtk/gtk.h>

class CGUIFrameGTK3 : public virtual CGUIFrame, public CGUIContainerGTK3{
    public:
        CGUIFrameGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIFrameGTK3();
        
        void SetLabel(const std::string &label) override;
};


#endif