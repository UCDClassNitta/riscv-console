#ifndef GUITEXTTAGGTK3_H
#define GUITEXTTAGGTK3_H
#include "GUITextTag.h"
#include <gtk/gtk.h>
#include <string>

class CGUITextTagGTK3 : public CGUITextTag{
    friend class CGUITextBufferGTK3;
    protected:
        GtkTextTag *DTextTag;
        static void TransformValue(GValue *value, const std::string &str_value);
    public:
        CGUITextTagGTK3(GtkTextTag *texttag);
        virtual ~CGUITextTagGTK3();

        int GetPriority() override;
        void SetPriority(int prio) override;
};

#endif