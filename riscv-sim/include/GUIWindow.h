
#ifndef GUIWINDOW_H
#define GUIWINDOW_H
#include "GUIContainer.h"

class CGUIWindow : public virtual CGUIContainer{
    public:
        virtual ~CGUIWindow(){};
        
        virtual void SetMinSize(int width, int height) = 0;
        virtual void SetMaxSize(int width, int height) = 0;
        virtual void SetMinSize(std::shared_ptr<CGUIWidget> widget, int width, int height) = 0;
        virtual void SetMaxSize(std::shared_ptr<CGUIWidget> widget, int width, int height) = 0;
        virtual void Resize(int width, int height) = 0;
        virtual void SetTitle(const std::string &title) = 0;
        virtual void Close() = 0;
};

#endif

