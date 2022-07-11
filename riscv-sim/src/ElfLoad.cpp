#include "ElfLoad.h"
#include <cstdio>
#include <cstring>
#include <cinttypes>
#include <algorithm>


std::string CElfLoad::SValue::GetString(){
    switch(DForm){
        case DW_FORM::string:
        case DW_FORM::strp:
        case DW_FORM::line_strp:    return (const char *)DData.data();
        default:                    return std::string();
    }
}

int64_t CElfLoad::SValue::GetINT64(){
    return -1;
}

uint64_t CElfLoad::SValue::GetUINT64(){
    switch(DForm){
        case DW_FORM::addr            :
        case DW_FORM::ref_addr        :
        case DW_FORM::sec_offset      :
        case DW_FORM::strp_sup        :

        case DW_FORM::data1           :
        case DW_FORM::data2           :
        case DW_FORM::data4           :
        case DW_FORM::data8           :

        case DW_FORM::ref1            :
        case DW_FORM::ref2            :
        case DW_FORM::ref4            :
        case DW_FORM::ref8            :

        case DW_FORM::flag            :
        
        case DW_FORM::strx1           :
        case DW_FORM::strx2           :
        case DW_FORM::strx3           :
        case DW_FORM::strx4           :

        case DW_FORM::addrx1          : 
        case DW_FORM::addrx2          : 
        case DW_FORM::addrx3          : 
        case DW_FORM::addrx4          :

        case DW_FORM::ref_sup4        : 
        case DW_FORM::ref_sup8        :

        case DW_FORM::ref_sig8        : 
        case DW_FORM::implicit_const  : {
                                            uint64_t ReturnValue = 0;
                                            for(auto ByteIter = DData.rbegin(); ByteIter != DData.rend(); ByteIter++){
                                                ReturnValue <<= 8;
                                                ReturnValue |= *ByteIter;
                                            }
                                            return ReturnValue;
                                        }
                                        break;
        case DW_FORM::sdata           :
        case DW_FORM::udata           :
        case DW_FORM::ref_udata       :
        case DW_FORM::strx            :
        case DW_FORM::addrx           :
        case DW_FORM::loclistx        :
        case DW_FORM::rnglistx        : {
                                            uint64_t ReturnValue = 0;
                                            for(auto ByteIter = DData.rbegin(); ByteIter != DData.rend(); ByteIter++){
                                                ReturnValue <<= 7;
                                                ReturnValue |= (*ByteIter) & 0x7F;
                                            }
                                            return ReturnValue;
                                        }
                                        break;
        default:                        break;
    }
    return 0;
}

void CElfLoad::SLineNumberStateMachine::Reset(){
    DRegs.address = DRegs.op_index = DRegs.column = DRegs.isa = DRegs.discriminator = 0;
    DRegs.file = DRegs.line = 1;
    DRegs.is_stmt = DDefaultIsStmt;
    DRegs.basic_block = DRegs.end_sequence = DRegs.prologue_end = DRegs.epilogue_begin = false;
}

void CElfLoad::SProgramHeader::Print(){
    printf("TYPE  0x%08" PRIX32 "\n",DSegmentType);
    printf("FLAGS 0x%08" PRIX32 "\n",DFlags);

    printf("FOFF  0x%08" PRIX64 "\n",DFileOffset);
    printf("VADDR 0x%08" PRIX64 "\n",DVirtualAddress);
    printf("PADDR 0x%08" PRIX64 "\n",DPhysicalAddress);
    printf("FSIZE 0x%08" PRIX64 "\n",DFileSize);
    printf("MSIZE 0x%08" PRIX64 "\n",DMemorySize);
    printf("ALIGN %" PRId64 "\n",DAlignment);
    for(auto &Symbol : DSymbols){
        printf("%s => 0x%08" PRIX64 "\n",Symbol.second.c_str(),Symbol.first);
    }
    printf("\n");
}

void CElfLoad::SSectionHeader::Print(const std::unordered_map< uint32_t, std::string > &names){
    auto Search = names.find(DNameIndex);
    if(Search != names.end()){
        printf("NAME  %s\n",Search->second.c_str());
    }
    else{
        printf("NAME  0x%08" PRIX32 "\n",DNameIndex);
    }
    printf("TYPE  0x%08" PRIX32 "\n",DType);
    printf("FLAGS 0x%08" PRIX64 "\n",DFlags);

    printf("FOFF  0x%08" PRIX64 "\n",DFileOffset);
    printf("VADDR 0x%08" PRIX64 "\n",DVirtualAddress);
    printf("FSIZE 0x%08" PRIX64 "\n",DSize);
    printf("LINK  0x%08" PRIX32 "\n",DLink);
    printf("INFO  0x%08" PRIX64 "\n",DInfo);
    printf("ESIZE %" PRId64 "\n",DEntrySize);
    printf("ALIGN %" PRId64 "\n\n",DAddressAlign);

}

void CElfLoad::SSymbolEntity::Print(const std::unordered_map< uint32_t,  std::vector< char > > &stringtables){
    auto Search = stringtables.find(DNameSectionIndex);
    if((Search != stringtables.end())&&(DNameIndex < Search->second.size())){
        printf("NAME  %s\n",Search->second.data() + DNameIndex);
    }
    else{
        printf("NAME  0x%08" PRIX32 "\n",DNameIndex);
    }
    printf("ADDR  0x%08" PRIX64 "\n",DAddress);
    printf("SIZE  0x%08" PRIX64 "\n",DSize);
    printf("INFO  0x%02" PRIX8 "\n",DInfo);
    printf("OTHER 0x%02" PRIX8 "\n",DOther);
    printf("SHNDX 0x%04" PRIX16 "\n\n",DSectionIndex);
}

