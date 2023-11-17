#include "GUIScrollableTextViewLineBox.h"
#include "GUIFactory.h"
#include <sstream>
#include <iterator>

CGUIScrollableTextViewLineBox::CGUIScrollableTextViewLineBox(std::shared_ptr<CGUIFactory> guifactory, size_t initsize){
    DGUIFactory = guifactory;
    DTextView = DGUIFactory->NewTextView();
    DContainingFrame = DGUIFactory->NewFrame();
    DScrollWindow = DGUIFactory->NewScrollWindow();
    DScrollWindow->Add(DTextView);
    DContainingFrame->Add(DScrollWindow);

    DTextView->SetWrapMode(CGUITextView::EWrapModeType::None);
    DTextView->SetMonospace(true);
    DTextView->SetCursorVisible(false);
    
    DHighlightTag = DTextView->GetBuffer()->CreateTag({{"weight", "700"}}); // PANGO_WEIGHT_BOLD == 700
    DHightlightedBufferedLine = 0;
    DMinLines = initsize;
    DFontFamily = "monospace";
    DWidthCharacters = -1;
    DMaxWidthCharacters = -1;
    DButtonPressCalldata = nullptr;
    DButtonPressCallback = nullptr;

    DButtonReleaseCalldata = nullptr;
    DButtonReleaseCallback = nullptr;

    DScrollCalldata = nullptr;
    DScrollCallback = nullptr;
    DScrollWindow->SetMinContentWidth(DTextView->GetRequiredCharactersWidth(DWidthCharacters));
    DScrollWindow->SetMinContentHeight(DTextView->GetRequiredLinesHeight(DMinLines));
    DTextView->SetButtonPressEventCallback(this,WidgetButtonEventCallback);
    DTextView->SetButtonReleaseEventCallback(this,WidgetButtonEventCallback);
    DScrollWindow->GetVerticalScrollBar()->SetValueChangedEventCallback(this,ScrollBarChangedEventCallback);
}

CGUIScrollableTextViewLineBox::~CGUIScrollableTextViewLineBox(){

}


bool CGUIScrollableTextViewLineBox::ScrollBarChangedEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data){
    CGUIScrollableTextViewLineBox *LineBox = static_cast<CGUIScrollableTextViewLineBox *>(data);
    return LineBox->ScrollBarChangedEvent(widget);
}


bool CGUIScrollableTextViewLineBox::WidgetScrollEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIScrollEvent &event, TGUICalldata data){
    CGUIScrollableTextViewLineBox *LineBox = static_cast<CGUIScrollableTextViewLineBox *>(data);
    return LineBox->WidgetScrollEvent(widget, event);
}


bool CGUIScrollableTextViewLineBox::WidgetButtonEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CGUIScrollableTextViewLineBox *LineBox = static_cast<CGUIScrollableTextViewLineBox *>(data);
    auto VerticalScrollBar = LineBox->DScrollWindow->GetVerticalScrollBar();
    int LineNumber = LineBox->DTextView->GetLineNumberAtY(event.DWindowY + VerticalScrollBar->GetValue());
    if(event.DType.IsButtonRelease()){
        if(LineBox->DButtonReleaseCallback){
            if(!LineBox->DButtonReleaseCallback(LineBox->shared_from_this(), event, LineNumber, LineBox->DButtonReleaseCalldata)){
                LineBox->DButtonReleaseCallback = nullptr;
                LineBox->DButtonReleaseCalldata = nullptr;
            }
        }
    }
    else{
        if(LineBox->DButtonPressCallback){
            if(!LineBox->DButtonPressCallback(LineBox->shared_from_this(), event, LineNumber, LineBox->DButtonPressCalldata)){
                LineBox->DButtonPressCallback = nullptr;
                LineBox->DButtonPressCalldata = nullptr;
            }
        }
    }
    return true;
}

bool CGUIScrollableTextViewLineBox::ScrollBarChangedEvent(std::shared_ptr<CGUIWidget> widget){
    if(DScrollCallback){
        if(!DScrollCallback(shared_from_this(),DScrollCalldata)){
            DScrollCallback = nullptr;
            DScrollCalldata = nullptr;
        }
    }
    
    return true;
}

bool CGUIScrollableTextViewLineBox::WidgetScrollEvent(std::shared_ptr<CGUIWidget> widget, SGUIScrollEvent &event){
    if(DScrollCallback){
        if(!DScrollCallback(shared_from_this(),DScrollCalldata)){
            DScrollCallback = nullptr;
            DScrollCalldata = nullptr;
        }
    }
    return true;
}

std::shared_ptr<CGUIWidget> CGUIScrollableTextViewLineBox::ContainingWidget() const{
    return DContainingFrame;
}

void CGUIScrollableTextViewLineBox::SetVerticalExpand(bool exp){
    DContainingFrame->SetVerticalExpand(exp);
}

void CGUIScrollableTextViewLineBox::SetHorizontalExpand(bool exp){
    DContainingFrame->SetHorizontalExpand(exp);
}

void CGUIScrollableTextViewLineBox::SetCursor(std::shared_ptr<CGUICursor> cursor){
    DTextView->SetCursor(cursor);
    DContainingFrame->SetCursor(cursor);
}

size_t CGUIScrollableTextViewLineBox::GetBaseLine() const{
    auto VerticalScrollBar = DScrollWindow->GetVerticalScrollBar();
    return DTextView->GetLineNumberAtY(VerticalScrollBar->GetValue());
}

void CGUIScrollableTextViewLineBox::SetBaseLine(size_t line){
    auto LineCount = GetBufferedLineCount();
    if(line < LineCount){
        auto VerticalScrollBar = DScrollWindow->GetVerticalScrollBar();
        VerticalScrollBar->SetValue((VerticalScrollBar->GetUpper() - VerticalScrollBar->GetLower()) * line / LineCount);
        DTextView->PlaceCursorOnscreen();
        //DScrollbar->SetValue(line);
    }
}

