
#ifndef GUIFILECHOOSERDIALOG_H
#define GUIFILECHOOSERDIALOG_H
#include "GUIWindow.h"

class CGUIFileFilter{
    public:
        virtual ~CGUIFileFilter(){};
        
        virtual void AddPattern(const std::string &pattern) = 0;
};

class CGUIFileChooserDialog : public virtual CGUIWindow{
    public:
        virtual ~CGUIFileChooserDialog(){};
        
        virtual void SetFilter(std::shared_ptr<CGUIFileFilter> filter) = 0;
        virtual std::string GetCurrentFolder() = 0;
        virtual void SetCurrentFolder(const std::string &folder) = 0;
        virtual std::string GetFilename() = 0;
        
        virtual int Run() = 0;
};

#endif

