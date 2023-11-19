#include "LineDataSource.h"

CLineDataSource::CLineDataSource(std::shared_ptr< CDataSource > source){
    DDataSource = source;
}

bool CLineDataSource::Read(std::string &line){
    char TempChar;
    
    line.clear();
    while(true){
        if(0 < DDataSource->Read(&TempChar, 1)){
            if('\n' == TempChar){
                return true;
            }
            else if('\r' != TempChar){
                line += TempChar;
            }
        }
        else{
            return 0 < line.length();
        }
    }
    
}

