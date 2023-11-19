#ifndef LINEDATASOURCE_H
#define LINEDATASOURCE_H

#include "DataSource.h"

class CLineDataSource{
    protected:
        std::shared_ptr< CDataSource > DDataSource;
    public:
        CLineDataSource(std::shared_ptr< CDataSource > source);
        
        bool Read(std::string &line);
};

#endif
