#include "GUIContainerGTK3.h"

CGUIContainerGTK3::CGUIContainerGTK3(GtkWidget *widget, bool reference) : CGUIWidgetGTK3(widget, reference){
    
}

CGUIContainerGTK3::~CGUIContainerGTK3(){
    
}

void CGUIContainerGTK3::SetBorderWidth(int width){
    gtk_container_set_border_width(GTK_CONTAINER(DWidget), width);
}

void CGUIContainerGTK3::Add(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_container_add(GTK_CONTAINER(DWidget), WidgetToAdd->Widget());
    DChildren.insert(widget);
}

void CGUIContainerGTK3::Remove(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    gtk_container_remove(GTK_CONTAINER(DWidget), WidgetToAdd->Widget());    
    DChildren.erase(widget);
}
