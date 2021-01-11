#ifndef RISCVCONSOLEAPPLICATION_H
#define RISCVCONSOLEAPPLICATION_H

#include "GUIFactory.h"
#include "GraphicFactory.h"
#include "RISCVConsole.h"
#include <unordered_map>

class CRISCVConsoleApplication : public std::enable_shared_from_this<CRISCVConsoleApplication>{
    struct SPrivateConstructionType{};

    protected:
        static std::shared_ptr< CRISCVConsoleApplication > DApplicationPointer;
        std::shared_ptr<CGUIApplication> DApplication;
        bool DDeleted = false;

        std::shared_ptr<CGUIWindow> DMainWindow;
        std::shared_ptr<CGUIBox> DConsoleBox;
        std::shared_ptr<CGUIDrawingArea> DConsoleVideo;
        std::shared_ptr<CGraphicSurface> DDoubleBufferSurface;
        std::shared_ptr<CGraphicSurface> DWorkingBufferSurface;
        std::shared_ptr<CGUIGrid> DControllerGrid;
        std::shared_ptr<CGUIToggleButton> DUpButton;
        std::shared_ptr<CGUIToggleButton> DDownButton;
        std::shared_ptr<CGUIToggleButton> DLeftButton;
        std::shared_ptr<CGUIToggleButton> DRightButton;
        std::shared_ptr<CGUIButton> DCommandButton;
        std::shared_ptr<CGUIToggleButton> DButton1;
        std::shared_ptr<CGUIToggleButton> DButton2;
        std::shared_ptr<CGUIToggleButton> DButton3;
        std::shared_ptr<CGUIToggleButton> DButton4;
        std::shared_ptr<CGUIBox> DControlsBox;
        std::shared_ptr<CGUIGrid> DSystemControlGrid;
        std::shared_ptr<CGUIButton> DPowerButton;
        std::shared_ptr<CGUIButton> DResetButton;
        std::shared_ptr<CGUIButton> DFirmwareButton;
        std::shared_ptr<CGUIButton> DCartridgeButton;
        
        std::unordered_map<uint32_t, std::shared_ptr<CGUIToggleButton> > DKeyControllerMapping;
        std::unordered_map<std::shared_ptr<CGUIToggleButton>, CRISCVConsole::EDirection> DDirectionButtonMapping;
        std::unordered_map<std::shared_ptr<CGUIToggleButton>, CRISCVConsole::EButtonNumber> DButtonNumberButtonMapping;

        std::shared_ptr<CRISCVConsole> DRISCVConsole;

        std::string DFileOpenFolder;

        static void ActivateCallback(TGUICalldata data);
        static bool TimeoutCallback(TGUICalldata data);
        static bool MainWindowDeleteEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        static void MainWindowDestroyCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        static bool MainWindowKeyPressEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event, TGUICalldata data);
        static bool MainWindowKeyReleaseEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event, TGUICalldata data);
        static bool MainWindowConfigureEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIConfigureEvent &event, TGUICalldata data);
        static bool DrawingAreaDrawCallback(std::shared_ptr<CGUIWidget> widget, std::shared_ptr<CGraphicResourceContext> rc, TGUICalldata data);
        static bool FirmwareButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool CartridgeButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool ControllerButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool CommandButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool ResetButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool PowerButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        

        void Activate();
        bool Timeout();
        bool MainWindowDeleteEvent(std::shared_ptr<CGUIWidget> widget);
        void MainWindowDestroy(std::shared_ptr<CGUIWidget> widget);
        bool MainWindowKeyPressEvent(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event);
        bool MainWindowKeyReleaseEvent(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event);
        bool MainWindowConfigureEvent(std::shared_ptr<CGUIWidget> widget, SGUIConfigureEvent &event);
        bool DrawingAreaDraw(std::shared_ptr<CGUIWidget> widget, std::shared_ptr<CGraphicResourceContext> rc);
        bool FirmwareButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool CartridgeButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool ControllerButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool CommandButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool ResetButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool PowerButtonToggledEvent(std::shared_ptr<CGUIWidget> widget);
        void UpdateConsoleButtonChange(std::shared_ptr<CGUIToggleButton> button);

    public:
        explicit CRISCVConsoleApplication(const std::string &appname, const SPrivateConstructionType &key);
        ~CRISCVConsoleApplication();
        
        static std::shared_ptr< CRISCVConsoleApplication > Instance(const std::string &appname);
        
        int Run(int argc, char *argv[]);

};



#endif