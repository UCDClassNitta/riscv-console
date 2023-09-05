#ifndef GUIFILECHOOSERDIALOG_H
#define GUIFILECHOOSERDIALOG_H

#include "GUIWindow.h"
#include "GUIFileFilter.h"

class CGUIFileChooserDialog : public virtual CGUIWindow{
    public:
        virtual ~CGUIFileChooserDialog() = default;
        
        virtual void SetFilter(std::shared_ptr<CGUIFileFilter> filter) = 0;
        virtual std::string GetCurrentFolder() = 0;
        virtual void SetCurrentFolder(const std::string &folder) = 0;
        virtual std::string GetFilename() = 0;
        
        virtual int Run() = 0;
};

#endif