size_t CGUIScrollableTextViewLineBox::GetLineCount() const{
    auto LineCount = GetBufferedLineCount();
    
    if(DMinLines < LineCount){
        auto VerticalScrollBar = DScrollWindow->GetVerticalScrollBar();
        return DScrollWindow->AllocatedHeight() * LineCount / (VerticalScrollBar->GetUpper() - VerticalScrollBar->GetLower());
    }
    return DMinLines;
}

void CGUIScrollableTextViewLineBox::SetLineCount(size_t count){
    DMinLines = count;
    DScrollWindow->SetMinContentHeight(DTextView->GetRequiredLinesHeight(DMinLines));
}

size_t CGUIScrollableTextViewLineBox::GetBufferedLineCount() const{
    return DTextView->GetBuffer()->GetLineCount();
}

std::string CGUIScrollableTextViewLineBox::GetBufferedLine(size_t index) const{
    auto TextBuffer = DTextView->GetBuffer();
    auto LineStart = TextBuffer->GetLineIter(index);
    auto LineEnd = TextBuffer->GetLineIter(index+1);
    if(index + 1 < GetBufferedLineCount()){
        LineEnd->MoveBackwardChars(1); // Remove \n
    }
    return TextBuffer->GetText(LineStart,LineEnd);
}

void CGUIScrollableTextViewLineBox::SetBufferedLines(const std::vector< std::string > &lines){
    auto TextBuffer = DTextView->GetBuffer();
    if(DHightlightedBufferedLine < GetBufferedLineCount()){
        auto LineStart = TextBuffer->GetLineIter(DHightlightedBufferedLine);
        auto LineEnd = TextBuffer->GetLineIter(DHightlightedBufferedLine+1);
        TextBuffer->RemoveTag(DHighlightTag,LineStart,LineEnd);
    }
    // based off of https://stackoverflow.com/questions/5288396/c-ostream-out-manipulation/5289170#5289170
    std::ostringstream OutStream;
    auto Beg = begin(lines), End = end(lines);

    if(Beg != End){
        std::copy(Beg, prev(End), std::ostream_iterator<std::string>(OutStream, "\n"));
        Beg = prev(End);
    }
    if(Beg != End){
        OutStream<<*Beg;
    }
    TextBuffer->SetText(OutStream.str());
    DHightlightedBufferedLine = GetBufferedLineCount();
}

void CGUIScrollableTextViewLineBox::UpdateBufferedLine(size_t index, const std::string &line){
    auto TextBuffer = DTextView->GetBuffer();
    auto LineCount = GetBufferedLineCount();
    if(index < LineCount){
        auto LineStart = TextBuffer->GetLineIter(index);
        auto LineEnd = TextBuffer->GetLineIter(index+1);
        if(DHightlightedBufferedLine == index){
            TextBuffer->RemoveTag(DHighlightTag,LineStart,LineEnd);
        }
        if(index + 1 < LineCount){
            LineEnd->MoveBackwardChars(1); // Remove \n
        }
        TextBuffer->Delete(LineStart,LineEnd);
        TextBuffer->Insert(LineStart,line);
        if(DHightlightedBufferedLine == index){
            LineStart = TextBuffer->GetLineIter(DHightlightedBufferedLine);
            LineEnd = TextBuffer->GetLineIter(DHightlightedBufferedLine+1);
            TextBuffer->ApplyTag(DHighlightTag,LineStart,LineEnd);
        }
    }
}

void CGUIScrollableTextViewLineBox::HighlightBufferedLine(size_t index){
    auto TextBuffer = DTextView->GetBuffer();
    auto LineCount = GetBufferedLineCount();
    if(DHightlightedBufferedLine < LineCount){
        auto LineStart = TextBuffer->GetLineIter(DHightlightedBufferedLine);
        auto LineEnd = TextBuffer->GetLineIter(DHightlightedBufferedLine+1);
        TextBuffer->RemoveTag(DHighlightTag,LineStart,LineEnd);
    }
    if(index < LineCount){
        DHightlightedBufferedLine = index;
        auto LineStart = TextBuffer->GetLineIter(DHightlightedBufferedLine);
        auto LineEnd = TextBuffer->GetLineIter(DHightlightedBufferedLine+1);
        TextBuffer->ApplyTag(DHighlightTag,LineStart,LineEnd);
    }

}

size_t CGUIScrollableTextViewLineBox::GetHighlightedBufferedLine() const{
    return DHightlightedBufferedLine;
}

void CGUIScrollableTextViewLineBox::SetFontFamily(const std::string &family){

}

void CGUIScrollableTextViewLineBox::SetWidthCharacters(int chars){
    DWidthCharacters = chars;
    DScrollWindow->SetMinContentWidth(DTextView->GetRequiredCharactersWidth(DWidthCharacters));
}

void CGUIScrollableTextViewLineBox::SetButtonPressEventCallback(TGUICalldata calldata, TGUIScrollableLineBoxButtonEventCallback callback){
    DButtonPressCalldata = calldata;
    DButtonPressCallback = callback;
}

void CGUIScrollableTextViewLineBox::SetButtonReleaseEventCallback(TGUICalldata calldata, TGUIScrollableLineBoxButtonEventCallback callback){
    DButtonReleaseCalldata = calldata;
    DButtonReleaseCallback = callback;
}

void CGUIScrollableTextViewLineBox::SetScrollEventCallback(TGUICalldata calldata, TGUIScrollableLineBoxScrollEventCallback callback){
    DScrollCalldata = calldata;
    DScrollCallback = callback;
}
