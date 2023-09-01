#include "GUITextIterGTK3.h"

CGUITextIterGTK3::CGUITextIterGTK3(){

}

CGUITextIterGTK3::~CGUITextIterGTK3(){
    
}

bool CGUITextIterGTK3::MoveBackwardLines(int count){
    return gtk_text_iter_backward_lines(&DIter, count);
}

bool CGUITextIterGTK3::MoveForwardLines(int count){
    return gtk_text_iter_forward_lines(&DIter, count);
}

bool CGUITextIterGTK3::MoveBackwardChars(int count){
    return gtk_text_iter_backward_chars(&DIter, count);
}

bool CGUITextIterGTK3::MoveForwardChars(int count){
    return gtk_text_iter_forward_chars(&DIter, count);
}
