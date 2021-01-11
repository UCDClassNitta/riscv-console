
#ifndef GUIMENUSHELL_H
#define GUIMENUSHELL_H
#include "GUIContainer.h"

class CGUIMenuShell : public virtual CGUIContainer{
    public:
        virtual ~CGUIMenuShell(){};
        
        virtual void Append(std::shared_ptr<CGUIWidget> widget) = 0;
};

#endif

