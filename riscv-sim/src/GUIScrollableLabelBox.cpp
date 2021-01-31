#include "GUIScrollableLabelBox.h"
#include "GUIFactory.h"

CGUIScrollableLabelBox::CGUIScrollableLabelBox(size_t initsize){
    DContainingFrame = CGUIFactory::NewFrame();
    DContainingGrid = CGUIFactory::NewGrid();
    DScrollbar = CGUIFactory::NewScrollBar(CGUIScrollBar::EOrientation::Vertical);
    DContainingGrid->Attach(DScrollbar,1,0,1,1);
    DBaseLine = 0;
    DHightlightedBufferedLine = 0;
    DFontFamily = "monospace";
    DWidthCharacters = -1;
    DMaxWidthCharacters = -1;

    DButtonPressCalldata = nullptr;
    DButtonPressCallback = nullptr;

    DButtonReleaseCalldata = nullptr;
    DButtonReleaseCallback = nullptr;

    DScrollCalldata = nullptr;
    DScrollCallback = nullptr;
    
    DScrollbar->SetLower(0);
    DScrollbar->SetUpper(1);
    DScrollbar->SetStepIncrement(1);
    DScrollbar->SetPageIncrement(1);
    SetLineCount(initsize);
    DScrollbar->SetValueChangedEventCallback(this,ScrollBarChangedEventCallback);
    DContainingFrame->Add(DContainingGrid);
}

CGUIScrollableLabelBox::~CGUIScrollableLabelBox(){

}

bool CGUIScrollableLabelBox::ScrollBarChangedEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data){
    CGUIScrollableLabelBox *ScrollableBox = static_cast<CGUIScrollableLabelBox *>(data);
    return ScrollableBox->ScrollBarChangedEvent(widget);
}

bool CGUIScrollableLabelBox::WidgetScrollEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIScrollEvent &event, TGUICalldata data){
    CGUIScrollableLabelBox *ScrollableBox = static_cast<CGUIScrollableLabelBox *>(data);
    return ScrollableBox->WidgetScrollEvent(widget, event);
}

bool CGUIScrollableLabelBox::WidgetButtonEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CGUIScrollableLabelBox *ScrollableBox = static_cast<CGUIScrollableLabelBox *>(data);
    for(size_t Index = 0; Index < ScrollableBox->DLabelEventBoxes.size(); Index++){
        if(widget == ScrollableBox->DLabelEventBoxes[Index]){
            if(event.DType.IsButtonRelease()){
                if(ScrollableBox->DButtonReleaseCallback){
                    if(!ScrollableBox->DButtonReleaseCallback(ScrollableBox->shared_from_this(), event, ScrollableBox->DBaseLine + Index, ScrollableBox->DButtonReleaseCalldata)){
                        ScrollableBox->DButtonReleaseCallback = nullptr;
                        ScrollableBox->DButtonReleaseCalldata = nullptr;
                    }
                }
            }
            else{
                if(ScrollableBox->DButtonPressCallback){
                    if(!ScrollableBox->DButtonPressCallback(ScrollableBox->shared_from_this(), event, ScrollableBox->DBaseLine + Index, ScrollableBox->DButtonPressCalldata)){
                        ScrollableBox->DButtonPressCallback = nullptr;
                        ScrollableBox->DButtonPressCalldata = nullptr;
                    }
                }
            }
            break;
        }
    }
    
    return true;
}

bool CGUIScrollableLabelBox::ScrollBarChangedEvent(std::shared_ptr<CGUIWidget> widget){
    size_t NewBaseLine = DScrollbar->GetValue();
    if(NewBaseLine != DBaseLine){
        DBaseLine = NewBaseLine;
        RefreshLabels();
    }
    if(DScrollCallback){
        if(!DScrollCallback(shared_from_this(),DScrollCalldata)){
            DScrollCallback = nullptr;
            DScrollCalldata = nullptr;
        }
    }
    
    return true;
}

