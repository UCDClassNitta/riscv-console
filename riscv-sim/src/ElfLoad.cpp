#include "ElfLoad.h"
#include "SeekableDataSourceSandbox.h"
#include <cstdio>
#include <cstring>
#include <cinttypes>
#include <algorithm>

CElfLoad::CElfLoad(std::shared_ptr< CDataSource > src){
    DInputSource = std::make_shared<CBufferedSeekableDataSource>(src);
    DInputSourceConverter = std::make_shared< CSeekableDataSourceConverter >(DInputSource);
    DValidFile = false;
    if(ValidateHeader()){
        if(ReadProgramHeaders()){
            LoadProgramPayloads();
            if(ReadSectionHeaders()){
                ReadSectionNames();
                ReadStringTables();
                ReadSymbolTables();
                ReadDebugInfo();
                MergeSymbols();
                //PrintHeaders();
            }
            DValidFile = true;
        }
        else{
            printf("Invalid Program Headers\n");
        }
    }
    else{
        printf("Invalid Header\n");
    }

}

CElfLoad::~CElfLoad(){

    
}

size_t CElfLoad::ProgramHeaderCount() const{
    return DProgramHeaders.size();
}

const CElfStructures::SProgramHeader &CElfLoad::ProgramHeader(size_t index) const{
    return DProgramHeaders[index];
}

size_t CElfLoad::SectionHeaderCount() const{
    return DSectionHeaders.size();
}

const CElfStructures::SSectionHeader &CElfLoad::SectionHeader(size_t index) const{
    return DSectionHeaders[index];
}

void CElfLoad::PrintHeaders(){
    PrintHeader();
    printf("---- PROGRAM HEADERS -----\n");
    for(auto &Header : DProgramHeaders){
        PrintProgramHeader(Header);
    }
    printf("---- SECTION HEADERS -----\n");
    for(auto &Header : DSectionHeaders){
        PrintSectionHeader(Header);
    }
    printf("---- SYMBOL ENTITIES -----\n");
    for(auto &Entity : DSymbolEntities){
        PrintSymbolEntity(Entity);
    }
    /*
    for(auto &StrKeyVal : DStrings){
        printf("%d,%d => %s\n",StrKeyVal.first.first,StrKeyVal.first.second,StrKeyVal.second.c_str());
    }
    */
}

bool CElfLoad::ValidateHeader(){
    uint8_t Buffer[16];

    DInputSourceConverter->Seek(0);
    DInputSourceConverter->Read(Buffer,16);
    //ReadData(Buffer, 16);
    //printf("16B header read\n");
    if((Buffer[0] != 0x7F)||(Buffer[1] != 'E')||(Buffer[2] != 'L')||(Buffer[3] != 'F')){
        return false;
    }
    D32Bit = Buffer[4] == 1;
    DLittleEndian = Buffer[5] == 1;
    DInputSourceConverter->LittleEndian(DLittleEndian);
    if(Buffer[6] != 1){
        return false;
    }
    DOSABI = Buffer[7];
    DObjectType = DInputSourceConverter->ReadUINT16();
    DMachine  = DInputSourceConverter->ReadUINT16();
    DELFVersion = DInputSourceConverter->ReadUINT32();
    DEntry = D32Bit ? DInputSourceConverter->ReadUINT32() : DInputSourceConverter->ReadUINT64();
    DProgramHeaderOffset = D32Bit ? DInputSourceConverter->ReadUINT32() : DInputSourceConverter->ReadUINT64();
    DSectionHeaderOffset = D32Bit ? DInputSourceConverter->ReadUINT32() : DInputSourceConverter->ReadUINT64();
    DFlags = DInputSourceConverter->ReadUINT32();
    DHeaderSize = DInputSourceConverter->ReadUINT16();
    DProgramHeaderEntrySize = DInputSourceConverter->ReadUINT16();
    DProgramHeaderEntryCount = DInputSourceConverter->ReadUINT16();
    DSectionHeaderEntrySize = DInputSourceConverter->ReadUINT16();
    DSectionHeaderEntryCount = DInputSourceConverter->ReadUINT16();
    DSectionNameIndex = DInputSourceConverter->ReadUINT16();

    return true;
}

