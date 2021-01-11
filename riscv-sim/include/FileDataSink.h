
#ifndef FILEDATASINK_H
#define FILEDATASINK_H

#include "DataSink.h"

class CFileDataSink : public CDataSink, public std::enable_shared_from_this<CDataSink>{
    protected:     
        int DFileHandle;
        std::string DFullPath;
        bool DCloseFile = false;
        
    public:
        CFileDataSink(const std::string &filename, int fd = -1);
        ~CFileDataSink();                         
        
        int Write(const void *data, int length) override;
        std::shared_ptr< CDataContainer > Container() override;
};

#endif
                                                                                                                   