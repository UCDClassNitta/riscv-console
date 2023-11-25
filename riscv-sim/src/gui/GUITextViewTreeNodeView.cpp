#include "GUITextViewTreeNodeView.h"
#include <sstream>
#include <iterator>
#include <cstdio>

const char CGUITextViewTreeNodeView::DVerticalBar[]         = {char(0xE2), char(0x94), char(0x82), '\0'};
const char CGUITextViewTreeNodeView::DHorizontalBar[]       = {char(0xE2), char(0x94), char(0x80), '\0'};
const char CGUITextViewTreeNodeView::DUpRightBar[]          = {char(0xE2), char(0x94), char(0x94), '\0'};
const char CGUITextViewTreeNodeView::DVerticalRightBar[]    = {char(0xE2), char(0x94), char(0x9C), '\0'};
const char CGUITextViewTreeNodeView::DSquaredPlus[]         = {char(0xE2), char(0x8A), char(0x9E), '\0'};
const char CGUITextViewTreeNodeView::DSquaredMinus[]        = {char(0xE2), char(0x8A), char(0x9F), '\0'};

CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::CGUITextViewTreeNodeIter(std::shared_ptr< CGUITextViewTreeNodeView > treeview, std::shared_ptr< CGUITextViewTreeNodeIter > parent, const std::string &text, bool expanded){
    DTreeNodeView = treeview;
    DParent = parent;
    DText = text;
    DExpanded = expanded;
    DTooltipIsMarkup = false;
}

void CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::MarkTreeDirty(){
    if(auto TreeView = DTreeNodeView.lock()){
        TreeView->MarkDirty();
    }
}

bool CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::GetExpanded() const{
    return DExpanded;
}

void CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::SetExpanded(bool expand){
    if(DParent.expired()){
        return;
    }
    if(DExpanded != expand){
        DExpanded = expand;
        if(!DChildren.empty()){
            MarkTreeDirty();
        }
    }
}

void CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::SetExpandedAll(bool expand){
    for(auto Child : DChildren){
        Child->SetExpandedAll(expand);
    }
}

std::string CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::GetText() const{
    return DText;
}

bool CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::SetText(const std::string &text){
    if(!DParent.expired()){
        if(DText != text){
            DText = text;
            MarkTreeDirty();
        }
        return true;
    }
    return false;
}

std::string CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::GetTooltip() const{
    return DTooltip;
}

bool CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::GetTooltipIsMarkup() const{
    return DTooltipIsMarkup;
}

void CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::SetTooltipText(const std::string &tip){
    DTooltip = tip;
    DTooltipIsMarkup = false;
}

void CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::SetTooltipMarkup(const std::string &markup){
    DTooltip = markup;
    DTooltipIsMarkup = true;
}

size_t CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::Depth() const{
    if(auto Parent = DParent.lock()){
        return Parent->Depth() + 1;
    }
    return 0;
}

size_t CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::ChildCount() const{
    return DChildren.size();
}

std::shared_ptr<CGUITreeNodeIter> CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::Parent() const{
    return DParent.lock();
}

std::shared_ptr<CGUITreeNodeIter> CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::GetChild(size_t index) const{
    if((0 <= index)&&(index < DChildren.size())){
        return DChildren[index];
    }
    return nullptr;
}

std::shared_ptr<CGUITreeNodeIter> CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::InsertChild(size_t index, const std::string &text){
    auto TreeNodeView = DTreeNodeView.lock();
    auto NewChild = std::make_shared<CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter>(TreeNodeView,shared_from_this(),text,TreeNodeView ? TreeNodeView->GetCreateExpanded() : false);
    auto Index = index > DChildren.size() ? DChildren.size() : index;
    Index = 0 > Index ? 0 : Index;
    DChildren.insert(DChildren.begin() + Index, NewChild);
    MarkTreeDirty();
    return NewChild;
}

bool CGUITextViewTreeNodeView::CGUITextViewTreeNodeIter::RemoveChild(size_t index){
    if((0 <= index)&&(index < DChildren.size())){
        auto Child = DChildren[index];
        DChildren.erase(DChildren.begin() + index);
        Child->DParent.reset();
        Child->DTreeNodeView.reset();
        if(DChildren.empty() || DExpanded){
            MarkTreeDirty();
        }
        return true;
    }
    return false;
}

