#ifndef GUITEXTBUFFER_H
#define GUITEXTBUFFER_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "GUITextTag.h"
#include "GUITextIter.h"

class CGUITextBuffer{
    public:
        virtual ~CGUITextBuffer() = default;

        virtual void SetText(const std::string &text) = 0;
        virtual void Insert(std::shared_ptr<CGUITextIter> start, const std::string &text) = 0;
        virtual void Delete(std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end) = 0;
        virtual std::shared_ptr<CGUITextTag> CreateTag(const std::vector< std::pair<std::string, std::string> > &properties) = 0;
        virtual void ApplyTag(std::shared_ptr<CGUITextTag> tag, std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end) = 0;
        virtual void RemoveTag(std::shared_ptr<CGUITextTag> tag, std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end) = 0;
        virtual std::shared_ptr<CGUITextIter> GetLineIter(int linenum) = 0;
        virtual std::shared_ptr<CGUITextIter> GetStartIter() = 0;
        virtual std::shared_ptr<CGUITextIter> GetEndIter() = 0;
        virtual std::string GetText(std::shared_ptr<CGUITextIter> start, std::shared_ptr<CGUITextIter> end) = 0;
        virtual int GetLineCount() = 0;
};

#endif
