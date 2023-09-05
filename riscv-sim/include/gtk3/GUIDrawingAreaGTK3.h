
#ifndef GUIDRAWINGAREAGTK3_H
#define GUIDRAWINGAREAGTK3_H
#include "GUIDrawingArea.h"
#include "GUIWidgetGTK3.h"

class CGUIDrawingAreaGTK3 : public virtual CGUIDrawingArea, public CGUIWidgetGTK3{
    public:
        CGUIDrawingAreaGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIDrawingAreaGTK3();
};

#endif

