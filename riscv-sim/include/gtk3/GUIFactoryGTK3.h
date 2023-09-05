#ifndef GUIFACTORYGTK3_H
#define GUIFACTORYGTK3_H

#include "GUIFactory.h"
#include "PeriodicTimeout.h"
#include <gtk/gtk.h>
#include <set>

class CGUIFactoryGTK3 : public CGUIFactory{        
    private:
        struct SPrivateConstructorKey{};
        static std::shared_ptr<CGUIApplication> DApplicationInstance;
        static std::shared_ptr<CGUIFactory> DInstance;

    public:
        explicit CGUIFactoryGTK3(const SPrivateConstructorKey &key);
        virtual ~CGUIFactoryGTK3();

        static std::shared_ptr<CGUIFactory> Instance();

        virtual std::shared_ptr<CGUIApplication> ApplicationInstance(const std::string &appname = "");
        virtual std::shared_ptr<CGUIDisplay> DefaultDisplay();
        virtual std::shared_ptr<CGraphicFactory> GraphicFactoryInstance();
        virtual std::shared_ptr<CGUIDrawingArea> NewDrawingArea();
        virtual std::shared_ptr<CGUIBox> NewBox(CGUIBox::EOrientation orientation, int spacing);
        virtual std::shared_ptr<CGUIEventBox> NewEventBox();
        virtual std::shared_ptr<CGUIGrid> NewGrid();
        virtual std::shared_ptr<CGUIFrame> NewFrame();
        virtual std::shared_ptr<CGUIButton> NewButton();
        virtual std::shared_ptr<CGUIComboBox> NewComboBox();
        virtual std::shared_ptr<CGUILabel> NewLabel(const std::string &text);
        virtual std::shared_ptr<CGUIToggleButton> NewToggleButton();
        virtual std::shared_ptr<CGUIMenu> NewMenu();
        virtual std::shared_ptr<CGUIMenuBar> NewMenuBar();
        virtual std::shared_ptr<CGUIMenuItem> NewMenuItem(const std::string &label);
        virtual std::shared_ptr<CGUITextView> NewTextView();
        virtual std::shared_ptr<CGUIScrollBar> NewScrollBar(CGUIScrollBar::EOrientation orientation);
        virtual std::shared_ptr<CGUIScrollWindow> NewScrollWindow();
        virtual std::shared_ptr<CGUIFileFilter> NewFileFilter();
        virtual std::shared_ptr<CGUIFileChooserDialog> NewFileChooserDialog(const std::string &title, bool openfile, std::shared_ptr<CGUIWindow> parent);

};

#endif

