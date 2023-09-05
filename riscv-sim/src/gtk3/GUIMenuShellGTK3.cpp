#include "GUIMenuShellGTK3.h"

CGUIMenuShellGTK3::CGUIMenuShellGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){
    
}

CGUIMenuShellGTK3::~CGUIMenuShellGTK3(){
    
}

void CGUIMenuShellGTK3::Append(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(DWidget), WidgetToAdd->Widget());
}