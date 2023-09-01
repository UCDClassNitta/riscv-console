#include "GUIDisplayGTK3.h"
#include "GUICursorGTK3.h"

CGUIDisplayGTK3::CGUIDisplayGTK3(GdkDisplay *display) : CGUIDisplay(){
    DDisplay = display;
}

CGUIDisplayGTK3::~CGUIDisplayGTK3(){

}

void CGUIDisplayGTK3::Sync(){
    gdk_display_sync(DDisplay);
}

void CGUIDisplayGTK3::Flush(){
    gdk_display_flush(DDisplay);
}

std::shared_ptr< CGUICursor > CGUIDisplayGTK3::NewCursor(CGUICursor::ECursorType type){
    GdkCursorType CursorType = GDK_BLANK_CURSOR;
    switch(type){
        case CGUICursor::ECursorType::RightPointer:     CursorType = GDK_RIGHT_PTR;
                                                        break;
        case CGUICursor::ECursorType::LeftPointer:      CursorType = GDK_LEFT_PTR;
                                                        break;
        case CGUICursor::ECursorType::CenterPointer:    CursorType = GDK_CENTER_PTR;
                                                        break;
        case CGUICursor::ECursorType::Crosshair:        CursorType = GDK_CROSSHAIR;
                                                        break;
        default:                                        break;
    }
    
    return std::make_shared<CGUICursorGTK3>(gdk_cursor_new_for_display(DDisplay, CursorType));
}