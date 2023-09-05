#include "GUICursorGTK3.h"

CGUICursorGTK3::CGUICursorGTK3(GdkCursor *cursor) : CGUICursor(){
    DCursor = cursor;
}

CGUICursorGTK3::~CGUICursorGTK3(){
    
}

CGUICursor::ECursorType CGUICursorGTK3::Type(){
    switch(gdk_cursor_get_cursor_type(DCursor)){
        case GDK_CENTER_PTR:    return ECursorType::CenterPointer;
        case GDK_LEFT_PTR:      return ECursorType::LeftPointer;
        case GDK_RIGHT_PTR:     return ECursorType::RightPointer;
        case GDK_CROSSHAIR:     return ECursorType::Crosshair;
        default:                return ECursorType::Blank;
    }
}
