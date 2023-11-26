#ifndef JSONWRITER_H
#define JSONWRITER_H

#include "DataSink.h"
#include "JSONElement.h"
#include <cstdint>
#include <string>
#include <vector> 
#include <unordered_map>

class CJSONWriter{
    protected:
        std::shared_ptr< CDataSink > DOutputSink;
        std::string DBuffer;
        size_t DIndention;

        bool WriteString(const std::string &outstr);
        bool WriteElement(std::shared_ptr< const CJSONElement > element, size_t depth, bool initialindent);

    public:
        CJSONWriter(size_t indention = 2);
        virtual ~CJSONWriter() = default;
        
        size_t Indention() const;
        void Indention(size_t indention);
        bool Write(std::shared_ptr< CDataSink > sink, std::shared_ptr< const CJSONElement > element);
};

#endif
