#ifndef GUIGRIDGTK3_H
#define GUIGRIDGTK3_H

#include "GUIContainerGTK3.h"
#include "GUIGrid.h"
#include <gtk/gtk.h>

class CGUIGridGTK3 : public virtual CGUIGrid, public CGUIContainerGTK3{
    public:
        CGUIGridGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIGridGTK3();
        
        virtual unsigned int GetRowSpacing() const override;
        virtual void SetRowSpacing(unsigned int spacing) override;
        virtual void RemoveRow(int row) override;
        virtual unsigned int GetColumnSpacing() const override;
        virtual void SetColumnSpacing(unsigned int spacing) override;
        virtual void RemoveColumn(int col) override;
        void Attach(std::shared_ptr<CGUIWidget> widget, int left, int top, int width, int height) override;
};

#endif