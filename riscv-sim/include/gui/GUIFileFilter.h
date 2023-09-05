#ifndef GUIFILEFILTER_H
#define GUIFILEFILTER_H

#include <string>

class CGUIFileFilter{
    public:
        virtual ~CGUIFileFilter() = default;
        
        virtual void AddPattern(const std::string &pattern) = 0;
};

#endif