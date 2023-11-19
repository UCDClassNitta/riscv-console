
#ifndef SEEKABLEDATASOURCE_H
#define SEEKABLEDATASOURCE_H

#include "DataSource.h"

class CSeekableDataSource : public CDataSource{
    public:
        virtual ~CSeekableDataSource() = default;
        virtual size_t Tell() const = 0;
        virtual size_t Seek(size_t pos) = 0;
};

#endif
