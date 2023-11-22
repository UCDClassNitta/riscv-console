#include "VariableTreeViewDecorator.h"

CVariableTreeViewDecorator::CVariableTreeViewDecorator(std::shared_ptr< CGUITreeNodeView > treeview){
    DTreeNodeView = treeview;
    DNameValueGap = 2;
    DDepthIndention = 2;
}

std::string CVariableTreeViewDecorator::CreateTooltipMarkup(std::shared_ptr<CVariableTranslator::CProgramState> state){
    size_t MaxSize = 0;
    for(size_t Index = 0; Index < state->Value().AlternativeCount(); Index++){
        std::string Conversion, ConversionType;
        state->Value().GetAlternativeConversion(Index,Conversion,ConversionType);
        MaxSize = std::max(MaxSize,Conversion.length() + ConversionType.length() + 2);
    }
    std::string ReturnTooltip = std::string("<tt>");
    if(CVariableTranslator::DCompilationUnitTypeString == state->Type()){
        ReturnTooltip += "Compilation Unit";
    }
    else if(CVariableTranslator::DFunctionTypeString == state->Type()){
        ReturnTooltip += "Function";
    }
    else if(CVariableTranslator::DBlockTypeString == state->Type()){
        ReturnTooltip += "Block";
    }
    else{
        ReturnTooltip += state->Type();
    }

    for(size_t Index = 0; Index < state->Value().AlternativeCount(); Index++){
        std::string Conversion, ConversionType;
        state->Value().GetAlternativeConversion(Index,Conversion,ConversionType);
        auto GapSize = MaxSize - (Conversion.length() + ConversionType.length()+2);
        ReturnTooltip += std::string("\n") + ConversionType + std::string(":");
        while(GapSize){
            GapSize--;
            ReturnTooltip += " ";
        }
        ReturnTooltip += Conversion;
    }
    ReturnTooltip += std::string("</tt>");
    return ReturnTooltip;
}

void CVariableTreeViewDecorator::UpdateExpansions(std::shared_ptr<CGUITreeNodeIter> node){
    DExpansions[DDepthFirstNodePaths[DPathIndex++]] = node->GetExpanded();
    for(size_t Index = 0; Index < node->ChildCount(); Index++){
        UpdateExpansions(node->GetChild(Index));
    }
}

size_t CVariableTreeViewDecorator::CalculateMaxWidth(std::shared_ptr<CVariableTranslator::CProgramState> state, size_t depth) const{
    size_t Length = depth * DDepthIndention + state->Name().length() + DNameValueGap + state->Value().Default().length();
    for(size_t Index = 0; Index < state->ChildCount(); Index++){
        Length = std::max(Length, CalculateMaxWidth(state->Child(Index),depth+1));
    }
    return Length;
}

void CVariableTreeViewDecorator::DecorateNode(std::shared_ptr<CGUITreeNodeIter> parent, const std::string &path, std::shared_ptr<CVariableTranslator::CProgramState> state, size_t siblingindex, size_t depth){
    std::string BasePath = path.empty() ? path : path + std::string("::");
    std::string ThisPath = BasePath + state->Name();
    auto GapWidth = DMaxWidth - (depth * DDepthIndention + state->Name().length() + state->Value().Default().length());
    std::string NodeText = state->Name() + std::string(GapWidth,' ') + state->Value().Default();
    bool Expand = false;
    std::shared_ptr<CGUITreeNodeIter> Node;
    if(siblingindex < parent->ChildCount()){
        Node = parent->GetChild(siblingindex);
        Node->SetText(NodeText);
    }
    else{
        Node = parent->InsertChild(siblingindex,NodeText);
    }
    Node->SetTooltipMarkup(CreateTooltipMarkup(state));

    auto Search = DExpansions.find(ThisPath);
    if(DExpansions.end() != Search){
        Expand = Search->second;
    }
    Node->SetExpanded(Expand);
    DDepthFirstNodePaths.push_back(ThisPath);
    for(size_t Index = 0; Index < state->ChildCount(); Index++){
        DecorateNode(Node,ThisPath,state->Child(Index),Index,depth+1);
    }
    while(state->ChildCount() < Node->ChildCount()){
        Node->RemoveChild(state->ChildCount());
    }
}

void CVariableTreeViewDecorator::DecorateTreeView(const std::vector< std::shared_ptr<CVariableTranslator::CProgramState> > &states){
    auto Root = DTreeNodeView->RootNode();
    DPathIndex = 0;
    for(size_t Index = 0; Index < Root->ChildCount(); Index++){
        UpdateExpansions(Root->GetChild(Index));
    }
    DDepthFirstNodePaths.clear();
    DMaxWidth = 0;
    DTreeNodeView->AllowUpdates(false);
    for(size_t Index = 0; Index < states.size(); Index++){
        DMaxWidth = std::max(DMaxWidth, CalculateMaxWidth(states[Index],0));
    }
    
    for(size_t Index = 0; Index < states.size(); Index++){
        DecorateNode(Root,std::string(),states[Index],Index,0);
    }
    while(states.size() < Root->ChildCount()){
        Root->RemoveChild(states.size());
    }
    DTreeNodeView->AllowUpdates(true);
}