// Code form Effective Modern C++ by Scott Meyers (see Item 10)
template<typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E enumerator) noexcept{
    return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

bool CGUIScrollableLabelBox::WidgetScrollEvent(std::shared_ptr<CGUIWidget> widget, SGUIScrollEvent &event){
    if(event.DDirection == EGUIScrollDirection::Up){
        uint32_t Index = DScrollbar->GetValue();
        if(Index){
            Index--;
            DScrollbar->SetValue(Index);
        }
    }
    else if(event.DDirection == EGUIScrollDirection::Down){
        uint32_t Index = DScrollbar->GetValue();
        if(Index + DLabels.size() < GetBufferedLineCount()){
            Index++;
            DScrollbar->SetValue(Index);
        }
    }
    else if(event.DDirection == EGUIScrollDirection::Smooth){
        double Index = DScrollbar->GetValue() + event.DDeltaY;
        if(Index + DLabels.size() > GetBufferedLineCount()){
            Index = (double)GetBufferedLineCount() - DLabels.size();
        }
        if(Index < 0){
            Index = 0;
        }
        DScrollbar->SetValue(Index);
    }
    if(DScrollCallback){
        if(!DScrollCallback(shared_from_this(),DScrollCalldata)){
            DScrollCallback = nullptr;
            DScrollCalldata = nullptr;
        }
    }

    return true;
}

void CGUIScrollableLabelBox::RefreshLabels(){
    for(size_t Index = 0; Index < DLabels.size(); Index++){
        if(DBaseLine + Index < DBufferedLines.size()){
            DLabels[Index]->SetText(DBufferedLines[DBaseLine + Index]);
            DLabels[Index]->SetBold(DHightlightedBufferedLine == DBaseLine + Index);
        }
        else{
            DLabels[Index]->SetText("");
            DLabels[Index]->SetBold(false);
        }
    }
}

std::shared_ptr<CGUIWidget> CGUIScrollableLabelBox::ContainingWidget() const{
    //return DContainingGrid;
    return DContainingFrame;
}

size_t CGUIScrollableLabelBox::GetBaseLine() const{
    return DBaseLine;
}

void CGUIScrollableLabelBox::SetBaseLine(size_t line){
    if(line < GetBufferedLineCount()){
        DScrollbar->SetValue(line);
    }
}

size_t CGUIScrollableLabelBox::GetLineCount() const{
    return DLabels.size();
}

void CGUIScrollableLabelBox::SetLineCount(size_t count){
    if(!count){
        return;
    }
    while(count < DLabels.size()){
        DContainingGrid->Remove(DLabelEventBoxes.back());
        DLabelEventBoxes.pop_back();
        DLabels.pop_back();
    }
    while(DLabels.size() < count){
        auto Label = CGUIFactory::NewLabel("");
        auto EventBox = CGUIFactory::NewEventBox();
        Label->SetFontFamily(DFontFamily);
        Label->SetJustification(SGUIJustificationType::Left);
        if(0 < DWidthCharacters){
            Label->SetWidthCharacters(DWidthCharacters);
        }
        if(0 < DMaxWidthCharacters){
            Label->SetMaxWidthCharacters(DWidthCharacters);
        }
        EventBox->Add(Label);
        EventBox->SetButtonPressEventCallback(this,WidgetButtonEventCallback);
        EventBox->SetButtonReleaseEventCallback(this,WidgetButtonEventCallback);
        DContainingGrid->Attach(EventBox,0,DLabels.size(),1,1);
        EventBox->SetScrollEventCallback(this,WidgetScrollEventCallback);
        EventBox->EnableEvent(EGUIEvent::Scroll);
        DLabelEventBoxes.push_back(EventBox);
        DLabels.push_back(Label);
    }
    DContainingGrid->Remove(DScrollbar);
    DContainingGrid->Attach(DScrollbar,1,0,1,DLabels.size());
    DScrollbar->SetPageSize(DLabels.size());
    RefreshLabels();
}

size_t CGUIScrollableLabelBox::GetBufferedLineCount() const{
    return DBufferedLines.size();
}

std::string CGUIScrollableLabelBox::GetBufferedLine(size_t index) const{
    if(index < DBufferedLines.size()){
        return DBufferedLines[index];
    }
    return std::string();
}

void CGUIScrollableLabelBox::SetBufferedLines(const std::vector< std::string > &lines){
    DBufferedLines = lines;
    DScrollbar->SetUpper(DBufferedLines.size());
    if(DBaseLine >= DBufferedLines.size()){
        DBaseLine = DLabels.size() >= DBufferedLines.size() ? 0 : DBufferedLines.size() - DLabels.size();
        DScrollbar->SetValue(DBaseLine);
    }
    DHightlightedBufferedLine = DBufferedLines.size();
    RefreshLabels();
}

void CGUIScrollableLabelBox::UpdateBufferedLine(size_t index, const std::string &line){
    if(index < DBufferedLines.size()){
        if((DBaseLine <= index)&&(index < DBaseLine + DLabels.size())){
            DLabels[index - DBaseLine]->SetText(line);
        }
        DBufferedLines[index] = line;
    }
}

void CGUIScrollableLabelBox::AppendBufferedLine(const std::string &line){
    auto CurrentSize = DBufferedLines.size();
    if((DBaseLine <= CurrentSize)&&(CurrentSize < DBaseLine + DLabels.size())){
        DLabels[CurrentSize - DBaseLine]->SetText(line);
    }
    DBufferedLines.push_back(line);
    DScrollbar->SetUpper(DBufferedLines.size());
}

void CGUIScrollableLabelBox::HighlightBufferedLine(size_t index){
    auto OldHighlighedLine = DHightlightedBufferedLine;
    DHightlightedBufferedLine = index;
    if((DBaseLine <= OldHighlighedLine)&&(OldHighlighedLine < DBaseLine + DLabels.size())){
        DLabels[OldHighlighedLine - DBaseLine]->SetBold(false);
    }
    if((DBaseLine <= DHightlightedBufferedLine)&&(DHightlightedBufferedLine < DBaseLine + DLabels.size())){
        DLabels[DHightlightedBufferedLine - DBaseLine]->SetBold(true);
    }
}

size_t CGUIScrollableLabelBox::GetHighlightedBufferedLine() const{
    return DHightlightedBufferedLine;
}

void CGUIScrollableLabelBox::SetFontFamily(const std::string &family){
    DFontFamily = family;
    for(auto &Label : DLabels){
        Label->SetFontFamily(DFontFamily);
    }
}

void CGUIScrollableLabelBox::SetWidthCharacters(int chars){
    DWidthCharacters = chars;
    for(auto &Label : DLabels){
        Label->SetWidthCharacters(DWidthCharacters);
    }
}

void CGUIScrollableLabelBox::SetMaxWidthCharacters(int chars){
    DMaxWidthCharacters = chars;
    for(auto &Label : DLabels){
        Label->SetMaxWidthCharacters(DMaxWidthCharacters);
    }
}

void CGUIScrollableLabelBox::SetButtonPressEventCallback(TGUICalldata calldata, TGUIScrollableLabelBoxButtonEventCallback callback){
    DButtonPressCalldata = calldata;
    DButtonPressCallback = callback;
}

void CGUIScrollableLabelBox::SetButtonReleaseEventCallback(TGUICalldata calldata, TGUIScrollableLabelBoxButtonEventCallback callback){
    DButtonReleaseCalldata = calldata;
    DButtonReleaseCallback = callback;
}

void CGUIScrollableLabelBox::SetScrollEventCallback(TGUICalldata calldata, TGUIScrollableLabelBoxScrollEventCallback callback){
    DScrollCalldata = calldata;
    DScrollCallback = callback;
}
