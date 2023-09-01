
#ifndef GUIDISPLAYGTK3_H
#define GUIDISPLAYGTK3_H
#include "GUIDisplay.h"
#include <gtk/gtk.h>

class CGUIDisplayGTK3 : public CGUIDisplay{
    protected:
        GdkDisplay *DDisplay;
    public:
        CGUIDisplayGTK3(GdkDisplay *display);
        virtual ~CGUIDisplayGTK3();
        
        void Sync() override;
        void Flush() override;
        
        std::shared_ptr< CGUICursor > NewCursor(CGUICursor::ECursorType type) override;
};

#endif

