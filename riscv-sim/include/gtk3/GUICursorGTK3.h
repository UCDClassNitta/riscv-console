
#ifndef GUICURSORGTK3_H
#define GUICURSORGTK3_H
#include "GUICursor.h"
#include <gtk/gtk.h>

class CGUICursorGTK3 : public CGUICursor{
    protected:
        GdkCursor *DCursor;
    public:
        
        CGUICursorGTK3(GdkCursor *cursor);
        virtual ~CGUICursorGTK3();
        
        GdkCursor *GetCursor() const{
            return DCursor;
        };

        ECursorType Type() override;
};

#endif

