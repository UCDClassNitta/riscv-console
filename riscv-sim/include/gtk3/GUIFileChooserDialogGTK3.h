#ifndef GUIFILECHOOSERDIALOGGTK3_H
#define GUIFILECHOOSERDIALOGGTK3_H

#include "GUIFileChooserDialog.h"
#include "GUIFileFilterGTK3.h"
#include "GUIWindowGTK3.h"
#include <gtk/gtk.h>
#include <string>


class CGUIFileChooserDialogGTK3 : public virtual CGUIFileChooserDialog, public CGUIWindowGTK3{
    public:
        CGUIFileChooserDialogGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIFileChooserDialogGTK3();
        
        void SetFilter(std::shared_ptr<CGUIFileFilter> filter) override;
        std::string GetCurrentFolder() override;
        void SetCurrentFolder(const std::string &folder) override;
        std::string GetFilename() override;
        
        int Run() override;
};

#endif