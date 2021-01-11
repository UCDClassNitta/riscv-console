
#ifndef GUILABEL_H
#define GUILABEL_H
#include "GUIWidget.h"

class CGUILabel : public virtual CGUIWidget{
    public:
        virtual ~CGUILabel(){};        
        
        virtual std::string GetText() = 0;
        virtual void SetText(const std::string &str) = 0;
        virtual void SetMarkup(const std::string &str) = 0;
};

#endif

