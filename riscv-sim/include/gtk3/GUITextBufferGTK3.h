#ifndef GUITEXTBUFFERGTK3_H
#define GUITEXTBUFFERGTK3_H
#include "GUITextBuffer.h"
#include "GUITextTagGTK3.h"
#include "GUITextIterGTK3.h"
#include <gtk/gtk.h>
#include <string>

class CGUITextBufferGTK3 : public CGUITextBuffer{
    protected:
        GtkTextBuffer *DTextBuffer;
    public:
        CGUITextBufferGTK3(GtkTextBuffer *textbuffer);
        virtual ~CGUITextBufferGTK3();

        void SetText(const std::string &text) override;
        void Insert(std::shared_ptr<CGUITextIter> start, const std::string &text) override;
        void Delete(std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end) override;
        std::shared_ptr<CGUITextTag> CreateTag(const std::vector< std::pair<std::string, std::string> > &properties) override;
        void ApplyTag(std::shared_ptr<CGUITextTag> tag, std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end) override;
        void RemoveTag(std::shared_ptr<CGUITextTag> tag, std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end) override;
        std::shared_ptr<CGUITextIter> GetLineIter(int linenum) override;
        std::shared_ptr<CGUITextIter> GetStartIter() override;
        std::shared_ptr<CGUITextIter> GetEndIter() override;
        std::string GetText(std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end) override;
        int GetLineCount() override;
};

#endif