#ifndef GUISCROLLABLELINEBOX_H
#define GUISCROLLABLELINEBOX_H
#include "GUIWidget.h"
#include <string>
#include <vector>

class CGUIScrollableLineBox;

using TGUIScrollableLineBoxButtonEventCallback = bool (*)(std::shared_ptr<CGUIScrollableLineBox> widget, SGUIButtonEvent &event, size_t line, TGUICalldata data);
using TGUIScrollableLineBoxScrollEventCallback = bool (*)(std::shared_ptr<CGUIScrollableLineBox> widget, TGUICalldata data);

class CGUIScrollableLineBox{
    public:
        virtual ~CGUIScrollableLineBox() = default;

        virtual std::shared_ptr<CGUIWidget> ContainingWidget() const = 0;

        virtual void SetVerticalExpand(bool exp) = 0;
        virtual void SetHorizontalExpand(bool exp) = 0;
        virtual void SetCursor(std::shared_ptr<CGUICursor> cursor) = 0;

        virtual size_t GetBaseLine() const = 0;
        virtual void SetBaseLine(size_t line) = 0;
        virtual size_t GetLineCount() const = 0;
        virtual void SetLineCount(size_t count) = 0;

        virtual size_t GetBufferedLineCount() const = 0;
        virtual std::string GetBufferedLine(size_t index) const = 0;
        virtual void SetBufferedLines(const std::vector< std::string > &lines) = 0;

        virtual void UpdateBufferedLine(size_t index, const std::string &line) = 0;
        virtual void HighlightBufferedLine(size_t index) = 0;
        virtual size_t GetHighlightedBufferedLine() const = 0;

        virtual void SetFontFamily(const std::string &family) = 0;
        virtual void SetWidthCharacters(int chars) = 0;

        virtual void SetButtonPressEventCallback(TGUICalldata calldata, TGUIScrollableLineBoxButtonEventCallback callback) = 0;
        virtual void SetButtonReleaseEventCallback(TGUICalldata calldata, TGUIScrollableLineBoxButtonEventCallback callback) = 0;
        virtual void SetScrollEventCallback(TGUICalldata calldata, TGUIScrollableLineBoxScrollEventCallback callback) = 0;
};

#endif

