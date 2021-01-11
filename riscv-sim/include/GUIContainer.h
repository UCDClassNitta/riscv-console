
#ifndef GUICONTAINER_H
#define GUICONTAINER_H
#include "GUIWidget.h"

class CGUIContainer : public virtual CGUIWidget{
    public:
        virtual ~CGUIContainer(){};
        
        virtual void SetBorderWidth(int width) = 0;
        virtual void Add(std::shared_ptr<CGUIWidget> widget) = 0;
        virtual void Remove(std::shared_ptr<CGUIWidget> widget) = 0;
};

#endif

