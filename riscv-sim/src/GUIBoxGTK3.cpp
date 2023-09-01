#include "GUIBoxGTK3.h"

CGUIBoxGTK3::CGUIBoxGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIBoxGTK3::~CGUIBoxGTK3(){

}

void CGUIBoxGTK3::PackStart(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_box_pack_start(GTK_BOX(DWidget), WidgetToAdd->Widget(), expand, fill, padding);
    DChildren.insert(widget);
}

void CGUIBoxGTK3::PackEnd(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_box_pack_end(GTK_BOX(DWidget), WidgetToAdd->Widget(), expand, fill, padding);
    DChildren.insert(widget);
}