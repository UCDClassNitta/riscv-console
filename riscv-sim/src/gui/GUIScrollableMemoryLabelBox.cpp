#include "GUIScrollableMemoryLabelBox.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>

const uint32_t CGUIScrollableMemoryLabelBox::DBytesPerLine = 16;

CGUIScrollableMemoryLabelBox::CGUIScrollableMemoryLabelBox(std::shared_ptr<CGUIFactory> guifactory, std::shared_ptr< CMemoryDevice > device, const std::unordered_map< uint32_t, uint32_t > &regions, size_t initsize) : CGUIScrollableLabelBox(guifactory, initsize){
    DGUIFactory = guifactory;
    DMemoryDevice = device;
    DAscending = true;
    DBaseAddress = ~0;
    for(auto &Region : regions){
        DMemoryBases.push_back(Region.first);
    }
    std::sort(DMemoryBases.begin(),DMemoryBases.end());
    DBufferedLineCount = 0;
    for(auto &BaseAddress : DMemoryBases){
        auto Search = regions.find(BaseAddress);
        DMemorySizes.push_back(Search->second);
        DMemoryLines.push_back((DMemorySizes.back() + DBytesPerLine-1)/DBytesPerLine);
        DBufferedLineCount += DMemoryLines.back();
    }
    DBaseAddress = DMemoryBases.front();
    DScrollbar->SetUpper(DBufferedLineCount);
    SetBaseLine(0);
    RefreshLabels();
    CGUIScrollableLabelBox::SetWidthCharacters(DBytesPerLine * 4 + 11);
    DHightlightedBufferedLine = DBufferedLineCount;
}

CGUIScrollableMemoryLabelBox::~CGUIScrollableMemoryLabelBox(){

}

bool CGUIScrollableMemoryLabelBox::LineIndexToBaseAddressMemoryIndex(size_t index, uint32_t &baseaddr, size_t &memindex) const{
    uint32_t Offset = index;
    for(size_t Index = 0; Index < DMemoryLines.size(); Index++){
        if(Offset < DMemoryLines[Index]){
            baseaddr = DMemoryBases[Index] + Offset * DBytesPerLine;
            memindex = Index;
            return true;
        }
        Offset -= DMemoryLines[Index];
    }
    return false;
}

void CGUIScrollableMemoryLabelBox::UpdateBaseLine(){
    uint32_t LineCount = 0;
    for(size_t Index = 0; Index < DMemoryBases.size(); Index++){
        if((DMemoryBases[Index] <= DBaseAddress)&&(DBaseAddress < DMemoryBases[Index] + DMemorySizes[Index])){
            LineCount += (DBaseAddress - DMemoryBases[Index]) / DBytesPerLine;
            break;
        }
        if(DBaseAddress < DMemoryBases[Index]){
            break;
        }
        LineCount += DMemoryLines[Index];
    }
    if(DAscending){
        SetBaseLine(LineCount);

    }
    else{
        LineCount = DBufferedLineCount - LineCount - 1;
        SetBaseLine(LineCount < GetLineCount() ? 0 : LineCount - GetLineCount());
    }
}

void CGUIScrollableMemoryLabelBox::RefreshLabels(){
    size_t MemoryIndex = 0;
    uint32_t CurrentAddress, MinAddress = 0;
    size_t CurrentLine = GetBaseLine();
    if(!DAscending){
        CurrentLine = DBufferedLineCount - CurrentLine - 1;
        CurrentLine = CurrentLine < GetLineCount() ? 0 : CurrentLine - GetLineCount() + 1;
    }
    LineIndexToBaseAddressMemoryIndex(CurrentLine,MinAddress,MemoryIndex);
    CurrentAddress = MinAddress;
    for(size_t Index = 0; Index < GetLineCount(); Index++){
        uint32_t NextAddress = CurrentAddress + DBytesPerLine;
        size_t LabelIndex = DAscending ? Index : GetLineCount() - Index - 1;
        UpdateMemoryLine(LabelIndex, CurrentAddress, NextAddress > DMemoryBases[MemoryIndex] + DMemorySizes[MemoryIndex] ? (DMemoryBases[MemoryIndex] + DMemorySizes[MemoryIndex]) - CurrentAddress : DBytesPerLine);
        CurrentAddress = NextAddress;
        if(DMemoryBases[MemoryIndex] + DMemorySizes[MemoryIndex] <= CurrentAddress){
            MemoryIndex++;
            if(MemoryIndex >= DMemoryBases.size()){
                break;
            }
            CurrentAddress = DMemoryBases[MemoryIndex];
        }
    }
    if(DAscending || (DBaseAddress < MinAddress) || (DBaseAddress >= CurrentAddress)){
        DBaseAddress = MinAddress;
    }
}

