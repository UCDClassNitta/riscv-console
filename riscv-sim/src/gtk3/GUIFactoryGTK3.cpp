
#include "GUIFactoryGTK3.h"
#include "GraphicFactoryCairo.h"
#include "GUIApplicationGTK3.h"
#include "GUIDisplayGTK3.h"
#include "GUIDrawingAreaGTK3.h"
#include "GUIBoxGTK3.h"
#include "GUIButtonGTK3.h"
#include "GUIComboBoxGTK3.h"
#include "GUIContainerGTK3.h"
#include "GUICursorGTK3.h"
#include "GUIEventBoxGTK3.h"
#include "GUIFileChooserDialogGTK3.h"
#include "GUIFileFilterGTK3.h"
#include "GUIFrameGTK3.h"
#include "GUIGridGTK3.h"
#include "GUILabelGTK3.h"
#include "GUIMenuGTK3.h"
#include "GUIMenuBarGTK3.h"
#include "GUIMenuItemGTK3.h"
#include "GUIMenuShellGTK3.h"
#include "GUIScrollBarGTK3.h"
#include "GUIScrollWindowGTK3.h"
#include "GUITextBufferGTK3.h"
#include "GUITextIterGTK3.h"
#include "GUITextTagGTK3.h"
#include "GUITextViewGTK3.h"
#include "GUIToggleButtonGTK3.h"
#include "GUIWindowGTK3.h"
#include <glib.h>
#include <glib/gi18n.h>


std::shared_ptr<CGUIApplication> CGUIFactoryGTK3::DApplicationInstance;
std::shared_ptr<CGUIFactory> CGUIFactoryGTK3::DInstance;

CGUIFactoryGTK3::CGUIFactoryGTK3(const SPrivateConstructorKey &key){

}

CGUIFactoryGTK3::~CGUIFactoryGTK3(){

}

std::shared_ptr<CGUIFactory> CGUIFactoryGTK3::Instance(){
    if(!DInstance){
        DInstance = std::make_shared<CGUIFactoryGTK3>(SPrivateConstructorKey{});
    }
    return DInstance;
}

std::shared_ptr<CGUIApplication> CGUIFactoryGTK3::ApplicationInstance(const std::string &appname){
    if(!DApplicationInstance && appname.length()){
        DApplicationInstance = std::make_shared<CGUIApplicationGTK3>(appname);
    }
    return DApplicationInstance;
}

std::shared_ptr<CGUIDisplay> CGUIFactoryGTK3::DefaultDisplay(){
    return std::make_shared<CGUIDisplayGTK3>(gdk_display_get_default());   
}

std::shared_ptr<CGraphicFactory> CGUIFactoryGTK3::GraphicFactoryInstance(){
    return CGraphicFactoryCairo::Instance();
}

std::shared_ptr<CGUIDrawingArea> CGUIFactoryGTK3::NewDrawingArea(){
    return std::make_shared<CGUIDrawingAreaGTK3>(gtk_drawing_area_new());
}

std::shared_ptr<CGUIBox> CGUIFactoryGTK3::NewBox(CGUIBox::EOrientation orientation, int spacing){
    return std::make_shared<CGUIBoxGTK3>(gtk_box_new(orientation == CGUIBox::EOrientation::Horizontal ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL, spacing));
}

std::shared_ptr<CGUIEventBox> CGUIFactoryGTK3::NewEventBox(){
    return std::make_shared<CGUIEventBoxGTK3>(gtk_event_box_new());
}

std::shared_ptr<CGUIGrid> CGUIFactoryGTK3::NewGrid(){
    return std::make_shared<CGUIGridGTK3>(gtk_grid_new());
}

std::shared_ptr<CGUIFrame> CGUIFactoryGTK3::NewFrame(){
    return std::make_shared<CGUIFrameGTK3>(gtk_frame_new(NULL));
}

std::shared_ptr<CGUIButton> CGUIFactoryGTK3::NewButton(){
    return std::make_shared<CGUIButtonGTK3>(gtk_button_new());
}

std::shared_ptr<CGUIComboBox> CGUIFactoryGTK3::NewComboBox(){
    auto Model = gtk_list_store_new(1,G_TYPE_STRING);
    auto ComboBox = gtk_combo_box_new_with_model(GTK_TREE_MODEL(Model));
    
    return std::make_shared<CGUIComboBoxGTK3>(ComboBox);
}

std::shared_ptr<CGUILabel> CGUIFactoryGTK3::NewLabel(const std::string &text){
    return std::make_shared<CGUILabelGTK3>(gtk_label_new(text.c_str()));
}

std::shared_ptr<CGUIToggleButton> CGUIFactoryGTK3::NewToggleButton(){
    return std::make_shared<CGUIToggleButtonGTK3>(gtk_toggle_button_new());
}

std::shared_ptr<CGUIMenu> CGUIFactoryGTK3::NewMenu(){
    return std::make_shared<CGUIMenuGTK3>(gtk_menu_new());
}

std::shared_ptr<CGUIMenuBar> CGUIFactoryGTK3::NewMenuBar(){
    return std::make_shared<CGUIMenuBarGTK3>(gtk_menu_bar_new());
}

std::shared_ptr<CGUIMenuItem> CGUIFactoryGTK3::NewMenuItem(const std::string &label){
    return std::make_shared<CGUIMenuItemGTK3>(gtk_menu_item_new_with_label(label.c_str()));
}

std::shared_ptr<CGUITextView> CGUIFactoryGTK3::NewTextView(){
    return std::make_shared<CGUITextViewGTK3>(gtk_text_view_new());
}

std::shared_ptr<CGUIScrollBar> CGUIFactoryGTK3::NewScrollBar(CGUIScrollBar::EOrientation orientation){
    return std::make_shared<CGUIScrollBarGTK3>(gtk_scrollbar_new(orientation == CGUIScrollBar::EOrientation::Horizontal ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL, NULL));
}

std::shared_ptr<CGUIScrollWindow> CGUIFactoryGTK3::NewScrollWindow(){
    return std::make_shared<CGUIScrollWindowGTK3>(gtk_scrolled_window_new(NULL, NULL));
}

std::shared_ptr<CGUIFileFilter> CGUIFactoryGTK3::NewFileFilter(){
    return std::make_shared<CGUIFileFilterGTK3>(gtk_file_filter_new());
}

std::shared_ptr<CGUIFileChooserDialog> CGUIFactoryGTK3::NewFileChooserDialog(const std::string &title, bool openfile, std::shared_ptr<CGUIWindow> parent){
    GtkWindow *ParentWidget = NULL;
    if(parent){
        std::shared_ptr<CGUIWindowGTK3> ParentWindow = std::dynamic_pointer_cast<CGUIWindowGTK3>(parent);
    
        ParentWidget = GTK_WINDOW(ParentWindow->Widget());
    }   
    if(openfile){
        return std::make_shared<CGUIFileChooserDialogGTK3>(gtk_file_chooser_dialog_new(title.c_str(), ParentWidget, GTK_FILE_CHOOSER_ACTION_OPEN, _("_Cancel"), GTK_RESPONSE_CANCEL, _("_Open"), GTK_RESPONSE_ACCEPT, NULL));
    }
    else{
        return std::make_shared<CGUIFileChooserDialogGTK3>(gtk_file_chooser_dialog_new(title.c_str(), ParentWidget, GTK_FILE_CHOOSER_ACTION_SAVE, _("_Cancel"), GTK_RESPONSE_CANCEL, _("_Save"), GTK_RESPONSE_ACCEPT, NULL));
    }
}
