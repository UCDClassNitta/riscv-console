#include "BufferedSeekableDataSource.h"
#include <cstring>

const size_t CBufferedSeekableDataSource::DBlockSize = 512;

CBufferedSeekableDataSource::CBufferedSeekableDataSource(std::shared_ptr<CDataSource> source) : DActualSource(source){
    DBufferedPosition = 0;
}

CBufferedSeekableDataSource::CBufferedSeekableDataSource(const std::vector< uint8_t > &buffer) : DBufferedData(buffer){
    DBufferedPosition = 0;
}

void CBufferedSeekableDataSource::PrepareToPosition(size_t pos){
    if(pos > DBufferedData.size()){
        auto OldSize =  DBufferedData.size();
        auto NewSize = ((pos + DBlockSize - 1) / DBlockSize) * DBlockSize;
        DBufferedData.resize(NewSize);
        int BytesToRead = DBufferedData.size() - OldSize;
        auto BytesRead = DActualSource ? DActualSource->Read(DBufferedData.data() + OldSize, BytesToRead) : 0;
        if(BytesRead < BytesToRead){
            if(0 > BytesRead){
                BytesRead = 0;
            }
            DBufferedData.resize(OldSize + BytesRead);
        }
    }
}

int CBufferedSeekableDataSource::Read(void *data, int length){
    PrepareToPosition(DBufferedPosition + length);
    if(DBufferedPosition + length  > DBufferedData.size()){
        if(DBufferedPosition >= DBufferedData.size()){
            return -1;
        }
        length = DBufferedData.size() - DBufferedPosition;
    }
    memcpy(data,DBufferedData.data() + DBufferedPosition, length);
    DBufferedPosition += length;
    return length;
}

size_t CBufferedSeekableDataSource::Tell() const{
    return DBufferedPosition;
}

size_t CBufferedSeekableDataSource::Seek(size_t pos){
    PrepareToPosition(pos);
    if(pos > DBufferedData.size()){
       pos = DBufferedData.size();
    }
    return DBufferedPosition = pos;
}
