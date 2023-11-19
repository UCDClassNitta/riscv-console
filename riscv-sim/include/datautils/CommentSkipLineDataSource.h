#ifndef COMMENTSKIPLINEDATASOURCE_H
#define COMMENTSKIPLINEDATASOURCE_H

#include "LineDataSource.h"

class CCommentSkipLineDataSource : public CLineDataSource{
    protected:
        char DCommentCharacter;
    public:
        CCommentSkipLineDataSource(std::shared_ptr< CDataSource > source, char commentchar);
        
        virtual bool Read(std::string &line);
};

#endif
