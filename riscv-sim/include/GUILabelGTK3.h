#ifndef GUILABELGTK3_H
#define GUILABELGTK3_H

#include "GUILabel.h"
#include "GUIWidgetGTK3.h"

#include <string>
#include <gtk/gtk.h>

class CGUILabelGTK3 : public virtual CGUILabel, public CGUIWidgetGTK3{
    public:
        CGUILabelGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUILabelGTK3();
        
        std::string GetText() override;
        void SetText(const std::string &str) override;
        void SetMarkup(const std::string &str) override;
        void SetJustification(const SGUIJustificationType &justify) override;
        void SetFontFamily(const std::string &family) override;
        void SetBold(bool bold) override;
        void SetWidthCharacters(int chars) override;
        void SetMaxWidthCharacters(int chars) override;
};

#endif