void CElfLoad::PrintHeader(){
    printf("Machine %d bits\n",D32Bit ? 32 : 64);
    printf("Endian %s\n",DLittleEndian ? "little": "big");
    printf("OBJTYPE %hu\n",(uint16_t)DObjectType);
    printf("MTYPE %04hX\n",(uint16_t)DMachine);
    printf("ELF Version %u\n",DELFVersion);
    printf("Entry 0x%08" PRIX64 "\n",DEntry);
    printf("PHOFF 0x%08" PRIX64 "\n",DProgramHeaderOffset);
    printf("SHOFF 0x%08" PRIX64 "\n",DSectionHeaderOffset);
    printf("HDSZ  %" PRIu16 "\n",DHeaderSize);
    printf("PHES  %" PRIu16 "\n",DProgramHeaderEntrySize);
    printf("PHEC  %" PRIu16 "\n",DProgramHeaderEntryCount);
    printf("SHES  %" PRIu16 "\n",DSectionHeaderEntrySize);
    printf("SHEC  %" PRIu16 "\n",DSectionHeaderEntryCount);
    printf("SNID  %" PRIu16 "\n",DSectionNameIndex);
}

void CElfLoad::PrintProgramHeader(const CElfStructures::SProgramHeader &header){
    printf("TYPE  0x%08" PRIX32 "\n",header.DSegmentType);
    printf("FLAGS 0x%08" PRIX32 "\n",header.DFlags);

    printf("FOFF  0x%08" PRIX64 "\n",header.DFileOffset);
    printf("VADDR 0x%08" PRIX64 "\n",header.DVirtualAddress);
    printf("PADDR 0x%08" PRIX64 "\n",header.DPhysicalAddress);
    printf("FSIZE 0x%08" PRIX64 "\n",header.DFileSize);
    printf("MSIZE 0x%08" PRIX64 "\n",header.DMemorySize);
    printf("ALIGN %" PRId64 "\n",header.DAlignment);
    for(auto &Symbol : header.DSymbols){
        printf("%s => 0x%08" PRIX64 "\n",Symbol.second.c_str(),Symbol.first);
    }
    printf("\n");
}

void CElfLoad::PrintSectionHeader(const CElfStructures::SSectionHeader &header){
    auto SectionName = DSectionNames.GetString(header.DNameIndex);
    if(!SectionName.empty()){
        printf("NAME  %s\n",SectionName.c_str());
    }
    else{
        printf("NAME  0x%08" PRIX32 "\n",header.DNameIndex);
    }
    printf("TYPE  0x%08" PRIX32 "\n",header.DType);
    printf("FLAGS 0x%08" PRIX64 "\n",header.DFlags);

    printf("FOFF  0x%08" PRIX64 "\n",header.DFileOffset);
    printf("VADDR 0x%08" PRIX64 "\n",header.DVirtualAddress);
    printf("FSIZE 0x%08" PRIX64 "\n",header.DSize);
    printf("LINK  0x%08" PRIX32 "\n",header.DLink);
    printf("INFO  0x%08" PRIX64 "\n",header.DInfo);
    printf("ESIZE %" PRId64 "\n",header.DEntrySize);
    printf("ALIGN %" PRId64 "\n\n",header.DAddressAlign);

}

void CElfLoad::PrintSymbolEntity(CElfStructures::SSymbolEntity &entity){
    auto Search = DStringTables.find(entity.DNameSectionIndex);
    if(Search != DStringTables.end()){
        printf("NAME  %s\n",Search->second.GetString(entity.DNameIndex).c_str());
    }
    else{
        printf("NAME  0x%08" PRIX32 "\n",entity.DNameIndex);
    }
    printf("ADDR  0x%08" PRIX64 "\n",entity.DAddress);
    printf("SIZE  0x%08" PRIX64 "\n",entity.DSize);
    printf("INFO  0x%02" PRIX8 "\n",entity.DInfo);
    printf("OTHER 0x%02" PRIX8 "\n",entity.DOther);
    printf("SHNDX 0x%04" PRIX16 "\n\n",entity.DSectionIndex);
}

