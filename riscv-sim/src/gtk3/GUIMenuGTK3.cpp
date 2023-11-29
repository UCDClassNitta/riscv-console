#include "GUIMenuGTK3.h"
#include "GUIOriginAnchorGTK3.h"

CGUIMenuGTK3::CGUIMenuGTK3(GtkWidget *widget, bool reference) : CGUIMenuShellGTK3(widget, reference){
    
}

CGUIMenuGTK3::~CGUIMenuGTK3(){
        
}

void CGUIMenuGTK3::PopupAtWidget(std::shared_ptr< CGUIWidget > widget, EGUIOriginAnchor widgetanchor, EGUIOriginAnchor menuanchor){
    std::shared_ptr<CGUIWidgetGTK3> Widget = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_menu_popup_at_widget(GTK_MENU(DWidget), 
                            Widget->Widget(), 
                            SGUIOriginAnchorGTK3::TranslateToGTK3Type(widgetanchor), 
                            SGUIOriginAnchorGTK3::TranslateToGTK3Type(menuanchor), 
                            NULL);
}

void CGUIMenuGTK3::PopupAtPointer(){
    gtk_menu_popup_at_pointer(GTK_MENU(DWidget), 
                            NULL);
}