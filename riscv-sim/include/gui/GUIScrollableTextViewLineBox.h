#ifndef GUISCROLLABLETEXTVIEWLINEBOX_H
#define GUISCROLLABLETEXTVIEWLINEBOX_H
#include "GUIScrollableLineBox.h"
#include "GUIFactory.h"
#include "GUIFrame.h"
#include "GUIGrid.h"
#include "GUILabel.h"
#include "GUIEventBox.h"
#include "GUIScrollWindow.h"
#include "GUITextView.h"
#include <string>
#include <vector>


class CGUIScrollableTextViewLineBox : public virtual CGUIScrollableLineBox, public std::enable_shared_from_this<CGUIScrollableTextViewLineBox> {
    protected:
        std::shared_ptr<CGUIFactory> DGUIFactory;
        std::shared_ptr<CGUIFrame> DContainingFrame;
        std::shared_ptr<CGUITextView> DTextView;
        std::shared_ptr<CGUIScrollWindow> DScrollWindow;
        std::shared_ptr<CGUITextTag> DHighlightTag;

        size_t DHightlightedBufferedLine;
        size_t DMinLines;
        std::string DFontFamily;
        int DWidthCharacters;
        int DMaxWidthCharacters;

        TGUICalldata DButtonPressCalldata;
        TGUIScrollableLineBoxButtonEventCallback DButtonPressCallback;
        
        TGUICalldata DButtonReleaseCalldata;
        TGUIScrollableLineBoxButtonEventCallback DButtonReleaseCallback;

        TGUICalldata DScrollCalldata;
        TGUIScrollableLineBoxScrollEventCallback DScrollCallback;

        static bool ScrollBarChangedEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);

        static bool WidgetScrollEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIScrollEvent &event, TGUICalldata data);

        static bool WidgetButtonEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);

        virtual bool ScrollBarChangedEvent(std::shared_ptr<CGUIWidget> widget);

        virtual bool WidgetScrollEvent(std::shared_ptr<CGUIWidget> widget, SGUIScrollEvent &event);

    public:
        CGUIScrollableTextViewLineBox(std::shared_ptr<CGUIFactory> guifactory, size_t initsize=8);
        virtual ~CGUIScrollableTextViewLineBox();

        virtual std::shared_ptr<CGUIWidget> ContainingWidget() const override;

        virtual void SetVerticalExpand(bool exp) override;
        virtual void SetHorizontalExpand(bool exp) override;
        virtual void SetCursor(std::shared_ptr<CGUICursor> cursor) override; 

        virtual size_t GetBaseLine() const override;
        virtual void SetBaseLine(size_t line) override;
        virtual size_t GetLineCount() const override;
        virtual void SetLineCount(size_t count) override;

        virtual size_t GetBufferedLineCount() const override;
        virtual std::string GetBufferedLine(size_t index) const override;
        virtual void SetBufferedLines(const std::vector< std::string > &lines) override;

        virtual void UpdateBufferedLine(size_t index, const std::string &line) override;
        virtual void HighlightBufferedLine(size_t index) override;
        virtual size_t GetHighlightedBufferedLine() const override;

        virtual void SetFontFamily(const std::string &family) override;
        virtual void SetWidthCharacters(int chars) override;

        virtual void SetButtonPressEventCallback(TGUICalldata calldata, TGUIScrollableLineBoxButtonEventCallback callback) override;
        virtual void SetButtonReleaseEventCallback(TGUICalldata calldata, TGUIScrollableLineBoxButtonEventCallback callback) override;
        virtual void SetScrollEventCallback(TGUICalldata calldata, TGUIScrollableLineBoxScrollEventCallback callback) override;

};

#endif

