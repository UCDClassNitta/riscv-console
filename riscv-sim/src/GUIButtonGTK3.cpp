#include "GUIButtonGTK3.h"

CGUIButtonGTK3::CGUIButtonGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIButtonGTK3::~CGUIButtonGTK3(){

}

std::string CGUIButtonGTK3::GetLabel() const{
    if(auto Label = gtk_button_get_label(GTK_BUTTON(DWidget))){
        return std::string((const char *)Label);
    }
    return std::string();
}

void CGUIButtonGTK3::SetLabel(const std::string &label){
    gtk_button_set_label(GTK_BUTTON(DWidget),(const gchar *)label.c_str());
}
