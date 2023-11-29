
#ifndef GUIMENU_H
#define GUIMENU_H
#include "GUIMenuShell.h"
#include "GUIOriginAnchor.h"

class CGUIMenu : public virtual CGUIMenuShell{
    public:
        virtual ~CGUIMenu() = default;

        virtual void PopupAtWidget(std::shared_ptr< CGUIWidget > widget, EGUIOriginAnchor widgetanchor, EGUIOriginAnchor menuanchor) = 0;
        virtual void PopupAtPointer() = 0;
};

#endif

