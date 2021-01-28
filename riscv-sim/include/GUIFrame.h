
#ifndef GUIFRAME_H
#define GUIFRAME_H
#include "GUIContainer.h"

class CGUIFrame : public virtual CGUIContainer{
    public:
        virtual ~CGUIFrame(){};
        
        virtual void SetLabel(const std::string &label) = 0;
};

#endif

