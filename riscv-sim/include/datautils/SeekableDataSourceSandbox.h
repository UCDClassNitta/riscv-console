
#ifndef SEEKABLEDATASOURCESANDBOX_H
#define SEEKABLEDATASOURCESANDBOX_H

#include "SeekableDataSource.h"

class CSeekableDataSourceSandbox : public CSeekableDataSource{
    private:
        std::shared_ptr<CSeekableDataSource> DActualSource;
        size_t DOffset;
        size_t DSize;
        size_t DPosition;

    public:
        CSeekableDataSourceSandbox(std::shared_ptr<CSeekableDataSource> source, size_t offset, size_t size);
        virtual ~CSeekableDataSourceSandbox() = default;
        int Read(void *data, int length) override;
        size_t Tell() const override;
        size_t Seek(size_t pos) override;
};

#endif
