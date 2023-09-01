#include "GUIMenuItemGTK3.h"

CGUIMenuItemGTK3::CGUIMenuItemGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){
    
}

CGUIMenuItemGTK3::~CGUIMenuItemGTK3(){
    
}

std::shared_ptr<CGUILabel> CGUIMenuItemGTK3::GetLabel(){
    GList *List = gtk_container_get_children(GTK_CONTAINER(DWidget));
    
    return std::make_shared<CGUILabelGTK3>(GTK_WIDGET(List->data), true);
}

void CGUIMenuItemGTK3::SetSubMenu(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(DWidget), WidgetToAdd->Widget());
}