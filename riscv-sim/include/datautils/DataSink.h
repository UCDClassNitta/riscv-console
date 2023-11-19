
#ifndef DATASINK_H
#define DATASINK_H

#include <string>
#include <memory>
#include "DataContainer.h"

class CDataSink{
    public:
        virtual ~CDataSink() = default;
        virtual int Write(const void *data, int length) = 0;
        virtual std::shared_ptr< CDataContainer > Container(){
            return nullptr;
        };
};

#endif