CElfLoad::CElfLoad(std::shared_ptr< CDataSource > src){
    DInputSource = src;
    DBufferedPosition = 0;
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
    }

}

CElfLoad::~CElfLoad(){

    
}

void CElfLoad::ReadData(uint8_t *buf, size_t size){
    if(DBufferedPosition + size > DBufferedData.size()){
        auto OldSize =  DBufferedData.size();
        DBufferedData.resize(DBufferedPosition + size);
        DInputSource->Read(DBufferedData.data() + OldSize,DBufferedData.size() - OldSize);
    }
    memcpy(buf,DBufferedData.data() + DBufferedPosition, size);
    DBufferedPosition += size;
}

void CElfLoad::Seek(uint32_t pos){
    DBufferedPosition = pos;
}

uint32_t CElfLoad::CurrentPosition(){
    return DBufferedPosition;
}

size_t CElfLoad::ProgramHeaderCount() const{
    return DProgramHeaders.size();
}

const CElfLoad::SProgramHeader &CElfLoad::ProgramHeader(size_t index) const{
    return DProgramHeaders[index];
}

size_t CElfLoad::SectionHeaderCount() const{
    return DSectionHeaders.size();
}

const CElfLoad::SSectionHeader &CElfLoad::SectionHeader(size_t index) const{
    return DSectionHeaders[index];
}

void CElfLoad::PrintHeaders(){
    PrintHeader();
    printf("---- PROGRAM HEADERS -----\n");
    for(auto &Header : DProgramHeaders){
        Header.Print();
    }
    printf("---- SECTION HEADERS -----\n");
    for(auto &Header : DSectionHeaders){
        Header.Print(DSectionNames);
    }
    printf("---- SYMBOL ENTITIES -----\n");
    for(auto &Entity : DSymbolEntities){
        Entity.Print(DStringTables);
    }
    /*
    for(auto &StrKeyVal : DStrings){
        printf("%d,%d => %s\n",StrKeyVal.first.first,StrKeyVal.first.second,StrKeyVal.second.c_str());
    }
    */
}

