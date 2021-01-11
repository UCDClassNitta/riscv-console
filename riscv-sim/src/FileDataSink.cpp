
#include "FileDataSink.h"
#include "FileDataContainer.h"
#include "Path.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

CFileDataSink::CFileDataSink(const std::string &filename, int fd) : CDataSink(){
    DFullPath = CPath::CurrentPath().Simplify(filename).ToString();

    DFileHandle = fd;
    if(DFullPath.length() && (0 > fd)){
        DFileHandle = open(DFullPath.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        
        DCloseFile = true;
    }
}

CFileDataSink::~CFileDataSink(){
    if(DCloseFile && (0 <= DFileHandle)){
        close(DFileHandle);   
    }
}

int CFileDataSink::Write(const void *data, int length){
    if(0 <= DFileHandle){
        int BytesWritten = write(DFileHandle, data, length);
        
        if(0 < BytesWritten){
            return BytesWritten;   
        }
    }
    return -1;
}

std::shared_ptr< CDataContainer > CFileDataSink::Container(){
    std::string ContainerName = CPath(DFullPath).Containing().ToString();
    
    if(ContainerName.length()){
        return std::make_shared< CDirectoryDataContainer >(ContainerName);
        
    }
    return nullptr;
}
