#include "GUIGridGTK3.h"

CGUIGridGTK3::CGUIGridGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIGridGTK3::~CGUIGridGTK3(){

}

unsigned int CGUIGridGTK3::GetRowSpacing() const{
    return gtk_grid_get_row_spacing(GTK_GRID(DWidget));
}

void CGUIGridGTK3::SetRowSpacing(unsigned int spacing){
    gtk_grid_set_row_spacing(GTK_GRID(DWidget), spacing);
}

void CGUIGridGTK3::RemoveRow(int row){
    gtk_grid_remove_row(GTK_GRID(DWidget), row);
}

unsigned int CGUIGridGTK3::GetColumnSpacing() const{
    return gtk_grid_get_column_spacing(GTK_GRID(DWidget));
}

void CGUIGridGTK3::SetColumnSpacing(unsigned int spacing){
    gtk_grid_set_column_spacing(GTK_GRID(DWidget), spacing);
}

void CGUIGridGTK3::RemoveColumn(int col){
    gtk_grid_remove_column(GTK_GRID(DWidget), col);
}

void CGUIGridGTK3::Attach(std::shared_ptr<CGUIWidget> widget, int left, int top, int width, int height){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);

    gtk_grid_attach(GTK_GRID(DWidget), WidgetToAdd->Widget(), left, top, width, height);
    DChildren.insert(widget);
}