CGUITextViewTreeNodeView::CGUITextViewTreeNodeView(std::shared_ptr<CGUIFactory> guifactory, size_t minlines, size_t mincols){
    DGUIFactory = guifactory;
    DTextView = DGUIFactory->NewTextView();
    DContainingFrame = DGUIFactory->NewFrame();
    DScrollWindow = DGUIFactory->NewScrollWindow();
    DScrollWindow->Add(DTextView);
    DContainingFrame->Add(DScrollWindow);
    DTextView->SetWrapMode(CGUITextView::EWrapModeType::None);
    DTextView->SetMonospace(true);
    DTextView->SetCursorVisible(false);

    DButtonPressCalldata = nullptr;
    DButtonPressCallback = nullptr;

    DButtonReleaseCalldata = nullptr;
    DButtonReleaseCallback = nullptr;
    DScrollWindow->SetMinContentWidth(DTextView->GetRequiredCharactersWidth(mincols));
    DScrollWindow->SetMinContentHeight(DTextView->GetRequiredLinesHeight(minlines));
    DTextView->SetButtonPressEventCallback(this,WidgetButtonEventCallback);
    DTextView->SetButtonReleaseEventCallback(this,WidgetButtonEventCallback);
    DTextView->SetMotionEventCallback(this,WidgetMotionEventCallback);
    DTextView->SetCursor(nullptr);
    DContainingFrame->SetCursor(nullptr);
    DAllowUpdates = true;
    DDirty = false;
    DCreateExpanded = false;
}

CGUITextViewTreeNodeView::~CGUITextViewTreeNodeView(){

}

void CGUITextViewTreeNodeView::Update(){
    DLines.clear();
    DVisibleNodes.clear();
    auto TheRootNode = RootNode();
    for(size_t Index = 0; Index < TheRootNode->ChildCount(); Index++){
        std::vector<bool> MoreChildren = {Index + 1 < TheRootNode->ChildCount()};
        UpdateNode(TheRootNode->GetChild(Index),MoreChildren);
    }

    auto TextBuffer = DTextView->GetBuffer();
    
    // based off of https://stackoverflow.com/questions/5288396/c-ostream-out-manipulation/5289170#5289170
    std::ostringstream OutStream;
    auto Beg = begin(DLines), End = end(DLines);

    if(Beg != End){
        std::copy(Beg, prev(End), std::ostream_iterator<std::string>(OutStream, "\n"));
        Beg = prev(End);
    }
    if(Beg != End){
        OutStream<<*Beg;
    }
    TextBuffer->SetText(OutStream.str());

    DDirty = false;
}

void CGUITextViewTreeNodeView::UpdateNode(std::shared_ptr<CGUITreeNodeIter> node, const std::vector< bool > &morechildren){
    std::string Line;

    for(size_t Index = 0; Index + 1 < morechildren.size(); Index++){
        Line += morechildren[Index] ? std::string(DVerticalBar) : std::string(" ");
        Line += std::string(" ");
    }
    if(node->ChildCount()){
        Line += node->GetExpanded() ? std::string(DSquaredMinus) : std::string(DSquaredPlus);
        Line += std::string(" ");
    }
    else{
        // Leaf
        Line += morechildren.back() ? std::string(DVerticalRightBar) : std::string(DUpRightBar);
        Line += std::string(" ");
    }
    Line += node->GetText();
    DLines.push_back(Line);
    DVisibleNodes.push_back(node);
    if(node->GetExpanded()){
        for(size_t Index = 0; Index < node->ChildCount(); Index++){
            auto MoreChildren = morechildren;
            MoreChildren.push_back(Index + 1 < node->ChildCount());
            UpdateNode(node->GetChild(Index),MoreChildren);
        }
    }
}

bool CGUITextViewTreeNodeView::WidgetMotionEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIMotionEvent &event, TGUICalldata data){
    CGUITextViewTreeNodeView *TreeNodeView = static_cast<CGUITextViewTreeNodeView *>(data);
    auto VerticalScrollBar = TreeNodeView->DScrollWindow->GetVerticalScrollBar();
    auto Y = event.DWindowY + VerticalScrollBar->GetValue();
    size_t LineNumber = TreeNodeView->DTextView->GetLineNumberAtY(Y);
    if(LineNumber < TreeNodeView->DVisibleNodes.size()){
        auto Node = TreeNodeView->DVisibleNodes[LineNumber];
        if(Node->GetTooltipIsMarkup()){
            TreeNodeView->DTextView->SetTooltipMarkup(Node->GetTooltip());
        }
        else{
            TreeNodeView->DTextView->SetTooltipText(Node->GetTooltip());
        }
    }
    return true;
}

