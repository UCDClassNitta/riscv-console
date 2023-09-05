#ifndef GUISCROLLABLELABELBOX_H
#define GUISCROLLABLELABELBOX_H
#include "GUIScrollableLineBox.h"
#include "GUIFactory.h"
#include "GUIFrame.h"
#include "GUIGrid.h"
#include "GUILabel.h"
#include "GUIEventBox.h"
#include "GUIScrollBar.h"
#include <string>
#include <vector>


class CGUIScrollableLabelBox : public virtual CGUIScrollableLineBox, public std::enable_shared_from_this<CGUIScrollableLabelBox> {
    protected:
        std::shared_ptr<CGUIFactory> DGUIFactory;
        std::shared_ptr<CGUIGrid> DContainingGrid;
        std::shared_ptr<CGUIFrame> DContainingFrame;
        std::vector< std::shared_ptr<CGUIEventBox> > DLabelEventBoxes;
        std::vector< std::shared_ptr<CGUILabel> > DLabels;
        std::shared_ptr<CGUIScrollBar> DScrollbar;

        std::vector< std::string > DBufferedLines;

        size_t DBaseLine;
        size_t DHightlightedBufferedLine;
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

        static bool WidgetDrawEvent(std::shared_ptr<CGUIWidget> widget, std::shared_ptr<CGraphicResourceContext> rc, TGUICalldata data);

        virtual void RefreshLabels();

    public:
        CGUIScrollableLabelBox(std::shared_ptr<CGUIFactory> guifactory, size_t initsize=8);
        virtual ~CGUIScrollableLabelBox();

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

