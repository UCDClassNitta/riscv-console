#include "JSONWriter.h"

CJSONWriter::CJSONWriter(size_t indention){
    Indention(indention);
}

bool CJSONWriter::WriteString(const std::string &outstr){
    static const size_t MaxBufferSize = 4096;
    DBuffer += outstr;
    if(MaxBufferSize <= DBuffer.length()){
        DOutputSink->Write(DBuffer.c_str(),DBuffer.length());
        DBuffer.clear();
    }
    return true;
}

bool CJSONWriter::WriteElement(std::shared_ptr< const CJSONElement > element, size_t depth, bool initialindent){
    if(initialindent){
        WriteString(std::string(DIndention * depth,' '));
    }
    switch(element->Type()){
        case CJSONElement::EType::Boolean:
        case CJSONElement::EType::Number:   WriteString(element->PrimitiveText());
                                            break;
        case CJSONElement::EType::String:   WriteString(std::string("\"") + element->PrimitiveText() + std::string("\""));
                                            break;
        case CJSONElement::EType::Array:    WriteString(std::string("[\n"));
                                            for(size_t Index = 0; Index < element->ChildCount(); Index++){
                                                WriteElement(element->Index(Index),depth+1,true);
                                                if(Index + 1 < element->ChildCount()){
                                                    WriteString(",");
                                                }
                                                WriteString("\n");
                                            }
                                            WriteString(std::string(DIndention * depth,' ') + std::string("]"));
                                            break;
        case CJSONElement::EType::Object:   {
                                                std::vector< std::string > Keys;
                                                WriteString(std::string("{\n"));
                                                element->MemberKeys(Keys);
                                                for(size_t Index = 0; Index < Keys.size(); Index++){
                                                    WriteString(std::string(DIndention * (depth + 1),' '));
                                                    WriteString(std::string("\"") + Keys[Index] + std::string("\": "));
                                                    WriteElement(element->Member(Keys[Index]),depth+1,false);
                                                    if(Index + 1 < Keys.size()){
                                                        WriteString(std::string(","));
                                                    }
                                                    WriteString(std::string("\n"));
                                                }
                                                WriteString(std::string(DIndention * depth,' ') + std::string("}"));
                                            }
                                            break;
    }
    return true;
}

size_t CJSONWriter::Indention() const{
    return DIndention;
}

void CJSONWriter::Indention(size_t indention){
    DIndention = 0 < indention ? indention : 1;
}

bool CJSONWriter::Write(std::shared_ptr< CDataSink > sink, std::shared_ptr< const CJSONElement > element){
    bool ReturnValue = false;
    if(sink){
        DOutputSink = sink;
        DBuffer.clear();
        if(WriteElement(element,0,false)){
            if(!DBuffer.empty()){
                DOutputSink->Write(DBuffer.c_str(),DBuffer.length());
            }
            ReturnValue = true;
        }
        DOutputSink.reset();
    }
    return ReturnValue;
}