bool CElfLoad::ReadProgramHeaders(){
    DInputSourceConverter->Seek(DProgramHeaderOffset);
    for(uint16_t Index = 0; Index < DProgramHeaderEntryCount; Index++){
        CElfStructures::SProgramHeader TempHeader;
        TempHeader.DSegmentType = CElfConstants::EProgramHeaderType(DInputSourceConverter->ReadUINT32());
        if(D32Bit){
            TempHeader.DFileOffset = DInputSourceConverter->ReadUINT32();
            TempHeader.DVirtualAddress = DInputSourceConverter->ReadUINT32();
            TempHeader.DPhysicalAddress = DInputSourceConverter->ReadUINT32();
            TempHeader.DFileSize = DInputSourceConverter->ReadUINT32();
            TempHeader.DMemorySize = DInputSourceConverter->ReadUINT32();
            TempHeader.DFlags = DInputSourceConverter->ReadUINT32();
            TempHeader.DAlignment = DInputSourceConverter->ReadUINT32();
        }
        else{
            TempHeader.DFlags = DInputSourceConverter->ReadUINT32();
            TempHeader.DFileOffset = DInputSourceConverter->ReadUINT64();
            TempHeader.DVirtualAddress = DInputSourceConverter->ReadUINT64();
            TempHeader.DPhysicalAddress = DInputSourceConverter->ReadUINT64();
            TempHeader.DFileSize = DInputSourceConverter->ReadUINT64();
            TempHeader.DMemorySize = DInputSourceConverter->ReadUINT64();
            TempHeader.DAlignment = DInputSourceConverter->ReadUINT64();
        }
        DProgramHeaders.push_back(TempHeader);
    }
    return true;
}


bool CElfLoad::ReadSectionHeaders(){
    DInputSourceConverter->Seek(DSectionHeaderOffset);
    for(uint16_t Index = 0; Index < DSectionHeaderEntryCount; Index++){
       CElfStructures::SSectionHeader TempHeader;
        TempHeader.DNameIndex = DInputSourceConverter->ReadUINT32();
        TempHeader.DType = CElfConstants::ESectionHeaderType(DInputSourceConverter->ReadUINT32());
        if(D32Bit){
            TempHeader.DFlags = DInputSourceConverter->ReadUINT32();
            TempHeader.DVirtualAddress = DInputSourceConverter->ReadUINT32();
            TempHeader.DFileOffset = DInputSourceConverter->ReadUINT32();
            TempHeader.DSize = DInputSourceConverter->ReadUINT32();
            TempHeader.DLink = DInputSourceConverter->ReadUINT32();
            TempHeader.DInfo = DInputSourceConverter->ReadUINT32();
            TempHeader.DAddressAlign = DInputSourceConverter->ReadUINT32();
            TempHeader.DEntrySize = DInputSourceConverter->ReadUINT32();
        }
        else{
            TempHeader.DFlags = DInputSourceConverter->ReadUINT64();
            TempHeader.DVirtualAddress = DInputSourceConverter->ReadUINT64();
            TempHeader.DFileOffset = DInputSourceConverter->ReadUINT64();
            TempHeader.DSize = DInputSourceConverter->ReadUINT64();
            TempHeader.DLink = DInputSourceConverter->ReadUINT32();
            TempHeader.DInfo = DInputSourceConverter->ReadUINT32();
            TempHeader.DAddressAlign = DInputSourceConverter->ReadUINT64();
            TempHeader.DEntrySize = DInputSourceConverter->ReadUINT64();
        }
        DSectionHeaders.push_back(TempHeader);
        if(TempHeader.DFlags == (CElfConstants::PF_W | CElfConstants::PF_R)){
            DGlobalVariableSectionIndices.insert(Index);
        }
    }
    return true;
}

bool CElfLoad::ReadSectionNames(){
    if(DSectionNameIndex < DSectionHeaders.size()){
        DInputSourceConverter->Seek(DSectionHeaders[DSectionNameIndex].DFileOffset);
        auto Size = DSectionHeaders[DSectionNameIndex].DSize;
        std::vector<char> Buffer(Size);
        DInputSourceConverter->Read(Buffer.data(),DSectionHeaders[DSectionNameIndex].DSize);
        DSectionNames.SetPayload(Buffer);
    }
    return true;
}

