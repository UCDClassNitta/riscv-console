#include "GUILabelGTK3.h"

CGUILabelGTK3::CGUILabelGTK3(GtkWidget *widget, bool reference) : CGUIWidgetGTK3(widget, reference){
    
}

CGUILabelGTK3::~CGUILabelGTK3(){
    
}

std::string CGUILabelGTK3::GetText(){
    return gtk_label_get_text(GTK_LABEL(DWidget));
}

void CGUILabelGTK3::SetText(const std::string &str){
    gtk_label_set_text(GTK_LABEL(DWidget), str.c_str());
}

void CGUILabelGTK3::SetMarkup(const std::string &str){
    gtk_label_set_markup(GTK_LABEL(DWidget), str.c_str());
}

void CGUILabelGTK3::SetJustification(const SGUIJustificationType &justify){
    gtk_label_set_justify(GTK_LABEL(DWidget), GtkJustification(justify.DType));
    if(justify.DType == SGUIJustificationType::Right){
        gtk_label_set_xalign(GTK_LABEL(DWidget), 1.0);
    }
    else if(justify.DType == SGUIJustificationType::Center){
        gtk_label_set_xalign(GTK_LABEL(DWidget), 0.5);
    }
    else{
        gtk_label_set_xalign(GTK_LABEL(DWidget), 0.0);
    }
}

void CGUILabelGTK3::SetFontFamily(const std::string &family){
    auto AttrList = gtk_label_get_attributes(GTK_LABEL(DWidget));
    auto FontAttr = pango_attr_family_new(family.c_str());
    if(!AttrList){
        AttrList = pango_attr_list_new();    
        pango_attr_list_insert(AttrList,FontAttr);
    }
    else{
        pango_attr_list_ref(AttrList);
        pango_attr_list_change(AttrList,FontAttr);
    }
    gtk_label_set_attributes(GTK_LABEL(DWidget), AttrList);
    pango_attr_list_unref(AttrList);
    //pango_attribute_destroy(FontAttr); // destroying causes segfault must be handed over to list
    
}

void CGUILabelGTK3::SetBold(bool bold){
    auto AttrList = gtk_label_get_attributes(GTK_LABEL(DWidget));
    auto BoldAttr = pango_attr_weight_new(bold ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
    if(!AttrList){
        AttrList = pango_attr_list_new();    
        pango_attr_list_insert(AttrList,BoldAttr);
    }
    else{
        pango_attr_list_ref(AttrList);
        pango_attr_list_change(AttrList,BoldAttr);
    }
    gtk_label_set_attributes(GTK_LABEL(DWidget), AttrList);
    pango_attr_list_unref(AttrList);

}

void CGUILabelGTK3::SetWidthCharacters(int chars){
    gtk_label_set_width_chars(GTK_LABEL(DWidget), chars);
}

void CGUILabelGTK3::SetMaxWidthCharacters(int chars){
    gtk_label_set_max_width_chars(GTK_LABEL(DWidget), chars);
}
