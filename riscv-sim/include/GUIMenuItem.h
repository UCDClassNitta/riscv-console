
#ifndef GUIMENUITEM_H
#define GUIMENUITEM_H
#include "GUIContainer.h"
#include "GUILabel.h"

class CGUIMenuItem : public virtual CGUIContainer{
    public:
        virtual ~CGUIMenuItem(){};
        
        virtual std::shared_ptr<CGUILabel> GetLabel() = 0;
        virtual void SetSubMenu(std::shared_ptr<CGUIWidget> widget) = 0;
};

#endif

