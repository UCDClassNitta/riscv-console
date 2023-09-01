#ifndef GUISCROLLWINDOWGTK3_H
#define GUISCROLLWINDOWGTK3_H

#include "GUIScrollWindow.h"
#include "GUIContainerGTK3.h"
#include "GUIScrollBarGTK3.h"
#include <gtk/gtk.h>

class CGUIScrollWindowGTK3 : public virtual CGUIScrollWindow, public CGUIContainerGTK3{
    protected:
        static GtkPolicyType TranslatePolicy(EScrollPolicyType policy);
        std::shared_ptr<CGUIScrollBarGTK3> DHorizontalScrollBar;
        std::shared_ptr<CGUIScrollBarGTK3> DVerticalScrollBar;
    public:
        CGUIScrollWindowGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIScrollWindowGTK3();

        void SetScrollPolicy(EScrollPolicyType hpolicy, EScrollPolicyType vpolicy) override;
        void SetMinContentWidth(int width) override;
        void SetMinContentHeight(int height) override;
        std::shared_ptr<CGUIScrollBar> GetHorizontalScrollBar() override;
        std::shared_ptr<CGUIScrollBar> GetVerticalScrollBar() override;
};

#endif