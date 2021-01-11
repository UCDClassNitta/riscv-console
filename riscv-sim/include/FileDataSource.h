
#ifndef FILEDATASOURCE_H
#define FILEDATASOURCE_H

#include "DataSource.h"

class CFileDataSource : public CDataSource, public std::enable_shared_from_this<CDataSource>{
    protected:
        int DFileHandle;
        std::string DFullPath;
        bool DCloseFile = false;
        
    public:
        CFileDataSource(const std::string &filename, int fd = -1);
        ~CFileDataSource();
        
        int Read(void *data, int length) override;
        std::shared_ptr< CDataContainer > Container() override;
};

#endif
