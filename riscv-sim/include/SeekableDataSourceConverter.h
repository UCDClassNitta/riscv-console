
#ifndef SEEKABLEDATASOURCECONVERTER_H
#define SEEKABLEDATASOURCECONVERTER_H

#include "SeekableDataSource.h"

class CSeekableDataSourceConverter : public CSeekableDataSource{
    private:
        std::shared_ptr<CSeekableDataSource> DActualSource;
        bool DLittleEndian;

    public:
        CSeekableDataSourceConverter(std::shared_ptr<CSeekableDataSource> source, bool littleendian = true);
        virtual ~CSeekableDataSourceConverter() = default;
        bool LittleEndian() const;
        bool LittleEndian(bool littleendian);
        int Read(void *data, int length) override;
        uint8_t ReadUINT8();
        uint16_t ReadUINT16();
        uint32_t ReadUINT32();
        uint64_t ReadUINT64();
        uint64_t ReadULEB128();
        int64_t ReadSLEB128();
        size_t Tell() const override;
        size_t Seek(size_t pos) override;
};

#endif
