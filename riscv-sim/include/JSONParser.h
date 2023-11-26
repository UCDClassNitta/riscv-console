#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "DataSource.h"
#include "JSONElement.h"
#include <cstdint>
#include <string>
#include <vector> 
#include <unordered_map>

class CJSONParser{
    public:
        enum class ETokenType{Invalid, Number, String, Boolean, Operator, End};
        enum class EOperatorType{Invalid, OpenCurly, CloseCurly, OpenSquare, CloseSquare, Colon, Comma};
        class CToken{
            protected:
                ETokenType DType;
                std::string DText;
                uint32_t DLine;
                uint32_t DColumn;
            public:
                CToken(ETokenType type, const std::string &text, uint32_t line, uint32_t column);
                CToken() = default;
                CToken(const CToken &token) = default;
                ~CToken() = default;

                ETokenType TokenType() const;
                std::string Text() const;
                uint32_t Line() const;
                uint32_t Column() const;
                EOperatorType OperatorType() const;
        };
    protected:
        std::shared_ptr< CDataSource > DSource;
        uint32_t DLine;
        uint32_t DColumn;
        bool DValidChar;
        char DPeekedChar;
        bool DValidToken;
        CToken DPeekedToken;
        std::shared_ptr< CJSONElement > DRoot;
        std::vector< std::shared_ptr< CJSONElement > > DParseStack;

        char PeekChar();
        void ConsumeChar();

        void ScanNumber();
        void ScanString();
        void ScanBoolean();
        void ScanOperator();
        void ScanInvalid(const std::string &beginning, uint32_t line, uint32_t column);

        CToken NextToken();
        CToken PeekToken();

        virtual void ConsumeToken(const CToken &token);
        virtual bool ParseJSON();
        virtual bool ParsePrimitive();
        virtual bool ParseContainer();
        virtual bool ParseArray();
        virtual bool ParseObject();
        virtual bool ParseMember();

    public:
        CJSONParser();
        virtual ~CJSONParser() = default;

        bool Parse(std::shared_ptr< CDataSource > source);
        std::shared_ptr< const CJSONElement > Root() const;
};

#endif
