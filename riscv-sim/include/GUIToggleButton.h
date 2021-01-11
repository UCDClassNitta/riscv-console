
#ifndef GUITOGGLEBUTTON_H
#define GUITOGGLEBUTTON_H
#include "GUIButton.h"

class CGUIToggleButton : public virtual CGUIButton{
    public:
        
        virtual ~CGUIToggleButton(){};
        
        virtual bool GetActive() const = 0;
        virtual void SetActive(bool active) = 0;
};

#endif

