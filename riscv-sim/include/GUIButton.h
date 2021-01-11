
#ifndef GUIBUTTON_H
#define GUIBUTTON_H
#include "GUIContainer.h"

class CGUIButton : public virtual CGUIContainer{
    public:
        
        virtual ~CGUIButton(){};
        
        virtual std::string GetLabel() const = 0;
        virtual void SetLabel(const std::string &label) = 0;
};

#endif