bool CElfLoad::ValidateHeader(){
    uint8_t Buffer[16];

    ReadData(Buffer, 16);
    //printf("16B header read\n");
    if((Buffer[0] != 0x7F)||(Buffer[1] != 'E')||(Buffer[2] != 'L')||(Buffer[3] != 'F')){
        return false;
    }
    D32Bit = Buffer[4] == 1;
    DLittleEndian = Buffer[5] == 1;
    if(Buffer[6] != 1){
        return false;
    }
    DOSABI = Buffer[7];
    DObjectType = ReadUINT16();
    DMachine  = ReadUINT16();
    DELFVersion = ReadUINT32();
    DEntry = D32Bit ? ReadUINT32() : ReadUINT64();
    DProgramHeaderOffset = D32Bit ? ReadUINT32() : ReadUINT64();
    DSectionHeaderOffset = D32Bit ? ReadUINT32() : ReadUINT64();
    DFlags = ReadUINT32();
    DHeaderSize = ReadUINT16();
    DProgramHeaderEntrySize = ReadUINT16();
    DProgramHeaderEntryCount = ReadUINT16();
    DSectionHeaderEntrySize = ReadUINT16();
    DSectionHeaderEntryCount = ReadUINT16();
    DSectionNameIndex = ReadUINT16();

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

uint8_t CElfLoad::ReadUINT8(){
    uint8_t Buffer;
    ReadData(&Buffer,sizeof(Buffer));
    return Buffer;
}

uint16_t CElfLoad::ReadUINT16(){
    uint8_t Buffer[2];
    ReadData(Buffer,sizeof(Buffer));

    if(DLittleEndian){
        return (((uint16_t)Buffer[1])<<8) | Buffer[0];    
    }
    return (((uint16_t)Buffer[0])<<8) | Buffer[1];
}

uint32_t CElfLoad::ReadUINT32(){
    uint8_t Buffer[4];
    ReadData(Buffer,sizeof(Buffer));

    if(DLittleEndian){
        return (((uint32_t)Buffer[3])<<24) | (((uint32_t)Buffer[2])<<16) | (((uint32_t)Buffer[1])<<8) | Buffer[0];
    }
    return (((uint32_t)Buffer[0])<<24) | (((uint32_t)Buffer[1])<<16) | (((uint32_t)Buffer[2])<<8) | Buffer[3];
}

uint64_t CElfLoad::ReadUINT64(){
    uint8_t Buffer[8];
    ReadData(Buffer,sizeof(Buffer));

    if(DLittleEndian){
        return (((uint64_t)Buffer[7])<<56) | (((uint64_t)Buffer[6])<<48) | (((uint64_t)Buffer[5])<<40) | (((uint64_t)Buffer[4])<<32) | (((uint64_t)Buffer[3])<<24) | (((uint64_t)Buffer[2])<<16) | (((uint64_t)Buffer[1])<<8) | Buffer[0];
    }
    return (((uint64_t)Buffer[0])<<56) | (((uint64_t)Buffer[1])<<48) | (((uint64_t)Buffer[2])<<40) | (((uint64_t)Buffer[3])<<32) | (((uint64_t)Buffer[4])<<24) | (((uint64_t)Buffer[5])<<16) | (((uint64_t)Buffer[6])<<8) | Buffer[7];
}

uint64_t CElfLoad::ReadULEB128(){
    uint64_t Value = 0;
    int Offset = 0;
    do{
        uint8_t Temp = ReadUINT8();
        //printf("%02X ",Temp);
        Value += ((uint64_t)(Temp & 0x7F))<<Offset;
        Offset += 7;
        if((Temp & 0x80) == 0){
            break;
        }
    }while(true);
    //printf("\n");
    return Value;
}

int64_t CElfLoad::ReadSLEB128(){
    int64_t Value = 0;
    int Offset = 0;
    do{
        uint8_t Temp = ReadUINT8();
        Value += ((uint64_t)(Temp & 0x7F))<<Offset;
        Offset += 7;
        if((Temp & 0x80) == 0){
            if(Temp & 0x40){
                Value |= -(((int64_t)1)<<Offset);
            }
            break;
        }
    }while(true);
    return Value;
}

std::string CElfLoad::ReadFormString(DW_FORM form){
    
    switch(form){
        case DW_FORM::string:       {
                                        std::string ReturnString;
                                        while(true){
                                            char TempCh = ReadUINT8();
                                            if(TempCh){
                                                ReturnString += TempCh;
                                            }
                                            else{
                                                break;
                                            }
                                        }
                                        return ReturnString;
                                    }
        case DW_FORM::strp:         {
                                        uint64_t Offset = D32Bit ? ReadUINT32() : ReadUINT64();
                                        if(Offset < DDebugStrings.size()){
                                            return  (const char *)(DDebugStrings.data() + Offset);
                                        }
                                    }
                                    break;
        case DW_FORM::line_strp:    {
                                        uint64_t Offset = D32Bit ? ReadUINT32() : ReadUINT64();
                                        if(Offset < DDebugLineStrings.size()){
                                            return  (const char *)(DDebugLineStrings.data() + Offset);
                                        }
                                    }
                                    break;
        default:                    break;
    }
    return std::string();
}

bool CElfLoad::ReadProgramHeaders(){
    Seek(DProgramHeaderOffset);
    for(uint16_t Index = 0; Index < DProgramHeaderEntryCount; Index++){
        SProgramHeader TempHeader;
        TempHeader.DSegmentType = ReadUINT32();
        if(D32Bit){
            TempHeader.DFileOffset = ReadUINT32();
            TempHeader.DVirtualAddress = ReadUINT32();
            TempHeader.DPhysicalAddress = ReadUINT32();
            TempHeader.DFileSize = ReadUINT32();
            TempHeader.DMemorySize = ReadUINT32();
            TempHeader.DFlags = ReadUINT32();
            TempHeader.DAlignment = ReadUINT32();
        }
        else{
            TempHeader.DFlags = ReadUINT32();
            TempHeader.DFileOffset = ReadUINT64();
            TempHeader.DVirtualAddress = ReadUINT64();
            TempHeader.DPhysicalAddress = ReadUINT64();
            TempHeader.DFileSize = ReadUINT64();
            TempHeader.DMemorySize = ReadUINT64();
            TempHeader.DAlignment = ReadUINT64();
        }
        DProgramHeaders.push_back(TempHeader);
    }
    return true;
}


bool CElfLoad::ReadSectionHeaders(){
    Seek(DSectionHeaderOffset);
    for(uint16_t Index = 0; Index < DSectionHeaderEntryCount; Index++){
        SSectionHeader TempHeader;
        TempHeader.DNameIndex = ReadUINT32();
        TempHeader.DType = ReadUINT32();
        if(D32Bit){
            TempHeader.DFlags = ReadUINT32();
            TempHeader.DVirtualAddress = ReadUINT32();
            TempHeader.DFileOffset = ReadUINT32();
            TempHeader.DSize = ReadUINT32();
            TempHeader.DLink = ReadUINT32();
            TempHeader.DInfo = ReadUINT32();
            TempHeader.DAddressAlign = ReadUINT32();
            TempHeader.DEntrySize = ReadUINT32();
        }
        else{
            TempHeader.DFlags = ReadUINT64();
            TempHeader.DVirtualAddress = ReadUINT64();
            TempHeader.DFileOffset = ReadUINT64();
            TempHeader.DSize = ReadUINT64();
            TempHeader.DLink = ReadUINT32();
            TempHeader.DInfo = ReadUINT32();
            TempHeader.DAddressAlign = ReadUINT64();
            TempHeader.DEntrySize = ReadUINT64();
        }
        DSectionHeaders.push_back(TempHeader);
    }
    return true;
}

bool CElfLoad::ReadSectionNames(){
    if(DSectionNameIndex < DSectionHeaders.size()){
        Seek(DSectionHeaders[DSectionNameIndex].DFileOffset);
        auto Size = DSectionHeaders[DSectionNameIndex].DSize;
        std::vector<uint8_t> Buffer(Size);
        ReadData(Buffer.data(),DSectionHeaders[DSectionNameIndex].DSize);
        size_t Index = 0;
        while(Index < Size){
            std::string TempStr((char *)&Buffer[Index]);
            DSectionNames[Index] = TempStr;
            Index += TempStr.length() + 1;
        }
    }
    return true;
}

bool CElfLoad::ReadStringTables(){
    uint32_t SectionIndex = 0;
    for(auto &SectionHeader : DSectionHeaders){
        if(SectionHeader.DType == 0x03){ // SHT_STRTAB
            Seek(SectionHeader.DFileOffset);

            auto Size = SectionHeader.DSize;
            std::vector<char> Buffer(Size);
            ReadData((uint8_t *)Buffer.data(),SectionHeader.DSize);
            DStringTables[SectionIndex] = Buffer;
        }
        SectionIndex++;
    }
    return true;
}

bool CElfLoad::ReadSymbolTables(){
    
    for(auto &SectionHeader : DSectionHeaders){
        if(SectionHeader.DType == 0x02){ // SHT_SYMTAB
            Seek(SectionHeader.DFileOffset);
            for(uint64_t SectionOffset = 0; SectionOffset < SectionHeader.DSize; SectionOffset += SectionHeader.DEntrySize){
                SSymbolEntity TempEntity;

                TempEntity.DNameIndex = ReadUINT32();
                if(D32Bit){
                    TempEntity.DAddress = ReadUINT32();
                    TempEntity.DSize = ReadUINT32();
                    TempEntity.DInfo = ReadUINT8();
                    TempEntity.DOther = ReadUINT8();
                    TempEntity.DSectionIndex = ReadUINT16();
                    TempEntity.DNameSectionIndex = SectionHeader.DLink;
                }
                else{
                    TempEntity.DInfo = ReadUINT8();
                    TempEntity.DOther = ReadUINT8();
                    TempEntity.DSectionIndex = ReadUINT16();
                    TempEntity.DAddress = ReadUINT64();
                    TempEntity.DSize = ReadUINT64();
                    TempEntity.DNameSectionIndex = SectionHeader.DLink;
                }
                DSymbolEntities.push_back(TempEntity);
            }
        }
    }
    return true;
}

bool CElfLoad::ReadDebugInfo(){
    DAbbreviationsOffset = 0;
    DAbbreviationsSize = 0;
    DLineNumberOffset = 0;
    DLineNumberSize = 0;
    DDebugStrings.clear();
    DDebugLineStrings.clear();
    DCompilaitonUnits.clear();
    DLineNumberData.DFilePaths.clear();
    DLineNumberData.DLineNumberEntries.clear();

    for(auto &SectionHeader : DSectionHeaders){
        auto Search = DSectionNames.find(SectionHeader.DNameIndex);
        if(Search != DSectionNames.end() && Search->second == ".debug_line"){
            DLineNumberOffset = SectionHeader.DFileOffset;
            DLineNumberSize = SectionHeader.DSize;
        }
        if(Search != DSectionNames.end() && Search->second == ".debug_abbrev"){
            DAbbreviationsOffset = SectionHeader.DFileOffset;
            DAbbreviationsSize = SectionHeader.DSize;
        }
    }
    if(!DAbbreviationsSize || !DLineNumberSize){
        return true; 
    }
    for(auto &SectionHeader : DSectionHeaders){
        auto Search = DSectionNames.find(SectionHeader.DNameIndex);
        if(Search != DSectionNames.end() && Search->second == ".debug_str"){
            DDebugStrings.resize(SectionHeader.DSize);
            Seek(SectionHeader.DFileOffset);
            ReadData((uint8_t *)DDebugStrings.data(),SectionHeader.DSize);
            size_t Offset = 0;
            while(Offset < DDebugStrings.size()){
                Offset += strlen(DDebugStrings.data() + Offset) + 1;
            }
        }
    }
    for(auto &SectionHeader : DSectionHeaders){
        auto Search = DSectionNames.find(SectionHeader.DNameIndex);
        if(Search != DSectionNames.end() && Search->second == ".debug_line_str"){
            DDebugLineStrings.resize(SectionHeader.DSize);
            Seek(SectionHeader.DFileOffset);
            ReadData((uint8_t *)DDebugLineStrings.data(),SectionHeader.DSize);
            size_t Offset = 0;
            while(Offset < DDebugLineStrings.size()){
                Offset += strlen(DDebugLineStrings.data() + Offset) + 1;
            }
        }
    }
    for(auto &SectionHeader : DSectionHeaders){
        auto Search = DSectionNames.find(SectionHeader.DNameIndex);
        if(Search != DSectionNames.end() && Search->second == ".debug_info"){
            auto CurrentOffset = SectionHeader.DFileOffset;
            auto SectionEnd = CurrentOffset + SectionHeader.DSize;
            while(CurrentOffset < SectionEnd){
                auto CULength = ReadDebugCompilationUnit(CurrentOffset);
                if(!CULength){
                    break;
                }
                CurrentOffset += CULength;
            }
        }
    }
    ConsolidateLineNumbers();

    return true;
}

bool CElfLoad::ReadDebugAbbreviationTable(uint64_t offset, std::unordered_map< uint64_t, std::shared_ptr< SAbbreviationEntry > > &abbrev){
    auto SectionEnd = DAbbreviationsOffset + DAbbreviationsSize;
    auto PreviousOffset = CurrentPosition();
    Seek(DAbbreviationsOffset + offset);
    while(CurrentPosition() < SectionEnd){
        uint64_t Code = ReadULEB128(); 
        if(!Code){
            Seek(PreviousOffset);
            return true;
        }
        uint64_t Tag = ReadULEB128();
        uint8_t Children = ReadUINT8();
        std::shared_ptr< SAbbreviationEntry > NewEntry = std::make_shared< SAbbreviationEntry >();
        NewEntry->DCode = Code;
        NewEntry->DTag = DW_TAG(Tag);
        NewEntry->DChildren = Children;
        while(CurrentPosition() < SectionEnd){
            uint64_t Name = ReadULEB128();
            uint64_t Form = ReadULEB128();
            int64_t Extra = 0;
            if(!Name && !Form){
                break;
            }
            if(DW_FORM(Form) == DW_FORM::implicit_const){
                Extra = ReadSLEB128();
            }
            NewEntry->DAttributes.push_back({DW_AT(Name),DW_FORM(Form),Extra});
        }
        abbrev[Code] = NewEntry;
    }
    Seek(PreviousOffset);
    return true;
}

bool CElfLoad::ReadDebugInformationEntry(std::shared_ptr< SDie > &die){
    auto Code = ReadULEB128();
    if(!Code){
        die.reset();
        return true;
    }
    if(auto CompilationUnit = die->DCompilationUnit.lock()){
        auto Search = CompilationUnit->DAbbreviationTable.find(Code);
        if(Search == CompilationUnit->DAbbreviationTable.end()){
            // Error 
            printf("Unknown abbreviation code 0x%08" PRIX64 "!\n", Code);
            return false;
        }
        auto Abbreviation = Search->second;
        die->DTag = Abbreviation->DTag;
        die->DAttributeEntry = Abbreviation;
        for(auto &Attribute : Abbreviation->DAttributes){
            SValue Value = ReadDebugValue(Attribute.DForm, CompilationUnit, Attribute.DExtra);
            if((Value.DForm != Attribute.DForm)&&(DW_FORM::indirect != Attribute.DForm)){
                // Error
                return false;
            }
            die->DAttributes[Attribute.DName] = Value;
        }
        if(Abbreviation->DChildren){
            while(1){
                auto NewChild = std::make_shared<SDie>();
                NewChild->DCompilationUnit = CompilationUnit;
                if(!ReadDebugInformationEntry(NewChild)){
                    return false;
                }
                if(!NewChild){
                    break;
                }
                die->DChildren.push_back(NewChild);
            }
        }
        return true;
    }
    return false;
}

bool CElfLoad::ReadDebugLineNumbers(uint64_t offset, std::shared_ptr<SCompilationUnit> cu){
    auto SectionEnd = DLineNumberOffset + DLineNumberSize;
    uint64_t LNSize;
    Seek(DLineNumberOffset + offset);
    LNSize = ReadUINT32();
    if(D32Bit){
        if(LNSize >= 0xFFFFFFF0){
            return false;
        }
    }
    else{
        if(LNSize != 0xFFFFFFFF){
            return false;
        }
        LNSize = ReadUINT64();
    }
    uint64_t ProgramEnd = CurrentPosition() + LNSize;
    SLineNumberStateMachine LineNumberStateMachine; 
    auto Version = ReadUINT16();
    if(Version != 5){
        return false;
    }
    LineNumberStateMachine.DAddressSize = ReadUINT8();
    LineNumberStateMachine.DSegmentSelectorSize = ReadUINT8();
    uint64_t HeaderLength = D32Bit ? ReadUINT32() : ReadUINT64();
    auto HeaderEnd = CurrentPosition() + HeaderLength;
    if(HeaderEnd > SectionEnd){
        return false;
    }
    LineNumberStateMachine.DMinimumInstructionLength = ReadUINT8();
    LineNumberStateMachine.DMaximumOperandsPerInstruction = ReadUINT8();
    LineNumberStateMachine.DDefaultIsStmt = ReadUINT8();
    LineNumberStateMachine.DLineBase = int8_t(ReadUINT8());
    LineNumberStateMachine.DLineRange = ReadUINT8();
    LineNumberStateMachine.DOpcodeBase = ReadUINT8();
    for(uint8_t OpcodeIndex = 1; OpcodeIndex < LineNumberStateMachine.DOpcodeBase; OpcodeIndex++){
        LineNumberStateMachine.DOpcodeLengths.push_back(ReadUINT8());
    }
    auto DirectoryEntryFormatCount = ReadUINT8();
    std::vector<std::pair<DW_LNCT,DW_FORM>> DirectoryEntryFormat;
    for(uint8_t DirectoryEntryFormatIndex = 0; DirectoryEntryFormatIndex < DirectoryEntryFormatCount; DirectoryEntryFormatIndex++){
        auto ContentType = ReadULEB128();
        auto Attribute = ReadULEB128();
        DirectoryEntryFormat.push_back({DW_LNCT(ContentType),DW_FORM(Attribute)});
    }
    auto DirectoriesCount = ReadULEB128();
    std::vector< std::unordered_map<DW_LNCT,SValue> > DirectoryEntries;
    for(uint64_t DirectoryIndex = 0; DirectoryIndex < DirectoriesCount; DirectoryIndex++){
        std::unordered_map<DW_LNCT,SValue> Entry;
        for(auto Format : DirectoryEntryFormat){
            Entry[Format.first] = ReadDebugValue(Format.second,cu,0);
        }
        DirectoryEntries.push_back(Entry);
    }
    auto FileNameEntryFormatCount = ReadUINT8();
    std::vector<std::pair<DW_LNCT,DW_FORM>> FileNameEntryFormat;
    for(uint8_t FileNameEntryFormatIndex = 0; FileNameEntryFormatIndex < FileNameEntryFormatCount; FileNameEntryFormatIndex++){
        auto ContentType = ReadULEB128();
        auto Attribute = ReadULEB128();
        FileNameEntryFormat.push_back({DW_LNCT(ContentType),DW_FORM(Attribute)});
    }
    auto FileNamesCount = ReadULEB128();
    std::vector< std::unordered_map<DW_LNCT,SValue> > FileNameEntries;
    for(uint64_t FileNamesIndex = 0; FileNamesIndex < FileNamesCount; FileNamesIndex++){
        std::unordered_map<DW_LNCT,SValue> Entry;
        for(auto Format : FileNameEntryFormat){
            Entry[Format.first] = ReadDebugValue(Format.second,cu,0);
        }
        FileNameEntries.push_back(Entry);
    }
    ResolvePaths(cu,DirectoryEntries,FileNameEntries);
    LineNumberStateMachine.Reset();
    while(CurrentPosition() < ProgramEnd){
        if(ComputeLineNumberStep(LineNumberStateMachine)){
            // Add line number data
            SLineNumberEntry LineNumberEntry = {LineNumberStateMachine.DRegs.file, LineNumberStateMachine.DRegs.line, LineNumberStateMachine.DRegs.column, uint32_t(LineNumberStateMachine.DRegs.address)};
            cu->DLineNumberData.DLineNumberEntries.push_back(LineNumberEntry);
        }
        if(LineNumberStateMachine.DRegs.end_sequence){
            LineNumberStateMachine.Reset();
        }
    }

    return true;
}

bool CElfLoad::ConsolidateLineNumbers(){
    std::unordered_map< std::string, uint32_t > FileIndices;

    for(auto CompilationUnit : DCompilaitonUnits){
        std::unordered_map< uint32_t, uint32_t > FileTranslations;

        for(auto LineNumberEntry : CompilationUnit->DLineNumberData.DLineNumberEntries){
            if(FileTranslations.find(LineNumberEntry.DFileIndex) == FileTranslations.end()){
                 auto FilePath = CompilationUnit->DLineNumberData.DFilePaths[LineNumberEntry.DFileIndex];
                if(FileIndices.find(FilePath) == FileIndices.end()){
                    FileIndices[FilePath] = DLineNumberData.DFilePaths.size();
                    FileTranslations[LineNumberEntry.DFileIndex] = DLineNumberData.DFilePaths.size();
                    DLineNumberData.DFilePaths.push_back(FilePath);
                }
                else{
                    FileTranslations[LineNumberEntry.DFileIndex] = FileIndices[FilePath];
                }
            }

            auto FileIndex = FileTranslations[LineNumberEntry.DFileIndex];
            DLineNumberData.DLineNumberEntries.push_back({FileIndex,LineNumberEntry.DLineNumber,LineNumberEntry.DColumn,LineNumberEntry.DAddress});
        }
    }
    std::sort(DLineNumberData.DLineNumberEntries.begin(),DLineNumberData.DLineNumberEntries.end(),[](const SLineNumberEntry &a, const SLineNumberEntry &b) -> bool{
        if(a.DAddress < b.DAddress){
            return true;
        }
        if(a.DAddress > b.DAddress){
            return false;
        }
        if(a.DFileIndex < b.DFileIndex){
            return true;
        }
        if(a.DFileIndex > b.DFileIndex){
            return false;
        }
        if(a.DLineNumber < b.DLineNumber){
            return true;
        }
        if(a.DLineNumber > b.DLineNumber){
            return false;
        }
        return a.DColumn < b.DColumn;
    });
    return true;
}

bool CElfLoad::ResolvePaths(std::shared_ptr<SCompilationUnit> cu, std::vector< std::unordered_map<DW_LNCT,SValue> > &directories, std::vector< std::unordered_map<DW_LNCT,SValue> > &files){
    std::string CompilationDirectory;
    std::vector<std::string> ResolvedDirectories;
    if(cu->DRoot->HasAttribute(DW_AT::comp_dir)){
        CompilationDirectory = cu->DRoot->GetAttribute(DW_AT::comp_dir).GetString();
        if(CompilationDirectory.back() != '/'){
            CompilationDirectory += "/";
        }
    }
    else{
        CompilationDirectory = "./";
    }
    for(auto &DirectoryEntry : directories){
        auto Path = DirectoryEntry[DW_LNCT::path].GetString();
        if(Path.front() != '/'){
            Path = CompilationDirectory + Path;
        }
        if(Path.back() != '/'){
            Path += "/";
        }
        ResolvedDirectories.push_back(Path);
    }
    for(auto &FileEntry : files){
        auto Path = FileEntry[DW_LNCT::path].GetString();
        if(FileEntry.find(DW_LNCT::directory_index) != FileEntry.end()){
            auto DirIndex = FileEntry[DW_LNCT::directory_index].GetUINT64();
            Path = ResolvedDirectories[DirIndex] + Path;
        }
        cu->DLineNumberData.DFilePaths.push_back(Path);
    }
    return true;
}

bool CElfLoad::ComputeLineNumberStep(SLineNumberStateMachine &machine){
    auto Opcode = ReadUINT8();
    if(!Opcode){
        // Extended opcode
        uint64_t Length = ReadULEB128();
        uint64_t InstructionEnd = CurrentPosition() + Length;
        Opcode = ReadUINT8();
        switch(DW_LNE(Opcode)){
            case DW_LNE::end_sequence:      machine.DRegs.end_sequence = true;
                                            if(Length != 1){
                                                // Error
                                            }
                                            return true;
            case DW_LNE::set_address:       machine.DRegs.address = D32Bit ? ReadUINT32() : ReadUINT64();
                                            machine.DRegs.op_index = 0;
                                            break;
            case DW_LNE::set_discriminator: machine.DRegs.discriminator = ReadULEB128();
                                            break;
            default:                        //ERROR
                                            break;
        }
        if(InstructionEnd != CurrentPosition()){
            // Error
        }
    }
    else if(Opcode < machine.DOpcodeBase){
        // Standard opcode
        uint32_t OperationAdvance = (255 - machine.DOpcodeBase) / machine.DLineRange;
        switch(DW_LNS(Opcode)){
            case DW_LNS::copy                :  machine.DRegs.basic_block = machine.DRegs.prologue_end = machine.DRegs.epilogue_begin = false;
                                                machine.DRegs.discriminator = 0;
                                                return true;
            case DW_LNS::advance_pc          :  OperationAdvance = ReadULEB128();
            case DW_LNS::const_add_pc        :  machine.DRegs.address += machine.DMinimumInstructionLength * ((machine.DRegs.op_index + OperationAdvance) / machine.DMaximumOperandsPerInstruction);
                                                machine.DRegs.op_index = (machine.DRegs.op_index + OperationAdvance) % machine.DMaximumOperandsPerInstruction;
                                                break;
            case DW_LNS::advance_line        :  machine.DRegs.line += ReadSLEB128();
                                                break;
            case DW_LNS::set_file            :  machine.DRegs.file = ReadULEB128();
                                                break;
            case DW_LNS::set_column          :  machine.DRegs.column = ReadULEB128();
                                                break;
            case DW_LNS::negate_stmt         :  machine.DRegs.is_stmt = !machine.DRegs.is_stmt;
                                                break;
            case DW_LNS::set_basic_block     :  machine.DRegs.basic_block = true;
                                                break;
            case DW_LNS::fixed_advance_pc    :  machine.DRegs.address += ReadUINT16();
                                                break;
            case DW_LNS::set_prologue_end    :  machine.DRegs.prologue_end = true;
                                                break;
            case DW_LNS::set_epilogue_begin  :  machine.DRegs.epilogue_begin = true;
                                                break;
            case DW_LNS::set_isa             :  machine.DRegs.isa = ReadULEB128();
                                                break;
        }
    }
    else{
        // Special opcode
        uint8_t AdjustedOpcode = Opcode - machine.DOpcodeBase;
        uint8_t OperationAdvance = AdjustedOpcode / machine.DLineRange;
        int8_t LineIncrement = machine.DLineBase + (AdjustedOpcode % machine.DLineRange);
        machine.DRegs.line = machine.DRegs.line + LineIncrement;
        machine.DRegs.address += machine.DMinimumInstructionLength * ((machine.DRegs.op_index + OperationAdvance) / machine.DMaximumOperandsPerInstruction);
        machine.DRegs.op_index = (machine.DRegs.op_index + OperationAdvance) % machine.DMaximumOperandsPerInstruction;
        machine.DRegs.basic_block = machine.DRegs.prologue_end = machine.DRegs.epilogue_begin = false;
        machine.DRegs.discriminator = 0;
        return true;
    }
    return false;
}

uint64_t CElfLoad::ReadDebugCompilationUnit(uint64_t offset){
    std::shared_ptr< SCompilationUnit > NewCompUnit;
    uint64_t CUSize;
    uint64_t LengthSize;
    Seek(offset);
    CUSize = ReadUINT32();
    if(D32Bit){
        if(CUSize >= 0xFFFFFFF0){
            return 0;
        }
        LengthSize = 4;
    }
    else{
        if(CUSize != 0xFFFFFFFF){
            return 0;
        }
        CUSize = ReadUINT64();
        LengthSize = 12;
    }
    NewCompUnit = std::make_shared< SCompilationUnit >();
    NewCompUnit->DOffset = offset;
    NewCompUnit->DLength = CUSize;
    NewCompUnit->DVersion = ReadUINT16();
    NewCompUnit->DType = DW_UT(ReadUINT8());
    if((NewCompUnit->DType != DW_UT::compile)&&(NewCompUnit->DType != DW_UT::partial)){
        return CUSize + LengthSize;
    }
    NewCompUnit->DAddressSize = ReadUINT8();
    NewCompUnit->DAbbreviationOffset = D32Bit ? ReadUINT32() : ReadUINT64();
    
    ReadDebugAbbreviationTable(NewCompUnit->DAbbreviationOffset,NewCompUnit->DAbbreviationTable);
    NewCompUnit->DRoot = std::make_shared<SDie>();
    NewCompUnit->DRoot->DCompilationUnit = NewCompUnit;
    ReadDebugInformationEntry(NewCompUnit->DRoot);
    if(NewCompUnit->DRoot->HasAttribute(DW_AT::stmt_list)){
        ReadDebugLineNumbers(NewCompUnit->DRoot->GetAttribute(DW_AT::stmt_list).GetUINT64(),NewCompUnit);
    }
    
    DCompilaitonUnits.push_back(NewCompUnit);
    return CUSize + LengthSize;
}

CElfLoad::SValue CElfLoad::ReadDebugValue(DW_FORM form, std::shared_ptr<SCompilationUnit> cu, int64_t implicit){
    SValue ReturnValue;
    size_t ReadSize = 0;
    DW_FORM ReturnForm = form;
    switch(form){
        case DW_FORM::addr            : ReadSize = cu->DAddressSize;
                                        break;
        case DW_FORM::block1          : ReadSize = ReadUINT8();
                                        break;
        case DW_FORM::block2          : ReadSize = ReadUINT16();
                                        break;
        case DW_FORM::block4          : ReadSize = ReadUINT32();
                                        break;
        case DW_FORM::block           :
        case DW_FORM::exprloc         : ReadSize = ReadULEB128();
                                        break;
        case DW_FORM::data1           :
        case DW_FORM::flag            :
        case DW_FORM::ref1            :
        case DW_FORM::strx1           :
        case DW_FORM::addrx1          : ReadSize = 1;
                                        break;
        case DW_FORM::data2           :
        case DW_FORM::ref2            :
        case DW_FORM::strx2           :
        case DW_FORM::addrx2          : ReadSize = 2;
                                        break;
        case DW_FORM::strx3           :
        case DW_FORM::addrx3          : ReadSize = 3;
                                        break;
        case DW_FORM::data4           :
        case DW_FORM::ref4            :
        case DW_FORM::strx4           :
        case DW_FORM::addrx4          :
        case DW_FORM::ref_sup4        : ReadSize = 4;
                                        break;
        case DW_FORM::data8           :
        case DW_FORM::ref8            :
        case DW_FORM::ref_sup8        :
        case DW_FORM::ref_sig8        : ReadSize = 8;
                                        break;
        case DW_FORM::data16          : ReadSize  = 16;
                                        break;
        case DW_FORM::string          : while(1){
                                            auto NextChar = ReadUINT8();
                                            ReturnValue.DData.push_back(NextChar);
                                            if(!NextChar){
                                                break;
                                            }
                                        }
                                        break;
        case DW_FORM::ref_addr        :
        case DW_FORM::sec_offset      :
        case DW_FORM::strp_sup        : ReadSize = D32Bit ? 4 : 8;
                                        break;
        case DW_FORM::sdata           :
        case DW_FORM::udata           :
        case DW_FORM::ref_udata       :
        case DW_FORM::strx            :
        case DW_FORM::addrx           :
        case DW_FORM::loclistx        :
        case DW_FORM::rnglistx        : while(1){
                                            auto NextChar = ReadUINT8();
                                            ReturnValue.DData.push_back(NextChar);
                                            if(0 == (NextChar & 0x80)){
                                                break;
                                            }
                                        }
                                        break;
        case DW_FORM::strp            : {
                                            uint64_t Offset = D32Bit ? ReadUINT32() : ReadUINT64();
                                            while(Offset < DDebugStrings.size()){
                                                ReturnValue.DData.push_back(DDebugStrings[Offset]);
                                                if(!DDebugStrings[Offset]){
                                                    break;
                                                }
                                                Offset++;
                                            }
                                        }
                                        break;
        case DW_FORM::line_strp       : {
                                            uint64_t Offset = D32Bit ? ReadUINT32() : ReadUINT64();
                                            while(Offset < DDebugLineStrings.size()){
                                                ReturnValue.DData.push_back(DDebugLineStrings[Offset]);
                                                if(!DDebugLineStrings[Offset]){
                                                    break;
                                                }
                                                Offset++;
                                            }
                                        }
                                        break;
        case DW_FORM::indirect        : return ReadDebugValue(DW_FORM(ReadULEB128()), cu, 0);
        
        case DW_FORM::flag_present    : break;        
        case DW_FORM::implicit_const  : for(size_t Index = 0; Index < sizeof(int64_t); Index++){
                                            ReturnValue.DData.push_back(implicit & 0xFF);
                                            implicit >>= 8;
                                        }
                                        break;
    }
    ReturnValue.DForm = ReturnForm;
    if(ReadSize){
        ReturnValue.DData.resize(ReadSize);
        ReadData(ReturnValue.DData.data(),ReadSize);
    }
    return ReturnValue;
}

bool CElfLoad::MergeSymbols(){
    for(auto &SymbolEntity : DSymbolEntities){
        auto Search = DStringTables.find(SymbolEntity.DNameSectionIndex);
        std::string SymbolName;
        if((Search != DStringTables.end())&&(SymbolEntity.DNameIndex < Search->second.size())){
            SymbolName = Search->second.data() + SymbolEntity.DNameIndex;
        }
        size_t ProgramHeaderIndex = 0;
        while(ProgramHeaderIndex < DProgramHeaders.size()){
            auto &ProgramHeader = DProgramHeaders[ProgramHeaderIndex];
            if((ProgramHeader.DSegmentType == 1)&&(ProgramHeader.DFlags & 1)&&(ProgramHeader.DVirtualAddress <= SymbolEntity.DAddress)&&(SymbolEntity.DAddress < ProgramHeader.DVirtualAddress + ProgramHeader.DMemorySize)){ // Loadable & Executable
                break;
            }
            ProgramHeaderIndex++;
        }
        if(ProgramHeaderIndex < DProgramHeaders.size()){
            auto &ProgramHeader = DProgramHeaders[ProgramHeaderIndex];
            if(SymbolEntity.DInfo == 0x12){ // Global function
                ProgramHeader.DSymbols[SymbolEntity.DAddress] = SymbolName;
            }
            else if(SymbolEntity.DInfo == 0x10){ // Global symbol
                if(ProgramHeader.DSymbols.find(SymbolEntity.DAddress) == ProgramHeader.DSymbols.end()){
                    ProgramHeader.DSymbols[SymbolEntity.DAddress] = SymbolName;
                }
            }
        }

        size_t SectionHeaderIndex = 0;
        while(SectionHeaderIndex < DSectionHeaders.size()){
            auto &SectionHeader = DSectionHeaders[SectionHeaderIndex];
            if((SectionHeader.DType == 1)&&(SectionHeader.DFlags & 0x4)&&(SectionHeader.DVirtualAddress <= SymbolEntity.DAddress)&&(SymbolEntity.DAddress < SectionHeader.DVirtualAddress + SectionHeader.DSize)){ // Loadable & Executable
                break;
            }
            SectionHeaderIndex++;
        }
        if(SectionHeaderIndex < DSectionHeaders.size()){
            auto &SectionHeader = DSectionHeaders[SectionHeaderIndex];
            if(SymbolEntity.DInfo == 0x12){ // Global function
                SectionHeader.DSymbols[SymbolEntity.DAddress] = SymbolName;
            }
            else if(SymbolEntity.DInfo == 0x10){ // Global symbol
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
        Seek(Header.DFileOffset);
        Header.DPayload.resize(Header.DFileSize);
        ReadData(Header.DPayload.data(),Header.DFileSize);
    }
    return true;
}

bool CElfLoad::PrintDebugInformationEntry(std::shared_ptr< SDie > die, int indent){
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

bool CElfLoad::PrintValue(const SValue &value){
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