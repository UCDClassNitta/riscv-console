#include "GUITextBufferGTK3.h"

CGUITextBufferGTK3::CGUITextBufferGTK3(GtkTextBuffer *textbuffer){
    DTextBuffer = textbuffer;
}

CGUITextBufferGTK3::~CGUITextBufferGTK3(){
    
}

void CGUITextBufferGTK3::SetText(const std::string &text){
    gtk_text_buffer_set_text(DTextBuffer, text.c_str(), -1);
}

void CGUITextBufferGTK3::Insert(std::shared_ptr<CGUITextIter> start, const std::string &text){
    auto IterStart = std::dynamic_pointer_cast<CGUITextIterGTK3>(start);
    gtk_text_buffer_insert(DTextBuffer, &IterStart->DIter, text.c_str(), text.length());
}

void CGUITextBufferGTK3::Delete(std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end){
    auto IterStart = std::dynamic_pointer_cast<CGUITextIterGTK3>(start);
    auto IterEnd = std::dynamic_pointer_cast<CGUITextIterGTK3>(end);
    gtk_text_buffer_delete(DTextBuffer, &IterStart->DIter, &IterEnd->DIter);
}

std::shared_ptr<CGUITextTag> CGUITextBufferGTK3::CreateTag(const std::vector< std::pair<std::string, std::string> > &properties){
    GtkTextTag *NewTextTag = gtk_text_buffer_create_tag(DTextBuffer, NULL, NULL, NULL);
    for(auto &NameValue : properties){
        GValue TempValue = G_VALUE_INIT;
        g_object_get_property(G_OBJECT(NewTextTag), NameValue.first.c_str(), &TempValue);
        CGUITextTagGTK3::TransformValue(&TempValue, NameValue.second);
        g_object_set_property(G_OBJECT(NewTextTag), NameValue.first.c_str(), &TempValue);
    }
    return std::make_shared<CGUITextTagGTK3>(NewTextTag);
}

void CGUITextBufferGTK3::ApplyTag(std::shared_ptr<CGUITextTag> tag, std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end){
    auto TextTag = std::dynamic_pointer_cast<CGUITextTagGTK3>(tag);
    auto IterStart = std::dynamic_pointer_cast<CGUITextIterGTK3>(start);
    auto IterEnd = std::dynamic_pointer_cast<CGUITextIterGTK3>(end);
    gtk_text_buffer_apply_tag(DTextBuffer, TextTag->DTextTag, &IterStart->DIter, &IterEnd->DIter);
}

void CGUITextBufferGTK3::RemoveTag(std::shared_ptr<CGUITextTag> tag, std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end){
    auto TextTag = std::dynamic_pointer_cast<CGUITextTagGTK3>(tag);
    auto IterStart = std::dynamic_pointer_cast<CGUITextIterGTK3>(start);
    auto IterEnd = std::dynamic_pointer_cast<CGUITextIterGTK3>(end);
    gtk_text_buffer_remove_tag(DTextBuffer, TextTag->DTextTag, &IterStart->DIter, &IterEnd->DIter);
}

std::shared_ptr<CGUITextIter> CGUITextBufferGTK3::GetLineIter(int linenum){
    auto NewIter = std::make_shared<CGUITextIterGTK3>();
    
    gtk_text_buffer_get_iter_at_line(DTextBuffer, &NewIter->DIter, linenum);
    return NewIter;
}

std::shared_ptr<CGUITextIter> CGUITextBufferGTK3::GetStartIter(){
    auto NewIter = std::make_shared<CGUITextIterGTK3>();
    
    gtk_text_buffer_get_start_iter(DTextBuffer,&NewIter->DIter);
    return NewIter;
}

std::shared_ptr<CGUITextIter> CGUITextBufferGTK3::GetEndIter(){
    auto NewIter = std::make_shared<CGUITextIterGTK3>();
    
    gtk_text_buffer_get_end_iter(DTextBuffer,&NewIter->DIter);
    return NewIter;
}

std::string CGUITextBufferGTK3::GetText(std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end){
    auto IterStart = std::dynamic_pointer_cast<CGUITextIterGTK3>(start);
    auto IterEnd = std::dynamic_pointer_cast<CGUITextIterGTK3>(end);
    auto TextPtr = gtk_text_buffer_get_text(DTextBuffer,&IterStart->DIter,&IterEnd->DIter,true);
    std::string ReturnString = TextPtr;
    g_free(TextPtr);
    return ReturnString;
}

int CGUITextBufferGTK3::GetLineCount(){
    return gtk_text_buffer_get_line_count(DTextBuffer);
}
