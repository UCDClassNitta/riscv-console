#ifndef RISCVCONSOLEAPPLICATION_H
#define RISCVCONSOLEAPPLICATION_H

#include "GUIFactory.h"
#include "GUIScrollableLabelBox.h"
#include "GUIScrollableMemoryLabelBox.h"
#include "GraphicFactory.h"
#include "RISCVConsole.h"
#include "RISCVConsoleApplicationConfig.h"
#include "AutoRecorder.h"
#include <unordered_map>

class CRISCVConsoleApplication : public std::enable_shared_from_this<CRISCVConsoleApplication>{
    struct SPrivateConstructionType{};

    protected:
        static std::shared_ptr< CRISCVConsoleApplication > DApplicationPointer;
        std::shared_ptr<CGUIApplication> DApplication;
        CRISCVConsoleApplicationConfiguration DConfiguration;
        bool DDeleted = false;
        bool DDebugMode = false;
        bool DFollowingInstruction = false;

        std::shared_ptr<CGUIWindow> DMainWindow;
        std::shared_ptr<CGUIBox> DConsoleDebugBox;
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
        std::shared_ptr<CGUIToggleButton> DPowerButton;
        std::shared_ptr<CGUIButton> DResetButton;
        std::shared_ptr<CGUIButton> DFirmwareButton;
        std::shared_ptr<CGUIToggleButton> DCartridgeButton;
        std::shared_ptr<CGUIBox> DDebugBox;
        std::shared_ptr<CGUIGrid> DRegisterGrid;
        std::vector< std::shared_ptr< CGUILabel > > DGeneralRegisterNameLabels;
        std::vector< std::shared_ptr< CGUILabel > > DGeneralRegisterValueLabels;
        std::shared_ptr< CGUILabel > DProgramCounterNameLabel;
        std::shared_ptr< CGUILabel > DProgramCounterValueLabel;

        std::shared_ptr<CGUIBox> DDebugControlBox;
        std::shared_ptr<CGUIToggleButton> DDebugRunButton;
        std::shared_ptr<CGUIButton> DDebugStepButton;
        std::shared_ptr<CGUIButton> DDebugClearButton;
        std::shared_ptr<CGUIToggleButton> DDebugRecordButton;
        
        std::shared_ptr<CGUIComboBox> DDebugInstructionComboBox;
        std::shared_ptr<CGUIScrollableLabelBox> DDebugInstructions;

        std::shared_ptr<CGUIScrollableLabelBox> DDebugCSRegisters;

        std::shared_ptr<CGUIScrollableMemoryLabelBox> DDebugMemory;
        std::shared_ptr<CGUIButton> DDebugMemoryFirmwareButton;
        std::shared_ptr<CGUIButton> DDebugMemoryCartridgeButton;
        std::shared_ptr<CGUIButton> DDebugMemoryRegistersButton;
        std::shared_ptr<CGUIButton> DDebugMemoryVideoButton;
        std::shared_ptr<CGUIButton> DDebugMemoryDataButton;
        std::shared_ptr<CGUIToggleButton> DDebugMemoryStackButton;
        std::unordered_map<std::shared_ptr<CGUIButton>, uint32_t> DDebugMemoryButtonMapping;
        std::unordered_map<uint32_t, std::vector<uint32_t> > DDebugMemorySubSectionMapping;
        std::unordered_map<uint32_t, uint32_t > DDebugMemorySubSectionIndex;
        uint32_t DLastMemoryBaseAddress;
        uint32_t DDebugMemoryGlobalPointerRegisterIndex;
        uint32_t DDebugMemoryStackPointerRegisterIndex;


        std::unordered_map<uint32_t, std::shared_ptr<CGUIToggleButton> > DKeyControllerMapping;
        std::unordered_map<std::shared_ptr<CGUIToggleButton>, CRISCVConsole::EDirection> DDirectionButtonMapping;
        std::unordered_map<std::shared_ptr<CGUIToggleButton>, CRISCVConsole::EButtonNumber> DButtonNumberButtonMapping;
        std::unordered_map<uint32_t, bool> DKeyZoomMapping;

        std::shared_ptr<CAutoRecorder> DInputRecorder;
        std::shared_ptr<CRISCVConsole> DRISCVConsole;

        std::string DFirmwareFileOpenFolder;
        std::string DCartridgeFileOpenFolder;
        std::string DRecordFileOpenFolder;
        bool DCartridgeInLoading;

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
        static bool CartridgeButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        static bool EjectButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool ControllerButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool CommandButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool ResetButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool PowerButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        static bool DebugMemoryButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool DebugMemoryStackButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        static bool RunButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        static bool StepButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool ClearButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool RecordButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        static bool InstructionComboBoxChangedEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        static bool InstructionBoxButtonEventCallback(std::shared_ptr<CGUIScrollableLabelBox> widget, SGUIButtonEvent &event, size_t line, TGUICalldata data);
        static bool InstructionBoxScrollEventCallback(std::shared_ptr<CGUIScrollableLabelBox> widget, TGUICalldata data);
        static void BreakpointEventCallback(CRISCVConsoleBreakpointCalldata data);

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
        bool CartridgeButtonToggledEvent(std::shared_ptr<CGUIWidget> widget);
        bool EjectButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool ControllerButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool CommandButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool ResetButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool PowerButtonToggledEvent(std::shared_ptr<CGUIWidget> widget);
        void UpdateConsoleButtonChange(std::shared_ptr<CGUIToggleButton> button);
        bool DebugMemoryButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool DebugMemoryStackButtonToggledEvent(std::shared_ptr<CGUIWidget> widget);
        bool RunButtonToggledEvent(std::shared_ptr<CGUIWidget> widget);
        bool StepButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool ClearButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool RecordButtonToggledEvent(std::shared_ptr<CGUIWidget> widget);
        bool InstructionComboBoxChangedEvent(std::shared_ptr<CGUIWidget> widget);
        bool InstructionBoxButtonEvent(std::shared_ptr<CGUIScrollableLabelBox> widget, SGUIButtonEvent &event, size_t line);
        bool InstructionBoxScrollEvent(std::shared_ptr<CGUIScrollableLabelBox> widget);
        void BreakpointEvent();

        void CreateConsoleWidgets();
        void CreateControllerWidgets();
        void CreateSystemControlWidgets();

        void CreateDebugWidgets();
        void CreateDebugRegisterWidgets();
        void CreateDebugControlWidgets();
        void CreateDebugInstructionWidgets();
        void CreateDebugCSRWidgets();
        void CreateDebugMemoryWidgets();

        bool ParseInstructionLine(size_t line, uint32_t &addr, bool &breakpoint);

        void SetKeyControllerMapping(const std::string &label, std::shared_ptr<CGUIToggleButton> button);
        void SetKeyZoomMapping(const std::string &keys, bool zoomin);
        uint32_t GetScreenTimeoutMS();
        uint32_t GetWidgetSpacing();
        uint32_t GetInstructionLineCount();
        uint32_t GetMemoryLineCount();
        uint32_t GetTimerUS();
        uint32_t GetCPUFrequency();

        void RefreshDebugRegisters();

        void ParseArguments(int &argc, char *argv[]);

        static std::string FormatHex32Bit(uint32_t val);

    public:
        explicit CRISCVConsoleApplication(const std::string &appname, const SPrivateConstructionType &key);
        ~CRISCVConsoleApplication();
        
        static std::shared_ptr< CRISCVConsoleApplication > Instance(const std::string &appname);
        
        int Run(int argc, char *argv[]);

};



#endif