bool CElfLoad::ReadStringTables(){
    uint32_t SectionIndex = 0;
    for(auto &SectionHeader : DSectionHeaders){
        if(SectionHeader.DType == CElfConstants::SHT_STRTAB){ // SHT_STRTAB
            DInputSourceConverter->Seek(SectionHeader.DFileOffset);

            auto Size = SectionHeader.DSize;
            std::vector<char> Buffer(Size);
            DInputSourceConverter->Read((uint8_t *)Buffer.data(),SectionHeader.DSize);
            DStringTables[SectionIndex].SetPayload(Buffer);
        }
        SectionIndex++;
    }
    return true;
}

bool CElfLoad::ReadSymbolTables(){
    
    for(auto &SectionHeader : DSectionHeaders){
        if(SectionHeader.DType == CElfConstants::SHT_SYMTAB){ // SHT_SYMTAB
            DInputSourceConverter->Seek(SectionHeader.DFileOffset);
            for(uint64_t SectionOffset = 0; SectionOffset < SectionHeader.DSize; SectionOffset += SectionHeader.DEntrySize){
                CElfStructures::SSymbolEntity TempEntity;

                TempEntity.DNameIndex = DInputSourceConverter->ReadUINT32();
                if(D32Bit){
                    TempEntity.DAddress = DInputSourceConverter->ReadUINT32();
                    TempEntity.DSize = DInputSourceConverter->ReadUINT32();
                    TempEntity.DInfo = DInputSourceConverter->ReadUINT8();
                    TempEntity.DOther = DInputSourceConverter->ReadUINT8();
                    TempEntity.DSectionIndex = DInputSourceConverter->ReadUINT16();
                    TempEntity.DNameSectionIndex = SectionHeader.DLink;
                }
                else{
                    TempEntity.DInfo = DInputSourceConverter->ReadUINT8();
                    TempEntity.DOther = DInputSourceConverter->ReadUINT8();
                    TempEntity.DSectionIndex = DInputSourceConverter->ReadUINT16();
                    TempEntity.DAddress = DInputSourceConverter->ReadUINT64();
                    TempEntity.DSize = DInputSourceConverter->ReadUINT64();
                    TempEntity.DNameSectionIndex = SectionHeader.DLink;
                }
                DSymbolEntities.push_back(TempEntity);
            }
        }
    }
    return true;
}

