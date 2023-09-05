
#ifndef GUIAPPLICATIONGTK3_H
#define GUIAPPLICATIONGTK3_H
#include "GUIApplication.h"
#include "PeriodicTimeout.h"
#include <gtk/gtk.h>

class CGUIApplicationGTK3 : public CGUIApplication{
    protected:
        GtkApplication *DApplication;
        TGUICalldata DActivateCalldata;
        TGUIApplicationCallback DActivateCallback;
        
        std::shared_ptr< CPeriodicTimeout > DPeriodicTimeout;
        int DTimerHandle;
        TGUICalldata DTimerCalldata;
        TGUITimeoutCallback DTimerCallback;
        
        static void ActivateCallback(GtkApplication* app, gpointer data);
        static gboolean TimeoutCallback(gpointer data);
        
    public:
        CGUIApplicationGTK3(const std::string &appname);
        virtual ~CGUIApplicationGTK3();
        
        virtual void SetActivateCallback(TGUICalldata calldata, TGUIApplicationCallback callback) override;
        virtual void SetTimer(int timems, TGUICalldata calldata, TGUITimeoutCallback callback) override;
        
        virtual void ProcessEvents(bool block) override;
        
        virtual int Run(int argc, char *argv[]) override;
        virtual void Quit() override;
        
        virtual std::shared_ptr<CGUIWindow> NewWindow() override;
};

#endif

