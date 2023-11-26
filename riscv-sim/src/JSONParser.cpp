#include "JSONParser.h"
#include <unordered_map>
#include <unordered_set>
#include <cctype>

CJSONParser::CToken::CToken(ETokenType type, const std::string &text, uint32_t line, uint32_t column){
    DType = type;
    DText = text;
    DLine = line;
    DColumn = column;
}

CJSONParser::ETokenType CJSONParser::CToken::TokenType() const{
    return DType;
}

std::string CJSONParser::CToken::Text() const{
    return DText;
}

uint32_t CJSONParser::CToken::Line() const{
    return DLine;
}

uint32_t CJSONParser::CToken::Column() const{
    return DColumn;
}

CJSONParser::EOperatorType CJSONParser::CToken::OperatorType() const{
    static std::unordered_map< std::string, EOperatorType > Operators = {{"{",EOperatorType::OpenCurly},{"}",EOperatorType::CloseCurly},
                                                                        {"[",EOperatorType::OpenSquare},{"]",EOperatorType::CloseSquare},
                                                                        {":",EOperatorType::Colon},{",",EOperatorType::Comma}};
    auto Search = Operators.find(DText);
    if(Operators.end() != Search){
        return Search->second;
    }
    return EOperatorType::Invalid;
}

CJSONParser::CJSONParser(){
    DLine = 0;
    DColumn = 0;
    DValidChar = false;
    DValidToken = false;
}

char CJSONParser::PeekChar(){
    if(!DValidChar){
        if(1 != DSource->Read(&DPeekedChar,1)){
            return -1;
        }
        DValidChar = true;
    }
    return DPeekedChar;
}

void CJSONParser::ConsumeChar(){
    DValidChar = false;
    if('\n' == DPeekedChar){
        DColumn = 0;
        DLine++;
    }
    else{
        DColumn++;
    }
}

void CJSONParser::ScanNumber(){
    auto PeekedChar = PeekChar();
    auto Line = DLine;
    auto Column = DColumn;
    std::string TokenText;
    bool InvalidNumber = false;
    if('-' == PeekedChar){
        TokenText += std::string(1, PeekedChar);
        ConsumeChar();
        PeekedChar = PeekChar();
    }
    if(!isdigit(PeekedChar)){
        DPeekedToken = CToken(ETokenType::Invalid, TokenText, Line, Column);
        DValidToken = true;
        return;
    }
    while(isdigit(PeekedChar)){
        TokenText += std::string(1, PeekedChar);
        ConsumeChar();
        PeekedChar = PeekChar();
    }
    if('.' == PeekedChar){
        TokenText += std::string(1, PeekedChar);
        ConsumeChar();
        PeekedChar = PeekChar();
        if(!isdigit(PeekedChar)){
            DPeekedToken = CToken(ETokenType::Invalid, TokenText, Line, Column);
            DValidToken = true;
            return;
        }
        while(isdigit(PeekedChar)){
            TokenText += std::string(1, PeekedChar);
            ConsumeChar();
            PeekedChar = PeekChar();
        }
    }
    if('e' == PeekedChar){
        TokenText += std::string(1, PeekedChar);
        ConsumeChar();
        PeekedChar = PeekChar();
        if(('+' == PeekedChar)||('-' == PeekedChar)){
            TokenText += std::string(1, PeekedChar);
            ConsumeChar();
            PeekedChar = PeekChar();
        }
        if(!isdigit(PeekedChar)){
            DPeekedToken = CToken(ETokenType::Invalid, TokenText, Line, Column);
            DValidToken = true;
            return;
        }
        while(isdigit(PeekedChar)){
            TokenText += std::string(1, PeekedChar);
            ConsumeChar();
            PeekedChar = PeekChar();
        }
    }
    if(isalpha(PeekedChar)){
        while(isalnum(PeekedChar)){
            TokenText += std::string(1,PeekedChar);
            ConsumeChar();
            PeekedChar = PeekChar();
        }
        InvalidNumber = true;
    }
    DPeekedToken = CToken(InvalidNumber  ? ETokenType::Invalid : ETokenType::Number, TokenText, Line, Column);
    DValidToken = true;
}

