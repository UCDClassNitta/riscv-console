
#ifndef GUICONTAINERGTK3_H
#define GUICONTAINERGTK3_H
#include "GUIContainer.h"
#include "GUIWidgetGTK3.h"
#include <set>

class CGUIContainerGTK3 : public virtual CGUIContainer, public CGUIWidgetGTK3{
    protected:
        std::set< std::shared_ptr<CGUIWidget> > DChildren;
    public:
        CGUIContainerGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIContainerGTK3();
        
        void SetBorderWidth(int width) override;
        void Add(std::shared_ptr<CGUIWidget> widget) override;
        void Remove(std::shared_ptr<CGUIWidget> widget) override;
};
#endif

