
#ifndef GUILABEL_H
#define GUILABEL_H
#include "GUIWidget.h"
#include "GUIJustification.h"

class CGUILabel : public virtual CGUIWidget{
    public:
        virtual ~CGUILabel(){};        
        
        virtual std::string GetText() = 0;
        virtual void SetText(const std::string &str) = 0;
        virtual void SetMarkup(const std::string &str) = 0;
        virtual void SetJustification(const SGUIJustificationType &justify) = 0;
        virtual void SetFontFamily(const std::string &family) = 0;
        virtual void SetBold(bool bold) = 0;
        virtual void SetWidthCharacters(int chars) = 0;
        virtual void SetMaxWidthCharacters(int chars) = 0;
};

#endif

