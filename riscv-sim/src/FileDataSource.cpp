
#include "FileDataSource.h"
#include "FileDataContainer.h"
#include "Path.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

CFileDataSource::CFileDataSource(const std::string &filename, int fd) : CDataSource(){
    DFullPath = CPath::CurrentPath().Simplify(filename).ToString();

    DFileHandle = fd;
    if(DFullPath.length() && (0 > fd)){
        DFileHandle = open(DFullPath.c_str(), O_RDONLY);
        DCloseFile = true;
    }
}

CFileDataSource::~CFileDataSource(){
    if(DCloseFile && (0 <= DFileHandle)){
        close(DFileHandle);   
    }
}

int CFileDataSource::Read(void *data, int length){
    if(0 <= DFileHandle){
        int BytesRead = read(DFileHandle, data, length);
        
        if(0 < BytesRead){
            return BytesRead;   
        }
    }
    return -1;
}

std::shared_ptr< CDataContainer > CFileDataSource::Container(){
    std::string ContainerName = CPath(DFullPath).Containing().ToString();
    
    if(ContainerName.length()){
        return std::make_shared< CDirectoryDataContainer >(ContainerName);
        
    }
    return nullptr;
}
