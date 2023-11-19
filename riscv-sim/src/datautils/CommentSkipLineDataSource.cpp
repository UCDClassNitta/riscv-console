#include "CommentSkipLineDataSource.h"

CCommentSkipLineDataSource::CCommentSkipLineDataSource(std::shared_ptr< CDataSource > source, char commentchar) : CLineDataSource(source){
    DCommentCharacter = commentchar;
}

bool CCommentSkipLineDataSource::Read(std::string &line){
    std::string TempLine;
    
    while(true){
        if(!CLineDataSource::Read(TempLine)){
            return false;
        }
        if(!TempLine.length() || TempLine[0] != DCommentCharacter){
            line = TempLine;
            break;
        }
        if((2 <= TempLine.length())&&(TempLine[1] == DCommentCharacter)){
            line = TempLine.substr(1);
            break;
        }
    }
    return true;
}

