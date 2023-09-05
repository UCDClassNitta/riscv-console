#include "GUIScrollWindowGTK3.h"

CGUIScrollWindowGTK3::CGUIScrollWindowGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){
    DHorizontalScrollBar = std::make_shared<CGUIScrollBarGTK3>(gtk_scrolled_window_get_hscrollbar(GTK_SCROLLED_WINDOW(widget)));
    DVerticalScrollBar = std::make_shared<CGUIScrollBarGTK3>(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(widget)));
}

CGUIScrollWindowGTK3::~CGUIScrollWindowGTK3(){

}

GtkPolicyType CGUIScrollWindowGTK3::TranslatePolicy(EScrollPolicyType policy){
    switch(policy){
        case CGUIScrollWindow::EScrollPolicyType::Always:       return GTK_POLICY_ALWAYS;
        case CGUIScrollWindow::EScrollPolicyType::Automatic:    return GTK_POLICY_AUTOMATIC;
        case CGUIScrollWindow::EScrollPolicyType::Never:        return GTK_POLICY_NEVER;
        case CGUIScrollWindow::EScrollPolicyType::External:     return GTK_POLICY_EXTERNAL;
    }
    return GTK_POLICY_ALWAYS;
}

void CGUIScrollWindowGTK3::SetScrollPolicy(EScrollPolicyType hpolicy, EScrollPolicyType vpolicy){
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(DWidget), TranslatePolicy(hpolicy),TranslatePolicy(vpolicy));
}

void CGUIScrollWindowGTK3::SetMinContentWidth(int width){
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(DWidget), width);
}

void CGUIScrollWindowGTK3::SetMinContentHeight(int height){
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(DWidget), height);
}

std::shared_ptr<CGUIScrollBar> CGUIScrollWindowGTK3::GetHorizontalScrollBar(){
    return DHorizontalScrollBar;
}

std::shared_ptr<CGUIScrollBar> CGUIScrollWindowGTK3::GetVerticalScrollBar(){
    return DVerticalScrollBar;
}