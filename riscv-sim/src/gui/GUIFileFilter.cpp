#include "GUIFileFilterGTK3.h"

CGUIFileFilterGTK3::CGUIFileFilterGTK3(GtkFileFilter *filter, bool reference){
    DFilter = filter;
    //if(reference){
        g_object_ref(DFilter);   
    //}
}

CGUIFileFilterGTK3::~CGUIFileFilterGTK3(){
    g_object_unref(DFilter);    
}

void CGUIFileFilterGTK3::AddPattern(const std::string &pattern){
    gtk_file_filter_add_pattern(DFilter, pattern.c_str());
}