bool CGUITextViewTreeNodeView::WidgetButtonEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CGUITextViewTreeNodeView *TreeNodeView = static_cast<CGUITextViewTreeNodeView *>(data);
    if(event.DType.IsButtonPress()){// && event.DButton == 0){
        //TreeNodeView->DTextView->GetLineNumberAtY()
        auto VerticalScrollBar = TreeNodeView->DScrollWindow->GetVerticalScrollBar();
        auto HorizontalScrollBar = TreeNodeView->DScrollWindow->GetHorizontalScrollBar();
        auto X = event.DWindowX + HorizontalScrollBar->GetValue();
        auto Y = event.DWindowY + VerticalScrollBar->GetValue();
        size_t LineOffset = TreeNodeView->DTextView->GetLineOffsetAtPosition(X,Y);
        size_t LineNumber = TreeNodeView->DTextView->GetLineNumberAtY(Y);
        if(LineNumber < TreeNodeView->DVisibleNodes.size()){
            auto Node = TreeNodeView->DVisibleNodes[LineNumber];
            if(((Node->Depth() - 1) * 2 == LineOffset)&&(Node->ChildCount())){
                Node->SetExpanded(!Node->GetExpanded());
                return true;
            }
        }
    }
    if(event.DType.IsButtonRelease()){
        if(TreeNodeView->DButtonReleaseCallback){
            if(!TreeNodeView->DButtonReleaseCallback(TreeNodeView->shared_from_this(), event, TreeNodeView->DButtonReleaseCalldata)){
                TreeNodeView->DButtonReleaseCallback = nullptr;
                TreeNodeView->DButtonReleaseCalldata = nullptr;
            }
        }
    }
    else{
        if(TreeNodeView->DButtonPressCallback){
            if(!TreeNodeView->DButtonPressCallback(TreeNodeView->shared_from_this(), event, TreeNodeView->DButtonPressCalldata)){
                TreeNodeView->DButtonPressCallback = nullptr;
                TreeNodeView->DButtonPressCalldata = nullptr;
            }
        }
    }
    return true;
}

std::shared_ptr<CGUIWidget> CGUITextViewTreeNodeView::ContainingWidget() const{
    return DContainingFrame;
}

std::shared_ptr<CGUITreeNodeIter> CGUITextViewTreeNodeView::RootNode(){
    if(!DRoot){
        DRoot = std::make_shared<CGUITextViewTreeNodeIter>( shared_from_this(), nullptr, std::string(), DCreateExpanded);
    }
    return DRoot;
}

std::shared_ptr<CGUITreeNodeIter> CGUITextViewTreeNodeView::GetNodeAtXY(int x, int y) const{
    auto VerticalScrollBar = DScrollWindow->GetVerticalScrollBar();
    auto Y = y + VerticalScrollBar->GetValue();
    size_t LineNumber = DTextView->GetLineNumberAtY(Y);
    if(LineNumber < DVisibleNodes.size()){
        return DVisibleNodes[LineNumber];
    }
    return nullptr;
}

bool CGUITextViewTreeNodeView::GetCreateExpanded() const{
    return DCreateExpanded;
}

void CGUITextViewTreeNodeView::SetCreateExpanded(bool exp){
    DCreateExpanded = exp;
}

void CGUITextViewTreeNodeView::AllowUpdates(bool allow){
    DAllowUpdates = allow;
    if(DAllowUpdates && DDirty){
        Update();
    }
}

void CGUITextViewTreeNodeView::MarkDirty(){
    DDirty = true;
    if(DAllowUpdates){
        Update();
    }
}

void CGUITextViewTreeNodeView::SetVerticalExpand(bool exp){
    DContainingFrame->SetVerticalExpand(exp);
}

void CGUITextViewTreeNodeView::SetHorizontalExpand(bool exp){
    DContainingFrame->SetHorizontalExpand(exp);
}

void CGUITextViewTreeNodeView::SetButtonPressEventCallback(TGUICalldata calldata, TGUITreeNodeViewButtonEventCallback callback){
    DButtonPressCalldata = calldata;
    DButtonPressCallback = callback;
}

void CGUITextViewTreeNodeView::SetButtonReleaseEventCallback(TGUICalldata calldata, TGUITreeNodeViewButtonEventCallback callback){
    DButtonReleaseCalldata = calldata;
    DButtonReleaseCallback = callback;
}
