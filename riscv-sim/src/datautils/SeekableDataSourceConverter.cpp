#include "SeekableDataSourceConverter.h"

CSeekableDataSourceConverter::CSeekableDataSourceConverter(std::shared_ptr<CSeekableDataSource> source, bool littleendian) : DActualSource(source), DLittleEndian(littleendian){
}


bool CSeekableDataSourceConverter::LittleEndian() const{
    return DLittleEndian;
}

bool CSeekableDataSourceConverter::LittleEndian(bool littleendian){
    return DLittleEndian = littleendian;
}

int CSeekableDataSourceConverter::Read(void *data, int length){
    return DActualSource->Read(data,length);
}

uint8_t CSeekableDataSourceConverter::ReadUINT8(){
    uint8_t Buffer;
    Read(&Buffer,sizeof(Buffer));
    return Buffer;
}

uint16_t CSeekableDataSourceConverter::ReadUINT16(){
    uint8_t Buffer[2];
    Read(Buffer,sizeof(Buffer));

    if(DLittleEndian){
        return (((uint16_t)Buffer[1])<<8) | Buffer[0];    
    }
    return (((uint16_t)Buffer[0])<<8) | Buffer[1];
}

uint32_t CSeekableDataSourceConverter::ReadUINT32(){
    uint8_t Buffer[4];
    Read(Buffer,sizeof(Buffer));

    if(DLittleEndian){
        return (((uint32_t)Buffer[3])<<24) | (((uint32_t)Buffer[2])<<16) | (((uint32_t)Buffer[1])<<8) | Buffer[0];
    }
    return (((uint32_t)Buffer[0])<<24) | (((uint32_t)Buffer[1])<<16) | (((uint32_t)Buffer[2])<<8) | Buffer[3];
}

uint64_t CSeekableDataSourceConverter::ReadUINT64(){
    uint8_t Buffer[8];
    Read(Buffer,sizeof(Buffer));

    if(DLittleEndian){
        return (((uint64_t)Buffer[7])<<56) | (((uint64_t)Buffer[6])<<48) | (((uint64_t)Buffer[5])<<40) | (((uint64_t)Buffer[4])<<32) | (((uint64_t)Buffer[3])<<24) | (((uint64_t)Buffer[2])<<16) | (((uint64_t)Buffer[1])<<8) | Buffer[0];
    }
    return (((uint64_t)Buffer[0])<<56) | (((uint64_t)Buffer[1])<<48) | (((uint64_t)Buffer[2])<<40) | (((uint64_t)Buffer[3])<<32) | (((uint64_t)Buffer[4])<<24) | (((uint64_t)Buffer[5])<<16) | (((uint64_t)Buffer[6])<<8) | Buffer[7];
}

uint64_t CSeekableDataSourceConverter::ReadULEB128(){
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

int64_t CSeekableDataSourceConverter::ReadSLEB128(){
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

size_t CSeekableDataSourceConverter::Tell() const{
    return DActualSource->Tell();
}

size_t CSeekableDataSourceConverter::Seek(size_t pos){
    return DActualSource->Seek(pos);
}
