#ifndef GUITREENODEVIEW_H
#define GUITREENODEVIEW_H

#include "GUIWidget.h"
#include "GUITreeNodeIter.h"

#include <vector>

class CGUITreeNodeView;

using TGUITreeNodeViewButtonEventCallback = bool (*)(std::shared_ptr<CGUITreeNodeView> widget, SGUIButtonEvent &event, TGUICalldata data);

class CGUITreeNodeView{
    public: 
        virtual ~CGUITreeNodeView() = default;

        virtual std::shared_ptr<CGUIWidget> ContainingWidget() const = 0;

        virtual std::shared_ptr<CGUITreeNodeIter> RootNode() = 0;

        virtual std::shared_ptr<CGUITreeNodeIter> GetNodeAtXY(int x, int y) const = 0;

        virtual bool GetCreateExpanded() const = 0;
        virtual void SetCreateExpanded(bool exp) = 0;

        virtual void AllowUpdates(bool allow) = 0;

        virtual void SetVerticalExpand(bool exp) = 0;
        virtual void SetHorizontalExpand(bool exp) = 0;
        
        virtual void SetButtonPressEventCallback(TGUICalldata calldata, TGUITreeNodeViewButtonEventCallback callback) = 0;
        virtual void SetButtonReleaseEventCallback(TGUICalldata calldata, TGUITreeNodeViewButtonEventCallback callback) = 0;
};

#endif
