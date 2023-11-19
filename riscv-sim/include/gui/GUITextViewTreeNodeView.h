#ifndef GUITEXTVIEWTREENODEVIEW_H
#define GUITEXTVIEWTREENODEVIEW_H

#include "GUITreeNodeView.h"
#include "GUIFactory.h"
#include <vector>

class CGUITextViewTreeNodeView : public CGUITreeNodeView, public std::enable_shared_from_this<CGUITextViewTreeNodeView> {
    protected:
        class CGUITextViewTreeNodeIter : public CGUITreeNodeIter, public std::enable_shared_from_this<CGUITextViewTreeNodeIter> {
            protected:
                std::string DText;
                bool DExpanded;
                std::string DTooltip;
                bool DTooltipIsMarkup;
                std::weak_ptr< CGUITextViewTreeNodeView > DTreeNodeView;
                std::weak_ptr< CGUITextViewTreeNodeIter > DParent;
                std::vector< std::shared_ptr< CGUITextViewTreeNodeIter > > DChildren;

                void MarkTreeDirty();

            public:
                CGUITextViewTreeNodeIter(std::shared_ptr< CGUITextViewTreeNodeView > treeview, std::shared_ptr< CGUITextViewTreeNodeIter > parent, const std::string &text, bool expanded);
                virtual ~CGUITextViewTreeNodeIter() = default;

                virtual bool GetExpanded() const override;
                virtual void SetExpanded(bool expand) override;
                virtual void SetExpandedAll(bool expand) override;

                virtual std::string GetText() const override;
                virtual bool SetText(const std::string &text) override;

                virtual std::string GetTooltip() const override;
                virtual bool GetTooltipIsMarkup() const override;
                virtual void SetTooltipText(const std::string &tip) override;
                virtual void SetTooltipMarkup(const std::string &markup) override;

                virtual size_t Depth() const override;
                virtual size_t ChildCount() const override;
                virtual std::shared_ptr<CGUITreeNodeIter> Parent() const override;
                virtual std::shared_ptr<CGUITreeNodeIter> GetChild(size_t index) const override;
                virtual std::shared_ptr<CGUITreeNodeIter> InsertChild(size_t index, const std::string &text) override;
                virtual bool RemoveChild(size_t index) override;
        };

        std::shared_ptr<CGUIFactory> DGUIFactory;
        std::shared_ptr<CGUIFrame> DContainingFrame;
        std::shared_ptr<CGUITextView> DTextView;
        std::shared_ptr<CGUIScrollWindow> DScrollWindow;
        std::shared_ptr<CGUITextViewTreeNodeIter> DRoot;
        std::vector<std::string> DLines;
        std::vector<std::shared_ptr<CGUITreeNodeIter> > DVisibleNodes;
        bool DAllowUpdates;
        bool DDirty;
        bool DCreateExpanded;

        TGUICalldata DButtonPressCalldata;
        TGUITreeNodeViewButtonEventCallback DButtonPressCallback;
        
        TGUICalldata DButtonReleaseCalldata;
        TGUITreeNodeViewButtonEventCallback DButtonReleaseCallback;

        static const char DVerticalBar[];
        static const char DHorizontalBar[];
        static const char DUpRightBar[];
        static const char DVerticalRightBar[];
        static const char DSquaredPlus[];
        static const char DSquaredMinus[];

        void Update();
        void UpdateNode(std::shared_ptr<CGUITreeNodeIter> node, const std::vector< bool > &morechildren);
        static bool WidgetMotionEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIMotionEvent &event, TGUICalldata data);
        static bool WidgetButtonEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);

    public: 
        CGUITextViewTreeNodeView(std::shared_ptr<CGUIFactory> guifactory, size_t minlines=8, size_t mincols=16);
        virtual ~CGUITextViewTreeNodeView();

        virtual std::shared_ptr<CGUIWidget> ContainingWidget() const override;

        virtual std::shared_ptr<CGUITreeNodeIter> RootNode() override;

        virtual std::shared_ptr<CGUITreeNodeIter> GetNodeAtXY(int x, int y) const override;

        virtual bool GetCreateExpanded() const override;
        virtual void SetCreateExpanded(bool exp) override;

        virtual void AllowUpdates(bool allow) override;

        void MarkDirty();

        virtual void SetVerticalExpand(bool exp) override;
        virtual void SetHorizontalExpand(bool exp) override;
        
        virtual void SetButtonPressEventCallback(TGUICalldata calldata, TGUITreeNodeViewButtonEventCallback callback) override;
        virtual void SetButtonReleaseEventCallback(TGUICalldata calldata, TGUITreeNodeViewButtonEventCallback callback) override;
};

#endif
