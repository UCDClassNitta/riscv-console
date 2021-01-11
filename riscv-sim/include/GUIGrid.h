
#ifndef GUIGRID_H
#define GUIGRID_H
#include "GUIContainer.h"

class CGUIGrid : public virtual CGUIContainer{
    public:
        virtual ~CGUIGrid(){};
        
        virtual unsigned int GetRowSpacing() const = 0;
        virtual void SetRowSpacing(unsigned int spacing) = 0;
        virtual unsigned int GetColumnSpacing() const = 0;
        virtual void SetColumnSpacing(unsigned int spacing) = 0;
        virtual void Attach(std::shared_ptr<CGUIWidget> widget, int left, int top, int width, int height) = 0;
};

#endif

