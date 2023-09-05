
#ifndef GUIBOX_H
#define GUIBOX_H
#include "GUIContainer.h"

class CGUIBox : public virtual CGUIContainer{
    public:
        enum class EOrientation{Horizontal, Vertical};
        virtual ~CGUIBox() = default;
        
        virtual void PackStart(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding) = 0;
        virtual void PackEnd(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding) = 0;
};

#endif

