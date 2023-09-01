#ifndef GUITEXTITERGTK3_H
#define GUITEXTITERGTK3_H
#include "GUITextIter.h"
#include <gtk/gtk.h>

class CGUITextIterGTK3 : public CGUITextIter{
    friend class CGUITextBufferGTK3;
    protected:
        GtkTextIter DIter;
    public:
        CGUITextIterGTK3();
        virtual ~CGUITextIterGTK3();

        bool MoveBackwardLines(int count) override;
        bool MoveForwardLines(int count) override; 
        bool MoveBackwardChars(int count) override;
        bool MoveForwardChars(int count) override;
};

#endif