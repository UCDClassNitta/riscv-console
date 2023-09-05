#include "GUIFileChooserDialogGTK3.h"

CGUIFileChooserDialogGTK3::CGUIFileChooserDialogGTK3(GtkWidget *widget, bool reference) : CGUIWindowGTK3(widget, reference){
    
}

CGUIFileChooserDialogGTK3::~CGUIFileChooserDialogGTK3(){

}

void CGUIFileChooserDialogGTK3::SetFilter(std::shared_ptr<CGUIFileFilter> filter){
    std::shared_ptr<CGUIFileFilterGTK3> FilterToAdd = std::dynamic_pointer_cast<CGUIFileFilterGTK3>(filter);
    
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(DWidget), FilterToAdd->DFilter);
}

std::string CGUIFileChooserDialogGTK3::GetCurrentFolder(){
    return std::string((char *)gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(DWidget)));
}

void CGUIFileChooserDialogGTK3::SetCurrentFolder(const std::string &folder){
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(DWidget), folder.c_str());    
}

std::string CGUIFileChooserDialogGTK3::GetFilename(){
    gchar *Filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(DWidget));
    std::string ReturnName((char *)Filename);
    g_free(Filename);
    return ReturnName;
}

int CGUIFileChooserDialogGTK3::Run(){
    return GTK_RESPONSE_CANCEL == gtk_dialog_run(GTK_DIALOG(DWidget)) ? 0 : 1;
}