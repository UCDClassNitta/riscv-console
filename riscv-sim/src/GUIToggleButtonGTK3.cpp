#include "GUIToggleButtonGTK3.h"

CGUIToggleButtonGTK3::CGUIToggleButtonGTK3(GtkWidget *widget, bool reference) : CGUIButtonGTK3(widget, reference){
    
}

CGUIToggleButtonGTK3::~CGUIToggleButtonGTK3(){

}

bool CGUIToggleButtonGTK3::GetActive() const{
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(DWidget));
}

void CGUIToggleButtonGTK3::SetActive(bool active){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(DWidget), active ? TRUE : FALSE);
}