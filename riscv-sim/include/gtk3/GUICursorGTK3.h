
#ifndef GUICURSORGTK3_H
#define GUICURSORGTK3_H
#include "GUICursor.h"
#include "GUIWidgetGTK3.h"
#include <gtk/gtk.h>

class CGUICursorGTK3 : public CGUICursor{
    friend class CGUIWidgetGTK3;
    protected:
        GdkCursor *DCursor;
    public:
        
        CGUICursorGTK3(GdkCursor *cursor);
        virtual ~CGUICursorGTK3();
        
        ECursorType Type() override;
};

#endif

