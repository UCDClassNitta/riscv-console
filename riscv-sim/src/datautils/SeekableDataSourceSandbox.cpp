#include "SeekableDataSourceSandbox.h"


CSeekableDataSourceSandbox::CSeekableDataSourceSandbox(std::shared_ptr<CSeekableDataSource> source, size_t offset, size_t size) : DActualSource(source), DOffset(offset), DSize(size){
    DPosition = 0;
}

int CSeekableDataSourceSandbox::Read(void *data, int length){
    if(DPosition + length > DSize){
        length = DSize - DPosition;
    }
    if(!length){
        return -1;
    }
    auto PreviousPosition = DActualSource->Tell();
    DActualSource->Seek(DOffset + DPosition);
    length = DActualSource->Read(data,length);
    DActualSource->Seek(PreviousPosition);
    if(0 <= length){
        DPosition += length;
    }
    return length;
}

size_t CSeekableDataSourceSandbox::Tell() const {
    return DPosition;
}

size_t CSeekableDataSourceSandbox::Seek(size_t pos) {
    if(pos < DSize){
        DPosition = pos;
    }
    else{
        DPosition = DSize;
    }
    return DPosition;
}
