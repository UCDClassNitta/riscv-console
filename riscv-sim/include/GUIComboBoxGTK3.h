#ifndef GUICOMBOBOXGTK3_H
#define GUICOMBOBOXGTK3_H

#include "GUIComboBox.h"
#include "GUIContainerGTK3.h"
#include <string>
#include <gtk/gtk.h>

class CGUIComboBoxGTK3 : public virtual CGUIComboBox, public CGUIContainerGTK3{
    protected:
        int DItemCount;
        GtkCellRenderer *DCellRenderer;
    public:
        CGUIComboBoxGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIComboBoxGTK3();

        virtual void SetFontFamily(const std::string &family);
        
        virtual int ItemCount() const;
        virtual std::string GetItem(int item) const;
        virtual void SetItem(int item, const std::string &str);
        virtual void AppendItem(const std::string &str);
        virtual void ClearItems();

        virtual int GetActiveItem() const;
        virtual void SetActiveItem(int item);
};

#endif