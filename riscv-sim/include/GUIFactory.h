
#ifndef GUIFACTORY_H
#define GUIFACTORY_H
#include <memory>
#include <string>
#include "GUIApplication.h"
#include "GUIDisplay.h"
#include "GUIMenu.h"
#include "GUIBox.h"
#include "GUIEventBox.h"
#include "GUIFrame.h"
#include "GUIButton.h"
#include "GUIComboBox.h"
#include "GUILabel.h"
#include "GUIToggleButton.h"
#include "GUIGrid.h"
#include "GUIMenuItem.h"
#include "GUIMenuBar.h"
#include "GUIScrollBar.h"
#include "GUIWindow.h"
#include "GUIFileChooserDialog.h"
#include "GUIDrawingArea.h"

class CGUIFactory{     
    protected:
        static std::shared_ptr<CGUIApplication> DApplicationPointer;
    
    public:
        static std::shared_ptr<CGUIApplication> ApplicationInstance(const std::string &appname = "");
        static std::shared_ptr<CGUIDisplay> DefaultDisplay();
        static std::shared_ptr<CGUIDrawingArea> NewDrawingArea();
        static std::shared_ptr<CGUIBox> NewBox(CGUIBox::EOrientation orientation, int spacing);
        static std::shared_ptr<CGUIEventBox> NewEventBox();
        static std::shared_ptr<CGUIGrid> NewGrid();
        static std::shared_ptr<CGUIFrame> NewFrame();
        static std::shared_ptr<CGUIButton> NewButton();
        static std::shared_ptr<CGUIComboBox> NewComboBox();
        static std::shared_ptr<CGUILabel> NewLabel(const std::string &text);
        static std::shared_ptr<CGUIToggleButton> NewToggleButton();
        static std::shared_ptr<CGUIMenu> NewMenu();
        static std::shared_ptr<CGUIMenuBar> NewMenuBar();
        static std::shared_ptr<CGUIMenuItem> NewMenuItem(const std::string &label);
        static std::shared_ptr<CGUIScrollBar> NewScrollBar(CGUIScrollBar::EOrientation orientation);
        static std::shared_ptr<CGUIFileFilter> NewFileFilter();
        static std::shared_ptr<CGUIFileChooserDialog> NewFileChooserDialog(const std::string &title, bool openfile, std::shared_ptr<CGUIWindow> parent);
};

#endif