bool CElfLoad::ReadDebugInfo(){
    auto AbbreviationsOffset = 0;
    auto LineNumberOffset = 0;
    std::unordered_map<std::string, uint64_t> GlobalSymbols;
    DDwarfProgram.DLittleEndian = DInputSourceConverter->LittleEndian();
    DDwarfProgram.DAbbreviationsSize = 0;
    DDwarfProgram.DLineNumberSize = 0;
    DDwarfProgram.DCompilaitonUnits.clear();
    DDwarfProgram.DLineNumberData.DFilePaths.clear();
    DDwarfProgram.DLineNumberData.DLineNumberEntries.clear();

    for(auto &SectionHeader : DSectionHeaders){
        auto SectionName = DSectionNames.GetString(SectionHeader.DNameIndex);
        if(SectionName == ".debug_line"){
            LineNumberOffset = SectionHeader.DFileOffset;
            DDwarfProgram.DLineNumberSize = SectionHeader.DSize;
        }
        if(SectionName == ".debug_abbrev"){
            AbbreviationsOffset = SectionHeader.DFileOffset;
            DDwarfProgram.DAbbreviationsSize = SectionHeader.DSize;
        }
    }
    if(!DDwarfProgram.DAbbreviationsSize || !DDwarfProgram.DLineNumberSize){
        return true; 
    }
    auto AbbreviationSandbox = std::make_shared< CSeekableDataSourceSandbox >(DInputSource, AbbreviationsOffset, DDwarfProgram.DAbbreviationsSize);
    DDwarfProgram.DAbbreviationsSourceConverter = std::make_shared< CSeekableDataSourceConverter >(AbbreviationSandbox);
    DDwarfProgram.DAbbreviationsSourceConverter->LittleEndian(DInputSourceConverter->LittleEndian());

    auto LineNumberSandbox = std::make_shared< CSeekableDataSourceSandbox >(DInputSource, LineNumberOffset, DDwarfProgram.DLineNumberSize);
    DDwarfProgram.DLineNumberSourceConverter = std::make_shared< CSeekableDataSourceConverter >(LineNumberSandbox);
    DDwarfProgram.DLineNumberSourceConverter->LittleEndian(DInputSourceConverter->LittleEndian());
    DDwarfProgram.D32Bit = D32Bit;
    DDwarfProgram.DDebugStrings = std::make_shared< CElfStructures::CStringTable >();
    for(auto &SectionHeader : DSectionHeaders){
        auto SectionName = DSectionNames.GetString(SectionHeader.DNameIndex);
        if(SectionName == ".debug_str"){
            std::vector<char> Buffer(SectionHeader.DSize);
            DInputSourceConverter->Seek(SectionHeader.DFileOffset);
            DInputSourceConverter->Read((uint8_t *)Buffer.data(),SectionHeader.DSize);
            DDwarfProgram.DDebugStrings->SetPayload(Buffer);
        }
    }
    DDwarfProgram.DDebugLineStrings = std::make_shared< CElfStructures::CStringTable >();
    for(auto &SectionHeader : DSectionHeaders){
        auto SectionName = DSectionNames.GetString(SectionHeader.DNameIndex);
        if(SectionName == ".debug_line_str"){
            std::vector<char> Buffer(SectionHeader.DSize);
            DInputSourceConverter->Seek(SectionHeader.DFileOffset);
            DInputSourceConverter->Read((uint8_t *)Buffer.data(),SectionHeader.DSize);
            DDwarfProgram.DDebugLineStrings->SetPayload(Buffer);
        }
    }
    for(auto &SectionHeader : DSectionHeaders){
        auto SectionName = DSectionNames.GetString(SectionHeader.DNameIndex);
        if(SectionName == ".debug_info"){
            uint64_t CurrentOffset = 0;
            auto DebugSourceSandbox = std::make_shared< CSeekableDataSourceSandbox >(DInputSource, SectionHeader.DFileOffset, SectionHeader.DSize);
            while(CurrentOffset < SectionHeader.DSize){
                DebugSourceSandbox->Seek(CurrentOffset);
                //auto CULength = ReadDebugCompilationUnit(DebugSourceSandbox);
                auto CULength = DDwarfProgram.ReadCompilationUnit(DebugSourceSandbox);
                if(!CULength){
                    break;
                }
                CurrentOffset += CULength;
            }
        }
    }
    for(auto &SymbolEntity : DSymbolEntities){
        auto Search = DStringTables.find(SymbolEntity.DNameSectionIndex);
        if(Search != DStringTables.end()){
            auto EntityName = Search->second.GetString(SymbolEntity.DNameIndex);
            if(!EntityName.empty()){
                GlobalSymbols[EntityName] = SymbolEntity.DAddress;
            }
        }
    }

    DDwarfProgram.ConsolidateLineNumbers();
    DDwarfProgram.ConsolidateVariables(GlobalSymbols);

    return true;
}