void CGUIScrollableMemoryLabelBox::UpdateMemoryLine(size_t index, uint32_t addr, uint32_t bytes){
    DLabels[index]->SetText(FormatMemoryLine(DMemoryDevice->LoadData(addr,bytes),addr,bytes));
}

std::string CGUIScrollableMemoryLabelBox::FormatMemoryLine(const uint8_t *buffer, uint32_t addr, uint32_t bytes){
    std::stringstream Stream;

    Stream<<std::setfill('0') << std::setw(8) << std::hex << addr;
    Stream<<":";
    for(uint32_t Index = 0; Index < bytes; Index++){
        Stream<<" "<<std::setfill('0') << std::setw(2) << std::hex << uint32_t(buffer[Index]);
    }
    for(uint32_t Index = bytes; Index < DBytesPerLine; Index++){
        Stream<<"   ";
    }
    Stream<<" ";
    for(uint32_t Index = 0; Index < bytes; Index++){
        Stream<<((' ' <= buffer[Index] && '~' >= buffer[Index]) ? char(buffer[Index]) : '.' );
    }
    return Stream.str();
}

uint32_t CGUIScrollableMemoryLabelBox::GetBaseAddress(){
    return DBaseAddress;
}

void CGUIScrollableMemoryLabelBox::SetBaseAddress(uint32_t addr, bool ascending){
    bool PrevAscending = DAscending;
    DBaseAddress = addr & (~(DBytesPerLine-1));
    DAscending = ascending;
    UpdateBaseLine();
    if(PrevAscending != DAscending){
        RefreshLabels();
    }
}

bool CGUIScrollableMemoryLabelBox::GetAddressAscending() const{
    return DAscending;
}

void CGUIScrollableMemoryLabelBox::SetLineCount(size_t count){
    CGUIScrollableLabelBox::SetLineCount(count);
}

size_t CGUIScrollableMemoryLabelBox::GetBufferedLineCount() const{
    return DBufferedLineCount;
}

std::string CGUIScrollableMemoryLabelBox::GetBufferedLine(size_t index) const{
    size_t MemoryIndex = 0;
    uint32_t CurrentAddress;
    size_t CurrentLine = GetBaseLine();

    if(!DAscending){
        CurrentLine = DBufferedLineCount - CurrentLine - 1;
        CurrentLine = CurrentLine < GetLineCount() ? 0 : CurrentLine - GetLineCount();
    }

    LineIndexToBaseAddressMemoryIndex(CurrentLine,CurrentAddress,MemoryIndex);

    uint32_t NextAddress = CurrentAddress + DBytesPerLine;
    uint32_t Bytes = NextAddress > DMemoryBases[MemoryIndex] + DMemorySizes[MemoryIndex] ? (DMemoryBases[MemoryIndex] + DMemorySizes[MemoryIndex]) - CurrentAddress : DBytesPerLine;

    return FormatMemoryLine(DMemoryDevice->LoadData(CurrentAddress,Bytes), CurrentAddress, Bytes);
}

void CGUIScrollableMemoryLabelBox::SetBufferedLines(const std::vector< std::string > &lines){
    
}

void CGUIScrollableMemoryLabelBox::UpdateBufferedLine(size_t index, const std::string &line){
    
}

void CGUIScrollableMemoryLabelBox::SetWidthCharacters(int chars){
    int MinWidth = DBytesPerLine * 4 + 11;
    if(chars >= MinWidth){
        CGUIScrollableLabelBox::SetWidthCharacters(chars);
    }
    
}

void CGUIScrollableMemoryLabelBox::Refresh(){
    RefreshLabels();
}
