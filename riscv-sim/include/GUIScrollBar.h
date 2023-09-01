
#ifndef GUISCROLLBAR_H
#define GUISCROLLBAR_H
#include "GUIWidget.h"

class CGUIScrollBar : public virtual CGUIWidget{
    public:
        enum class EOrientation{Horizontal, Vertical};
        virtual ~CGUIScrollBar() = default;
        
        virtual double GetValue() = 0;
        virtual double GetLower() = 0;
        virtual double GetUpper() = 0;
        virtual double GetStepIncrement() = 0;
        virtual double GetPageIncrement() = 0;
        virtual double GetPageSize() = 0;
        virtual void SetValue(double val) = 0;
        virtual void SetLower(double lower) = 0;
        virtual void SetUpper(double upper) = 0;
        virtual void SetStepIncrement(double inc) = 0;
        virtual void SetPageIncrement(double inc) = 0;
        virtual void SetPageSize(double size) = 0;
};

#endif