bool CElfLoad::MergeSymbols(){
    for(auto &SymbolEntity : DSymbolEntities){
        auto Search = DStringTables.find(SymbolEntity.DNameSectionIndex);
        std::string SymbolName;
        if(Search != DStringTables.end()){
            SymbolName = Search->second.GetString(SymbolEntity.DNameIndex);
        }
        size_t ProgramHeaderIndex = 0;
        while(ProgramHeaderIndex < DProgramHeaders.size()){
            auto &ProgramHeader = DProgramHeaders[ProgramHeaderIndex];
            if((ProgramHeader.DSegmentType == CElfConstants::PT_LOAD)&&(ProgramHeader.DFlags & CElfConstants::PF_X)&&(ProgramHeader.DVirtualAddress <= SymbolEntity.DAddress)&&(SymbolEntity.DAddress < ProgramHeader.DVirtualAddress + ProgramHeader.DMemorySize)){ // Loadable & Executable
                break;
            }
            ProgramHeaderIndex++;
        }
        if(ProgramHeaderIndex < DProgramHeaders.size()){
            auto &ProgramHeader = DProgramHeaders[ProgramHeaderIndex];
            if(SymbolEntity.DInfo == CElfConstants::SYMBOL_INFO(CElfConstants::STB_GLOBAL,CElfConstants::STT_FUNC)){ // Global function
                ProgramHeader.DSymbols[SymbolEntity.DAddress] = SymbolName;
            }
            else if(SymbolEntity.DInfo == CElfConstants::SYMBOL_INFO(CElfConstants::STB_GLOBAL,CElfConstants::STT_NOTYPE)){ // Global symbol
                if(ProgramHeader.DSymbols.find(SymbolEntity.DAddress) == ProgramHeader.DSymbols.end()){
                    ProgramHeader.DSymbols[SymbolEntity.DAddress] = SymbolName;
                }
            }
        }

        size_t SectionHeaderIndex = 0;
        while(SectionHeaderIndex < DSectionHeaders.size()){
            auto &SectionHeader = DSectionHeaders[SectionHeaderIndex];
            if((SectionHeader.DType == CElfConstants::SHT_PROGBITS)&&(SectionHeader.DFlags & CElfConstants::PF_R)&&(SectionHeader.DVirtualAddress <= SymbolEntity.DAddress)&&(SymbolEntity.DAddress < SectionHeader.DVirtualAddress + SectionHeader.DSize)){ // Loadable & Executable
                break;
            }
            SectionHeaderIndex++;
        }
        if(SectionHeaderIndex < DSectionHeaders.size()){
            auto &SectionHeader = DSectionHeaders[SectionHeaderIndex];
            if(SymbolEntity.DInfo == CElfConstants::SYMBOL_INFO(CElfConstants::STB_GLOBAL,CElfConstants::STT_FUNC)){ // Global function
                SectionHeader.DSymbols[SymbolEntity.DAddress] = SymbolName;
            }
            else if(SymbolEntity.DInfo == CElfConstants::SYMBOL_INFO(CElfConstants::STB_GLOBAL,CElfConstants::STT_NOTYPE)){ // Global symbol
                if(SectionHeader.DSymbols.find(SymbolEntity.DAddress) == SectionHeader.DSymbols.end()){
                    SectionHeader.DSymbols[SymbolEntity.DAddress] = SymbolName;
                }
            }
        }
    }
    return true;
}

bool CElfLoad::LoadProgramPayloads(){
    for(auto &Header : DProgramHeaders){
        DInputSourceConverter->Seek(Header.DFileOffset);
        Header.DPayload.resize(Header.DFileSize);
        DInputSourceConverter->Read(Header.DPayload.data(),Header.DFileSize);
    }
    return true;
}

bool CElfLoad::PrintDebugInformationEntry(std::shared_ptr< CDwarfStructures::SDie > die, int indent){
    std::string Indent;
    while(indent){
        Indent += " ";
        indent--;
    }
    printf("%sTAG x%02x\n", Indent.c_str(), int(die->DTag));
    printf("%sAttr:\n", Indent.c_str());
    for(auto &Attribute : die->DAttributes){
        printf("%s %02x ",Indent.c_str(), int(Attribute.first));
        PrintValue(Attribute.second);
        printf("\n");
    }
    if(!die->DChildren.empty()){
        printf("%sChildren:\n", Indent.c_str());
        for(auto Child : die->DChildren){
            PrintDebugInformationEntry(Child, indent+2);
        }
    }
    else{
        printf("\n");
    }
    return true;
}

bool CElfLoad::PrintValue(const CDwarfStructures::SValue &value){
    switch(value.DForm){
        case DW_FORM::string:
        case DW_FORM::strp:
        case DW_FORM::line_strp:    printf("%s",value.DData.data());
                                    return true;
        default:                    break;
    }
    bool First = true;
    for(auto Byte : value.DData){
        if(!First){
            printf(" ");
        }
        printf("%02x", int(Byte));
        First = false;
    }
    return true;
}