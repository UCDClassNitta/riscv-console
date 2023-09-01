#include "GUIScrollBarGTK3.h"

CGUIScrollBarGTK3::CGUIScrollBarGTK3(GtkWidget *widget, bool reference) : CGUIWidgetGTK3(widget,reference){

}

CGUIScrollBarGTK3::~CGUIScrollBarGTK3(){
    
}

double CGUIScrollBarGTK3::GetValue(){
    return gtk_range_get_value(GTK_RANGE(DWidget));
}

double CGUIScrollBarGTK3::GetLower(){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    return gtk_adjustment_get_lower(Adjustment);
}

double CGUIScrollBarGTK3::GetUpper(){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    return gtk_adjustment_get_upper(Adjustment);
}

double CGUIScrollBarGTK3::GetStepIncrement(){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    return gtk_adjustment_get_step_increment(Adjustment);
}

double CGUIScrollBarGTK3::GetPageIncrement(){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    return gtk_adjustment_get_page_increment(Adjustment);
}

double CGUIScrollBarGTK3::GetPageSize(){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    return gtk_adjustment_get_page_size(Adjustment);
}

void CGUIScrollBarGTK3::SetValue(double val){
    gtk_range_set_value(GTK_RANGE(DWidget),val);
}

void CGUIScrollBarGTK3::SetLower(double lower){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    gtk_adjustment_set_lower(Adjustment, lower);
}

void CGUIScrollBarGTK3::SetUpper(double upper){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    gtk_adjustment_set_upper(Adjustment, upper);
}

void CGUIScrollBarGTK3::SetStepIncrement(double inc){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    gtk_adjustment_set_step_increment(Adjustment, inc);
}

void CGUIScrollBarGTK3::SetPageIncrement(double inc){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    gtk_adjustment_set_page_increment(Adjustment, inc);
}

void CGUIScrollBarGTK3::SetPageSize(double size){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    gtk_adjustment_set_page_size(Adjustment, size);
}