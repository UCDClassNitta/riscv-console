#ifndef VARIABLETREEVIEWDECORATOR_H
#define VARIABLETREEVIEWDECORATOR_H

#include "VariableTranslator.h"
#include "GUITreeNodeView.h"
#include <unordered_map>

class CVariableTreeViewDecorator{
    protected:
        std::shared_ptr< CGUITreeNodeView > DTreeNodeView;
        std::unordered_map< std::string, bool > DExpansions;
        std::vector< std::string > DDepthFirstNodePaths;
        size_t DNameValueGap;
        size_t DDepthIndention;
        size_t DMaxWidth;
        size_t DPathIndex;

        static std::string CreateTooltipMarkup(std::shared_ptr<CVariableTranslator::CProgramState> state);
        void UpdateExpansions(std::shared_ptr<CGUITreeNodeIter> node);
        size_t CalculateMaxWidth(std::shared_ptr<CVariableTranslator::CProgramState> state, size_t depth) const;
        void DecorateNode(std::shared_ptr<CGUITreeNodeIter> parent, const std::string &path, std::shared_ptr<CVariableTranslator::CProgramState> state, size_t siblingindex, size_t depth);

    public:
        CVariableTreeViewDecorator(std::shared_ptr< CGUITreeNodeView > treeview);
        ~CVariableTreeViewDecorator() = default;

        void DecorateTreeView(const std::vector< std::shared_ptr<CVariableTranslator::CProgramState> > &states);
};

#endif