void CJSONParser::ScanString(){
    auto PeekedChar = PeekChar();
    auto Line = DLine;
    auto Column = DColumn;
    std::string TokenText;
    bool ValidString = true;
    ConsumeChar();
    PeekedChar = PeekChar();
    while('"' != PeekedChar){
        if(0 > PeekedChar){
            DPeekedToken = CToken(ETokenType::Invalid, TokenText, Line, Column);
            DValidToken = true;
            return;
        }
        if('\\' == PeekedChar){
            ConsumeChar();
            PeekedChar = PeekChar();
            switch(PeekedChar){
                case 'b':   TokenText += std::string(1,'\b');
                            break;
                case 'n':   TokenText += std::string(1,'\n');
                            break;
                case 'r':   TokenText += std::string(1,'\r');
                            break;
                case 't':   TokenText += std::string(1,'\t');
                            break;
                case '\\':  TokenText += std::string(1,'\\');
                            break;
                case '\'':  TokenText += std::string(1,'\'');
                            break;
                case '"':   TokenText += std::string(1,'"');
                            break;
                default:    TokenText += std::string(1,'\\');
                            TokenText += std::string(1,PeekedChar);
                            ValidString = false;
                            break;
            }
        }
        else if((' ' <= PeekedChar) && ('~' >= PeekedChar)){
            TokenText += std::string(1,PeekedChar);
        }
        else{
            TokenText += std::string(1,PeekedChar);
            ValidString = false;
        }
        ConsumeChar();
        PeekedChar = PeekChar();
    }
    ConsumeChar();
    DPeekedToken = CToken(ValidString ? ETokenType::String : ETokenType::Invalid, TokenText, Line, Column);
    DValidToken = true;
}

void CJSONParser::ScanBoolean(){
    static std::unordered_set< std::string > ValidBoolean = {"true","false","null"};
    auto PeekedChar = PeekChar();
    auto Line = DLine;
    auto Column = DColumn;
    std::string TokenText;
    while(isalnum(PeekedChar)){
        TokenText += std::string(1,PeekedChar);
        ConsumeChar();
        PeekedChar = PeekChar();
    }
    DPeekedToken = CToken(ValidBoolean.end() != ValidBoolean.find(TokenText) ? ETokenType::Boolean : ETokenType::Invalid, TokenText, Line, Column);
    DValidToken = true;
}

void CJSONParser::ScanOperator(){
    auto PeekedChar = PeekChar();
    DPeekedToken = CToken(ETokenType::Operator, std::string(1,PeekedChar), DLine, DColumn);
    DValidToken = true;
    ConsumeChar();
}

void CJSONParser::ScanInvalid(const std::string &beginning, uint32_t line, uint32_t column){
    auto PeekedChar = PeekChar();
    if(0 > PeekedChar){
        DPeekedToken = CToken(ETokenType::End, std::string(), line, column);
        DValidToken = true;
        return;
    }
    std::string TokenText = beginning;
    if(beginning.empty() && ispunct(PeekedChar)){
        TokenText = std::string(1,PeekedChar);
        ConsumeChar();
    }
    else{
        while(isalnum(PeekedChar)){
            TokenText += std::string(1,PeekedChar);
            ConsumeChar();
            PeekedChar = PeekChar();
        }
    }
    DPeekedToken = CToken(ETokenType::Invalid, TokenText, line, column);
    DValidToken = true;
}

CJSONParser::CToken CJSONParser::NextToken(){
    CJSONParser::CToken ReturnToken = PeekToken();
    ConsumeToken(ReturnToken);
    return ReturnToken;
}

CJSONParser::CToken CJSONParser::PeekToken(){
    if(!DValidToken){
        auto PeekedChar = PeekChar();
        while(isspace(PeekedChar)){
            ConsumeChar();
            PeekedChar = PeekChar();
        }
        if(('-' == PeekedChar)||isdigit(PeekedChar)){
            ScanNumber();
        }
        else if(('t' == PeekedChar)||('f' == PeekedChar)||('n' == PeekedChar)){
            ScanBoolean();
        }
        else if('"' == PeekedChar){
            ScanString();
        }
        else{
            switch(PeekChar()){
                case '[':
                case ']':
                case '{':
                case '}':
                case ':':
                case ',':   ScanOperator();
                            break;
                default:    ScanInvalid(std::string(), DLine, DColumn);
            }
        }
    }
    return DPeekedToken;
}

