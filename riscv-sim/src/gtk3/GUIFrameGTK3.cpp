#include "GUIFrameGTK3.h"

CGUIFrameGTK3::CGUIFrameGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIFrameGTK3::~CGUIFrameGTK3(){

}

void CGUIFrameGTK3::SetLabel(const std::string &label){
    gtk_frame_set_label(GTK_FRAME(DWidget), label.c_str());
}