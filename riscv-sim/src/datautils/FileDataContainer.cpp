
#include "FileDataContainer.h"
#include "FileDataSource.h"
#include "FileDataSink.h"
#include "Path.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

CDirectoryDataContainerIterator::CDirectoryDataContainerIterator(const std::string &path, const SPrivateConstructorKey &key) : CDataContainerIterator(){
    DDirectory = opendir(path.c_str());
    DEntryResult = nullptr;
    if(nullptr != DDirectory){
        DEntryResult = readdir(DDirectory);
    }
}

CDirectoryDataContainerIterator::~CDirectoryDataContainerIterator(){
    if(nullptr != DDirectory){
        closedir(DDirectory);
    }
}

std::string CDirectoryDataContainerIterator::Name(){
    return DEntryResult ? DEntryResult->d_name : std::string();
}

bool CDirectoryDataContainerIterator::IsContainer(){
    return DEntryResult ? DT_DIR == DEntryResult->d_type : false;
}

bool CDirectoryDataContainerIterator::IsValid(){
    return nullptr != DEntryResult;
}

void CDirectoryDataContainerIterator::Next(){
    if(nullptr != DDirectory){
        if(nullptr != DEntryResult){
            DEntryResult = readdir(DDirectory);
        }
    }
}

CDirectoryDataContainer::CDirectoryDataContainer(const std::string &path) : CDataContainer(){
    DFullPath = CPath::CurrentPath().Simplify(path).ToString();
}

CDirectoryDataContainer::~CDirectoryDataContainer(){
    
}


std::shared_ptr< CDataContainerIterator > CDirectoryDataContainer::First(){
    if(DFullPath.length()){
        return std::make_shared< CDirectoryDataContainerIterator >(DFullPath, CDirectoryDataContainerIterator::SPrivateConstructorKey());
    }
    return nullptr;
}

std::shared_ptr< CDataSource > CDirectoryDataContainer::DataSource(const std::string &name){
    std::string FileName = CPath(DFullPath).Simplify(CPath(name)).ToString();
    
    if(FileName.length()){
        return std::make_shared< CFileDataSource >(FileName);
    }
    return nullptr;
}

std::shared_ptr< CDataSink > CDirectoryDataContainer::DataSink(const std::string &name){
    std::string FileName = CPath(DFullPath).Simplify(CPath(name)).ToString();
    
    if(FileName.length()){
        return std::make_shared< CFileDataSink >(FileName);
    }
    return nullptr;
}

std::shared_ptr< CDataContainer > CDirectoryDataContainer::Container(){
    std::string ContainerName = CPath(DFullPath).Containing().ToString();
    
    if(ContainerName.length()){
        return std::make_shared< CDirectoryDataContainer >(ContainerName);
    }
    return nullptr;
}

std::shared_ptr< CDataContainer > CDirectoryDataContainer::DataContainer(const std::string &name){
    std::string ContainerName = CPath(DFullPath).Simplify(CPath(name)).ToString();
    
    if(ContainerName.length()){
        return std::make_shared< CDirectoryDataContainer >(ContainerName);
    }
    return nullptr;
}