void CJSONParser::ConsumeToken(const CToken &token){
    DValidToken = false;
}

bool CJSONParser::ParseJSON(){
    auto TempToken = PeekToken();
    switch(TempToken.TokenType()){
        case ETokenType::Number:
        case ETokenType::String:
        case ETokenType::Boolean:   return ParsePrimitive();
        case ETokenType::Operator:  return ParseContainer();
        default:                    return false;
    }
}

bool CJSONParser::ParsePrimitive(){
    auto PrimitiveToken = NextToken();
    auto NewPrimitive = std::make_shared< CJSONElement >();    
    DParseStack.push_back(NewPrimitive);
    switch(PrimitiveToken.TokenType()){
        case ETokenType::Number:    NewPrimitive->Type(CJSONElement::EType::Number);
                                    NewPrimitive->PrimitiveText(PrimitiveToken.Text());
                                    return true;
        case ETokenType::String:    NewPrimitive->Type(CJSONElement::EType::String);
                                    NewPrimitive->PrimitiveText(PrimitiveToken.Text());
                                    return true;
        case ETokenType::Boolean:   NewPrimitive->Type(CJSONElement::EType::Boolean);
                                    NewPrimitive->PrimitiveText(PrimitiveToken.Text());
                                    return true;
        default:                    return false;
    }
}

bool CJSONParser::ParseContainer(){
    auto OpenToken = PeekToken();
    switch(OpenToken.OperatorType()){
        case EOperatorType::OpenCurly:  return ParseObject();
        case EOperatorType::OpenSquare: return ParseArray();
        default:                        return false;
    }
}

bool CJSONParser::ParseArray(){
    auto NextParsedToken = NextToken();
    auto NewArray = std::make_shared< CJSONElement >();
    NewArray->Type(CJSONElement::EType::Array);
    DParseStack.push_back(NewArray);
    do{
        if(!ParseJSON()){
            return false;
        }
        NewArray->PushBack(DParseStack.back());
        DParseStack.pop_back();
        NextParsedToken = NextToken();
    }while(EOperatorType::Comma == NextParsedToken.OperatorType());
    return EOperatorType::CloseSquare == NextParsedToken.OperatorType();
}

bool CJSONParser::ParseObject(){
    auto NextParsedToken = NextToken();
    auto NewObject = std::make_shared< CJSONElement >();
    NewObject->Type(CJSONElement::EType::Object);
    DParseStack.push_back(NewObject);
    do{
        if(!ParseMember()){
            return false;
        }
        NextParsedToken = NextToken();
    }while(EOperatorType::Comma == NextParsedToken.OperatorType());
    if(EOperatorType::CloseCurly == NextParsedToken.OperatorType()){
        return true;
    }
    return false;
}

bool CJSONParser::ParseMember(){
    auto NextParsedToken = NextToken();
    if(ETokenType::String != NextParsedToken.TokenType()){
        return false;
    }
    auto MemberKey = NextParsedToken.Text();
    NextParsedToken = NextToken();
    if(EOperatorType::Colon != NextParsedToken.OperatorType()){
        return false;
    }
    if(ParseJSON()){
        auto MemberValue = DParseStack.back();
        DParseStack.pop_back();
        if(DParseStack.back()->Member(MemberKey)){
            return false;
        }
        DParseStack.back()->AssignMember(MemberKey, MemberValue);
        return true;
    }
    return false;
}

bool CJSONParser::Parse(std::shared_ptr< CDataSource > source){
    DRoot.reset();
    DParseStack.clear();
    DLine = 0;
    DColumn = 0;
    DValidChar = false;
    DValidToken = false;
    DSource = source;
    auto TempToken = PeekToken();
    switch(TempToken.TokenType()){
        case ETokenType::Invalid:
        case ETokenType::End:       return false;
        default:                    break;
    }
    if(ParseJSON()){
        DRoot = DParseStack.back();
    }
    return bool(DRoot);
}

std::shared_ptr< const CJSONElement > CJSONParser::Root() const{
    return DRoot;
}
