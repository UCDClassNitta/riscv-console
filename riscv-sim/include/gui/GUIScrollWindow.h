#ifndef GUISCROLLWINDOW_H
#define GUISCROLLWINDOW_H

#include "GUIContainer.h"
#include "GUIScrollBar.h"

class CGUIScrollWindow : public virtual CGUIContainer{
    public:
        enum class EScrollPolicyType{Always = 0, Automatic, Never, External};

        virtual ~CGUIScrollWindow() = default;
        virtual void SetScrollPolicy(EScrollPolicyType hpolicy, EScrollPolicyType vpolicy) = 0;
        virtual void SetMinContentWidth(int width) = 0;
        virtual void SetMinContentHeight(int height) = 0;
        virtual std::shared_ptr<CGUIScrollBar> GetHorizontalScrollBar() = 0;
        virtual std::shared_ptr<CGUIScrollBar> GetVerticalScrollBar() = 0;
};

#endif
