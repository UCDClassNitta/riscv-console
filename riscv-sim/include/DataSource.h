
#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <string>
#include <memory>
#include "DataContainer.h"

class CDataSource{
    public:
        virtual ~CDataSource(){};
        virtual int Read(void *data, int length) = 0;
        virtual std::shared_ptr< CDataContainer > Container(){
            return nullptr;
        };
};

#endif
