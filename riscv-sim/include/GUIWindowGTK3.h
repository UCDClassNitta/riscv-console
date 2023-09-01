
#ifndef GUIWINDOWGTK3_H
#define GUIWINDOWGTK3_H
#include "GUIWindow.h"
#include "GUIContainerGTK3.h"

class CGUIWindowGTK3 : public virtual CGUIWindow, public CGUIContainerGTK3{
    public:
        CGUIWindowGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIWindowGTK3();
        
        void SetMinSize(int width, int height) override;
        void SetMaxSize(int width, int height) override;
        void SetMinSize(std::shared_ptr<CGUIWidget> widget, int width, int height) override;
        void SetMaxSize(std::shared_ptr<CGUIWidget> widget, int width, int height) override;
        void Resize(int width, int height) override;
        void SetTitle(const std::string &title) override;
        void Close() override;
        std::shared_ptr<CGraphicSurface> RenderToSurface() override;
};

#endif

