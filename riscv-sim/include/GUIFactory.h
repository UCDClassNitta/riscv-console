
#ifndef GUIFACTORY_H
#define GUIFACTORY_H
#include <memory>
#include <string>
#include "GUIApplication.h"
#include "GUIDisplay.h"
#include "GraphicFactory.h"
#include "GUIWindow.h"
#include "GUIDrawingArea.h"
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
#include "GUITextTag.h"
#include "GUITextIter.h"
#include "GUITextBuffer.h"
#include "GUITextView.h"
#include "GUIScrollBar.h"
#include "GUIScrollWindow.h"
#include "GUIFileChooserDialog.h"
#include "GUIFileFilter.h"

class CGUIFactory{         
    public:
        virtual ~CGUIFactory() = default;
        virtual std::shared_ptr<CGUIApplication> ApplicationInstance(const std::string &appname = "") = 0;
        virtual std::shared_ptr<CGUIDisplay> DefaultDisplay() = 0;
        virtual std::shared_ptr<CGraphicFactory> GraphicFactoryInstance() = 0;
        virtual std::shared_ptr<CGUIDrawingArea> NewDrawingArea() = 0;
        virtual std::shared_ptr<CGUIBox> NewBox(CGUIBox::EOrientation orientation, int spacing) = 0;
        virtual std::shared_ptr<CGUIEventBox> NewEventBox() = 0;
        virtual std::shared_ptr<CGUIGrid> NewGrid() = 0;
        virtual std::shared_ptr<CGUIFrame> NewFrame() = 0;
        virtual std::shared_ptr<CGUIButton> NewButton() = 0;
        virtual std::shared_ptr<CGUIComboBox> NewComboBox() = 0;
        virtual std::shared_ptr<CGUILabel> NewLabel(const std::string &text) = 0;
        virtual std::shared_ptr<CGUIToggleButton> NewToggleButton() = 0;
        virtual std::shared_ptr<CGUIMenu> NewMenu() = 0;
        virtual std::shared_ptr<CGUIMenuBar> NewMenuBar() = 0;
        virtual std::shared_ptr<CGUIMenuItem> NewMenuItem(const std::string &label) = 0;
        virtual std::shared_ptr<CGUITextView> NewTextView() = 0;
        virtual std::shared_ptr<CGUIScrollBar> NewScrollBar(CGUIScrollBar::EOrientation orientation) = 0; 
        virtual std::shared_ptr<CGUIScrollWindow> NewScrollWindow() = 0;
        virtual std::shared_ptr<CGUIFileFilter> NewFileFilter() = 0;
        virtual std::shared_ptr<CGUIFileChooserDialog> NewFileChooserDialog(const std::string &title, bool openfile, std::shared_ptr<CGUIWindow> parent) = 0;

};

#endif

