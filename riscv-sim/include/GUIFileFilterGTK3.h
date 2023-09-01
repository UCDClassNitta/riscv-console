#ifndef GUIFILEFILTERGTK3_H
#define GUIFILEFILTERGTK3_H

#include "GUIFileFilter.h"
#include <gtk/gtk.h>
#include <string>

class CGUIFileFilterGTK3 : public virtual CGUIFileFilter{
    friend class CGUIFileChooserDialogGTK3;
    protected:
        GtkFileFilter *DFilter;
        
    public:
        CGUIFileFilterGTK3(GtkFileFilter *filter, bool reference = false);
        virtual ~CGUIFileFilterGTK3();
                
        void AddPattern(const std::string &pattern) override;
};

#endif