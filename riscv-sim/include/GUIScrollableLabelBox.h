
#ifndef GUISCROLLABLELABELBOX_H
#define GUISCROLLABLELABELBOX_H
#include "GUIFrame.h"
#include "GUIGrid.h"
#include "GUILabel.h"
#include "GUIEventBox.h"
#include "GUIScrollBar.h"
#include <string>
#include <vector>

class CGUIScrollableLabelBox{
    protected:
        std::shared_ptr<CGUIGrid> DContainingGrid;
        std::shared_ptr<CGUIFrame> DContainingFrame;
        std::vector< std::shared_ptr<CGUIEventBox > > DLabelEventBoxes;
        std::vector< std::shared_ptr<CGUILabel> > DLabels;
        std::shared_ptr<CGUIScrollBar> DScrollbar;

        std::vector< std::string > DBufferedLines;

        size_t DBaseLine;
        size_t DHightlightedBufferedLine;
        std::string DFontFamily;
        int DWidthCharacters;
        int DMaxWidthCharacters;

        static bool ScrollBarChangedEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);

        static bool WidgetScrollEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIScrollEvent &event, TGUICalldata data);

        static bool WidgetButtonEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);

        virtual bool ScrollBarChangedEvent(std::shared_ptr<CGUIWidget> widget);

        virtual bool WidgetScrollEvent(std::shared_ptr<CGUIWidget> widget, SGUIScrollEvent &event);

        virtual void RefreshLabels();

    public:
        CGUIScrollableLabelBox(size_t initsize=8);
        virtual ~CGUIScrollableLabelBox();

        virtual std::shared_ptr<CGUIWidget> ContainingWidget() const;

        virtual size_t GetBaseLine() const;
        virtual void SetBaseLine(size_t line);
        virtual size_t GetLineCount() const;
        virtual void SetLineCount(size_t count);

        virtual size_t GetBufferedLineCount() const;
        virtual std::string GetBufferedLine(size_t index) const;
        virtual void SetBufferedLines(const std::vector< std::string > &lines);

        virtual void UpdateBufferedLine(size_t index, const std::string &line);
        virtual void AppendBufferedLine(const std::string &line);
        virtual void HighlightBufferedLine(size_t index);

        virtual void SetFontFamily(const std::string &family);
        virtual void SetWidthCharacters(int chars);
        virtual void SetMaxWidthCharacters(int chars);

};

#endif

