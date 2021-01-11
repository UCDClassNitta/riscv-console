
#ifndef FILEDATACONTAINER_H
#define FILEDATACONTAINER_H

#include "DataContainer.h"
#include <dirent.h>

class CDirectoryDataContainerIterator : public CDataContainerIterator{
    friend class CDirectoryDataContainer;
    
    protected:
        struct SPrivateConstructorKey{};
        DIR *DDirectory;
        struct dirent *DEntryResult;
        
    public:  
        explicit CDirectoryDataContainerIterator(const std::string &path, const SPrivateConstructorKey &key);
        ~CDirectoryDataContainerIterator();
        std::string Name();
        bool IsContainer();
        bool IsValid();
        void Next();
};

class CDirectoryDataContainer : public CDataContainer{
    protected:
        std::string DFullPath;
        
    public:
        CDirectoryDataContainer(const std::string &path);
        ~CDirectoryDataContainer();
        
        std::shared_ptr< CDataContainerIterator > First() override;
        std::shared_ptr< CDataSource > DataSource(const std::string &name) override;
        std::shared_ptr< CDataSink > DataSink(const std::string &name)  override;
        std::shared_ptr< CDataContainer > Container()  override;
        std::shared_ptr< CDataContainer > DataContainer(const std::string &name)  override;
};

#endif
