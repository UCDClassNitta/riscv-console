
#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H
#include "GUIEvent.h"
#include "GUIWindow.h"

class CGUIApplication{
    public:
        CGUIApplication(){};
        virtual ~CGUIApplication(){};
        
        virtual void SetActivateCallback(TGUICalldata calldata, TGUIApplicationCallback callback) = 0;
        virtual void SetTimer(int timems, TGUICalldata calldata, TGUITimeoutCallback callback) = 0;
        
        virtual void ProcessEvents(bool block) = 0;
        
        virtual int Run(int argc, char *argv[]) = 0;
        virtual void Quit() = 0;
        
        virtual std::shared_ptr<CGUIWindow> NewWindow() = 0;
};

#endif

