
#ifndef GUISCROLLBAR_H
#define GUISCROLLBAR_H
#include "GUIWidget.h"

class CGUIScrollBar : public virtual CGUIWidget{
    public:
        enum class EOrientation{Horizontal, Vertical};
        virtual ~CGUIScrollBar(){};
        
        virtual double GetValue() = 0;
        virtual void SetValue(double val) = 0;
        virtual void SetLower(double lower) = 0;
        virtual void SetUpper(double upper) = 0;
        virtual void SetStepIncrement(double inc) = 0;
        virtual void SetPageIncrement(double inc) = 0;
        virtual void SetPageSize(double size) = 0;
};

#endif

