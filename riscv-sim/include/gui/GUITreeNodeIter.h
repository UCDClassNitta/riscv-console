#ifndef GUITREENODEITER_H
#define GUITREENODEITER_H

#include <string>
#include <memory>

class CGUITreeNodeIter{
    public:
        virtual ~CGUITreeNodeIter() = default;

        virtual bool GetExpanded() const = 0;
        virtual void SetExpanded(bool expand) = 0;
        virtual void SetExpandedAll(bool expand) = 0;

        virtual std::string GetText() const = 0;
        virtual bool SetText(const std::string &text) = 0;

        virtual std::string GetTooltip() const = 0;
        virtual bool GetTooltipIsMarkup() const = 0;
        virtual void SetTooltipText(const std::string &tip) = 0;
        virtual void SetTooltipMarkup(const std::string &markup) = 0;

        virtual size_t Depth() const = 0;
        virtual size_t ChildCount() const = 0;
        virtual std::shared_ptr<CGUITreeNodeIter> Parent() const = 0;
        virtual std::shared_ptr<CGUITreeNodeIter> GetChild(size_t index) const = 0;
        virtual std::shared_ptr<CGUITreeNodeIter> InsertChild(size_t index, const std::string &text) = 0;
        virtual bool RemoveChild(size_t index) = 0;
};

#endif
