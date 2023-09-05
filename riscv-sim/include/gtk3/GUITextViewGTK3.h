#ifndef GUITEXTVIEWGTK3_H
#define GUITEXTVIEWGTK3_H
#include "GUITextView.h"
#include "GUIContainerGTK3.h"
#include "GUITextBufferGTK3.h"
#include <gtk/gtk.h>

class CGUITextViewGTK3 : public virtual CGUITextView, public CGUIContainerGTK3{
    protected:
        std::shared_ptr<CGUITextBufferGTK3> DBuffer;
    public: 
        CGUITextViewGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUITextViewGTK3();

        void PlaceCursorOnscreen() override;
        void SetEditable(bool edit) override;
        void SetCursorVisible(bool visibility) override;
        void SetMonospace(bool monospace) override;
        void SetWrapMode(EWrapModeType wrap) override;
        std::shared_ptr<CGUITextBuffer> GetBuffer() override;
        int GetRequiredCharactersWidth(int chars) override;
        int GetRequiredLinesHeight(int lines) override;
        int GetLineNumberAtY(int y) override;
};


#endif