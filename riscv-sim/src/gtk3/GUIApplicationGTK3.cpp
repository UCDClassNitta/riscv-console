#include "GUIApplicationGTK3.h"
#include "GUIWindowGTK3.h"
#include "Path.h"
#include <sys/stat.h>

CGUIApplicationGTK3::CGUIApplicationGTK3(const std::string &appname){
    DApplication = gtk_application_new (appname.c_str(), G_APPLICATION_FLAGS_NONE);
    //g_signal_connect(DApplication, "startup", G_CALLBACK (StartupCallback), NULL);
    g_signal_connect(DApplication, "activate", G_CALLBACK (ActivateCallback), this);
    
    DActivateCalldata = nullptr;
    DActivateCallback = nullptr;

    DTimerHandle = -1;
    DTimerCalldata = nullptr;
    DTimerCallback = nullptr;
    //DPeriodicTimeout = std::make_shared< CPeriodicTimeout > (50);
}

CGUIApplicationGTK3::~CGUIApplicationGTK3(){

}

void CGUIApplicationGTK3::ActivateCallback(GtkApplication* app, gpointer data){
    CGUIApplicationGTK3 *App = static_cast<CGUIApplicationGTK3 *>(data);
    if(App->DActivateCallback){
        App->DActivateCallback(App->DActivateCalldata);
    }
}

gboolean CGUIApplicationGTK3::TimeoutCallback(gpointer data){
    CGUIApplicationGTK3 *App = static_cast<CGUIApplicationGTK3 *>(data);
    if(App->DTimerCallback){
        int TimeoutMS;
        
        while((0 >= (TimeoutMS = App->DPeriodicTimeout->MiliSecondsUntilDeadline())) && App->DTimerCallback(App->DTimerCalldata)){
            App->DPeriodicTimeout->AcknowledgeDeadline();
        }
        if(TimeoutMS > 0){
            App->DTimerHandle = g_timeout_add(TimeoutMS, TimeoutCallback, data);
        }
    }
    return FALSE;
}

void CGUIApplicationGTK3::SetActivateCallback(TGUICalldata calldata, TGUIApplicationCallback callback){
    DActivateCalldata = calldata;
    DActivateCallback = callback;
}

void CGUIApplicationGTK3::SetTimer(int timems, TGUICalldata calldata, TGUITimeoutCallback callback){
    if(0 >= timems){
        if(0 <= DTimerHandle){
            g_source_remove(DTimerHandle);
            DTimerHandle = -1;
        }
        DTimerCalldata = nullptr;
        DTimerCallback = nullptr;
        return;
    }
    if(callback){
        if(0 <= DTimerHandle){
            g_source_remove(DTimerHandle);
            DTimerHandle = -1;
        }
        DTimerCalldata = calldata;
        DTimerCallback = callback;
        DPeriodicTimeout = std::make_shared< CPeriodicTimeout > (timems);
        int TimeoutMS = DPeriodicTimeout->MiliSecondsUntilDeadline();
        DTimerHandle = g_timeout_add(TimeoutMS < 1 ? 1 : TimeoutMS, TimeoutCallback, this);
    }
}

void CGUIApplicationGTK3::ProcessEvents(bool block){
    gtk_main_iteration_do(block ? TRUE : FALSE);
}

int CGUIApplicationGTK3::Run(int argc, char *argv[]){
    return g_application_run(G_APPLICATION(DApplication), argc, argv);
}

void CGUIApplicationGTK3::Quit(){
    g_application_quit(G_APPLICATION(DApplication));
}

std::shared_ptr<CGUIWindow> CGUIApplicationGTK3::NewWindow(){
    auto Window = gtk_application_window_new(DApplication);

    CPath HomeDirectory(getenv("HOME"));
    struct stat StatBuffer;
    if(-1 == stat(HomeDirectory.Simplify(std::string(".config/gtk-3.0/gtk.css")).ToString().c_str(),&StatBuffer)){
        std::string ToolTipCSS =    "@define-color base_color #ffffff;\n"
                                    "@define-color tooltip_bg_color #ffffff;\n"
                                    "@define-color tooltip_fg_color #000000;\n"
                                    "@define-color theme_tooltip_bg_color @tooltip_bg_color;\n"
                                    "@define-color theme_tooltip_fg_color @tooltip_fg_color;\n"
                                    "\n"
                                    "tooltip {\n"
                                    "  padding: 0px 0px 0px 0px;\n"
                                    "  margin: 0px 0px 0px 0px;\n"
                                    "  border-width: 1px;\n"
                                    "  border-style: none;\n"
                                    "  border-radius: 0px;\n"
                                    "  border-color: #ffffff;\n"
                                    "  background-image: none;\n"
                                    "  background-color: #ffffff;\n"
                                    "  color: #000000;\n"
                                    "  border: 0px;\n"
                                    "}\n"
                                    "\n"
                                    "tooltip * {\n"
                                    "  background-color: #ffffff;\n"
                                    "  font-weight: bold;\n"
                                    "  padding: 0px 0px 0px 0px;\n"
                                    "  margin: 0px 0px 0px 0px;\n"
                                    "  border-width: 0px;\n"
                                    "  border-style: none;\n"
                                    "  border-color: @tooltip_bg_color;\n"
                                    "  color: #000000;\n"
                                    "  text-shadow: 0px 0px 0px #bbbbbb, 0px 0px 0px #cccccc;\n"
                                    "}\n";
        auto NewProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(NewProvider, ToolTipCSS.c_str(), -1, NULL);
        auto DefaultScreen = gdk_screen_get_default();
        gtk_style_context_add_provider_for_screen(DefaultScreen,GTK_STYLE_PROVIDER(NewProvider),GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        


    }
    

    return std::make_shared<CGUIWindowGTK3>(Window);
}