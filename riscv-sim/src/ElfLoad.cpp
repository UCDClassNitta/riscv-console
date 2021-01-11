#include "ElfLoad.h"
#include <cstdio>
#include <cstring>

void CElfLoad::CProgramHeader::Print(){
    printf("TYPE  0x%08X\n",DSegmentType);
    printf("FLAGS 0x%08X\n",DFlags);

    printf("FOFF  0x%08lX\n",DFileOffset);
    printf("VADDR 0x%08lX\n",DVirtualAddress);
    printf("PADDR 0x%08lX\n",DPhysicalAddress);
    printf("FSIZE 0x%08lX\n",DFileSize);
    printf("MSIZE 0x%08lX\n",DMemorySize);
    printf("ALIGN %ld\n",DAlignment);
}

CElfLoad::CElfLoad(std::shared_ptr< CDataSource > src){
    DInputSource = src;
    DBufferedPosition = 0;
    DValidFile = false;
    if(ValidateHeader()){
        if(ReadProgramHeaders()){
            LoadProgramPayloads();
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

const CElfLoad::CProgramHeader &CElfLoad::ProgramHeader(size_t index) const{
    return DProgramHeaders[index];
}

void CElfLoad::PrintHeaders(){
    PrintHeader();
    for(auto &Header : DProgramHeaders){
        Header.Print();
    }
}

bool CElfLoad::ValidateHeader(){
    uint8_t Buffer[16];

    ReadData(Buffer, 16);
    printf("16B header read\n");
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
    printf("Entry 0x%08lX\n",DEntry);
    printf("PHOFF 0x%08lX\n",DProgramHeaderOffset);
    printf("SHOFF 0x%08lX\n",DSectionHeaderOffset);
    printf("HDSZ  %hu\n",DHeaderSize);
    printf("PHES  %hu\n",DProgramHeaderEntrySize);
    printf("PHEC  %hu\n",DProgramHeaderEntryCount);
    printf("SHES  %hu\n",DSectionHeaderEntrySize);
    printf("SHEC  %hu\n",DSectionHeaderEntryCount);
    printf("SNID  %hu\n",DSectionNameIndex);
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
        CProgramHeader TempHeader;
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

bool CElfLoad::LoadProgramPayloads(){
    for(auto &Header : DProgramHeaders){
        Seek(Header.DFileOffset);
        Header.DPayload.resize(Header.DFileSize);
        ReadData(Header.DPayload.data(),Header.DFileSize);
    }
    return true;
}