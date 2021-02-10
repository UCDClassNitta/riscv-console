
#ifndef GUICOMBOBOX_H
#define GUICOMBOBOX_H
#include "GUIContainer.h"

class CGUIComboBox : public virtual CGUIContainer{
    public:
        virtual ~CGUIComboBox(){};
        
        virtual void SetFontFamily(const std::string &family) = 0;

        virtual int ItemCount() const = 0;
        virtual std::string GetItem(int item) const = 0;
        virtual void SetItem(int item, const std::string &str) = 0;
        virtual void AppendItem(const std::string &str) = 0;
        virtual void ClearItems() = 0;

        virtual int GetActiveItem() const = 0;
        virtual void SetActiveItem(int item) = 0;
};

#endif

