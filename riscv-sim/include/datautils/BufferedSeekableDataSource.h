
#ifndef BUFFEREDSEEKABLEDATASOURCE_H
#define BUFFEREDSEEKABLEDATASOURCE_H

#include <cstdint>
#include <vector>
#include "SeekableDataSource.h"

class CBufferedSeekableDataSource : public CSeekableDataSource{
    private:
        std::shared_ptr<CDataSource> DActualSource;
        std::vector< uint8_t > DBufferedData;
        size_t DBufferedPosition;
        static const size_t DBlockSize;
        void PrepareToPosition(size_t pos);

    public:
        CBufferedSeekableDataSource(std::shared_ptr<CDataSource> source);
        CBufferedSeekableDataSource(const std::vector< uint8_t > &buffer);
        virtual ~CBufferedSeekableDataSource() = default;
        int Read(void *data, int length) override;
        size_t Tell() const override;
        size_t Seek(size_t pos) override;
};

#endif
