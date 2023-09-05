
#ifndef GUISCROLLBARGTK3_H
#define GUISCROLLBARGTK3_H
#include "GUIScrollBar.h"
#include "GUIWidgetGTK3.h"
#include <gtk/gtk.h>

class CGUIScrollBarGTK3 : public virtual CGUIScrollBar, public CGUIWidgetGTK3{
    public:
        CGUIScrollBarGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIScrollBarGTK3();
        
        double GetValue() override;
        double GetLower() override;
        double GetUpper() override;
        double GetStepIncrement() override;
        double GetPageIncrement() override;
        double GetPageSize() override;
        void SetValue(double val) override;
        void SetLower(double lower) override;
        void SetUpper(double upper) override;
        void SetStepIncrement(double inc) override;
        void SetPageIncrement(double inc) override;
        void SetPageSize(double size) override;
};

#endif