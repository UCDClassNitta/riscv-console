
#ifndef GUIWINDOW_H
#define GUIWINDOW_H
#include "GUIContainer.h"

class CGUIWindow : public virtual CGUIContainer{
    public:
        virtual ~CGUIWindow() = default;
        
        virtual void GetPosition(int &x, int &y) = 0;
        virtual void Move(int x, int y) = 0;
        virtual void SetMinSize(int width, int height) = 0;
        virtual void SetMaxSize(int width, int height) = 0;
        virtual void SetMinSize(std::shared_ptr<CGUIWidget> widget, int width, int height) = 0;
        virtual void SetMaxSize(std::shared_ptr<CGUIWidget> widget, int width, int height) = 0;
        virtual void Resize(int width, int height) = 0;
        virtual void SetTitle(const std::string &title) = 0;
        virtual void Close() = 0;
        virtual std::shared_ptr<CGraphicSurface> RenderToSurface() = 0;
};

#endif

