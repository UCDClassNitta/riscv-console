#include "ElfLoad.h"
#include <cstdio>
#include <cstring>
#include <cinttypes>


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