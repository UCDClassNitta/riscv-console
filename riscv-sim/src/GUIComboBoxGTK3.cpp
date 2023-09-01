#include "GUIComboBoxGTK3.h"

CGUIComboBoxGTK3::CGUIComboBoxGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){
    DItemCount = 0;
    gtk_combo_box_set_id_column(GTK_COMBO_BOX(DWidget),0);
    DCellRenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(DWidget), DCellRenderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(DWidget), DCellRenderer,
                                   "text", 0,
                                   NULL);

}

CGUIComboBoxGTK3::~CGUIComboBoxGTK3(){

}

void CGUIComboBoxGTK3::SetFontFamily(const std::string &family){
    g_object_set(DCellRenderer, "family", family.c_str(), NULL);
}

int CGUIComboBoxGTK3::ItemCount() const{
    return DItemCount;
}

std::string CGUIComboBoxGTK3::GetItem(int item) const{
    if((0 <= item)&&(item < DItemCount)){
        auto Model = gtk_combo_box_get_model(GTK_COMBO_BOX(DWidget));
        GtkTreeIter Iterator;
        if(gtk_tree_model_get_iter_from_string(Model,&Iterator,std::to_string(item).c_str())){
            gchar *StringData;
            gtk_tree_model_get(Model, &Iterator,0,&StringData,-1);
            return std::string((char *)StringData);
        }
    }
    return std::string();
}

void CGUIComboBoxGTK3::SetItem(int item, const std::string &str){
    if((0 <= item)&&(item < DItemCount)){
        auto Model = gtk_combo_box_get_model(GTK_COMBO_BOX(DWidget));
        GtkTreeIter Iterator;
        if(gtk_tree_model_get_iter_from_string(Model,&Iterator,std::to_string(item).c_str())){
            gtk_list_store_set(GTK_LIST_STORE(Model),&Iterator,0,str.c_str(),-1);
        }
    }
}

void CGUIComboBoxGTK3::AppendItem(const std::string &str){
    auto Model = gtk_combo_box_get_model(GTK_COMBO_BOX(DWidget));
    GtkTreeIter Iterator;
    gtk_list_store_append(GTK_LIST_STORE(Model),&Iterator);
    gtk_list_store_set(GTK_LIST_STORE(Model),&Iterator,0,str.c_str(),-1);
    DItemCount++;
}

void CGUIComboBoxGTK3::ClearItems(){
    auto Model = gtk_combo_box_get_model(GTK_COMBO_BOX(DWidget));
    gtk_list_store_clear(GTK_LIST_STORE(Model));
    DItemCount = 0;
}

int CGUIComboBoxGTK3::GetActiveItem() const{
    return gtk_combo_box_get_active(GTK_COMBO_BOX(DWidget));
}

void CGUIComboBoxGTK3::SetActiveItem(int item){
    if((0 <= item)&&(item < DItemCount)){
        gtk_combo_box_set_active(GTK_COMBO_BOX(DWidget),item);
    }
}