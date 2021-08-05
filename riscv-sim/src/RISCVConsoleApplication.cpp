#include "RISCVConsoleApplication.h"
#include "FileDataSink.h"
#include "FileDataSource.h"
#include "Path.h"
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iomanip>

std::shared_ptr< CRISCVConsoleApplication > CRISCVConsoleApplication::DApplicationPointer;

CRISCVConsoleApplication::CRISCVConsoleApplication(const std::string &appname, const SPrivateConstructionType &key){
    std::string ConfigFilePath = getenv("HOME");
    
    ConfigFilePath += ConfigFilePath.back() == '/' ? ".riscv_console.cfg" : "/.riscv_console.cfg";
    auto ConfigFile = std::make_shared<CFileDataSource>(ConfigFilePath);
    DConfiguration.Load(ConfigFile);

    DFileOpenFolder = CPath::CurrentPath().ToString();
    DApplication = CGUIFactory::ApplicationInstance(appname);
    
    DRISCVConsole = std::make_shared<CRISCVConsole>(GetTimerUS(),GetScreenTimeoutMS(),GetCPUFrequency());
    DInputRecorder = std::make_shared<CAutoRecorder>(GetTimerUS(),GetScreenTimeoutMS(),GetCPUFrequency());
    DApplication->SetActivateCallback(this, ActivateCallback);
    
}

CRISCVConsoleApplication::~CRISCVConsoleApplication(){
    
}

void CRISCVConsoleApplication::ActivateCallback(TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    App->Activate();
}

bool CRISCVConsoleApplication::TimeoutCallback(TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    if(App->DDeleted){
        return false;   
    }
    
    return App->Timeout();
}

bool CRISCVConsoleApplication::MainWindowDeleteEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->MainWindowDeleteEvent(widget);
}

void CRISCVConsoleApplication::MainWindowDestroyCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    App->MainWindowDestroy(widget);
}

bool CRISCVConsoleApplication::MainWindowKeyPressEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->MainWindowKeyPressEvent(widget,event);
}

bool CRISCVConsoleApplication::MainWindowKeyReleaseEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->MainWindowKeyReleaseEvent(widget,event);
}

bool CRISCVConsoleApplication::MainWindowConfigureEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIConfigureEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->MainWindowConfigureEvent(widget,event);
}

bool CRISCVConsoleApplication::DrawingAreaDrawCallback(std::shared_ptr<CGUIWidget> widget, std::shared_ptr<CGraphicResourceContext> rc, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->DrawingAreaDraw(widget, rc);
}

bool CRISCVConsoleApplication::FirmwareButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->FirmwareButtonClickEvent(widget,event);
}

bool CRISCVConsoleApplication::CartridgeButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->CartridgeButtonToggledEvent(widget);
}

bool CRISCVConsoleApplication::ControllerButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->ControllerButtonClickEvent(widget,event);
}

bool CRISCVConsoleApplication::CommandButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->CommandButtonClickEvent(widget,event);
}

bool CRISCVConsoleApplication::ResetButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->ResetButtonClickEvent(widget,event);
}

bool CRISCVConsoleApplication::PowerButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->PowerButtonToggledEvent(widget);
}

bool CRISCVConsoleApplication::DebugMemoryButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->DebugMemoryButtonClickEvent(widget,event);
}

bool CRISCVConsoleApplication::DebugMemoryStackButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->DebugMemoryStackButtonToggledEvent(widget);
}

bool CRISCVConsoleApplication::RunButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->RunButtonToggledEvent(widget);
}

bool CRISCVConsoleApplication::StepButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->StepButtonClickEvent(widget,event);
}

bool CRISCVConsoleApplication::ClearButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->ClearButtonClickEvent(widget,event);
}

bool CRISCVConsoleApplication::RecordButtonToggledEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->RecordButtonToggledEvent(widget);
}

bool CRISCVConsoleApplication::InstructionComboBoxChangedEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->InstructionComboBoxChangedEvent(widget);
}

bool CRISCVConsoleApplication::InstructionBoxButtonEventCallback(std::shared_ptr<CGUIScrollableLabelBox> widget, SGUIButtonEvent &event, size_t line, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->InstructionBoxButtonEvent(widget,event,line);
}

bool CRISCVConsoleApplication::InstructionBoxScrollEventCallback(std::shared_ptr<CGUIScrollableLabelBox> widget, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->InstructionBoxScrollEvent(widget);
}

void CRISCVConsoleApplication::BreakpointEventCallback(CRISCVConsoleBreakpointCalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    App->BreakpointEvent();
}

void CRISCVConsoleApplication::Activate(){
    DRISCVConsole->SetDebugMode(DDebugMode);
    if(DDebugMode){
        DRISCVConsole->SetBreakcpointCallback(this,BreakpointEventCallback);
    }
    DMainWindow = DApplication->NewWindow();
    DMainWindow->SetDeleteEventCallback(this, MainWindowDeleteEventCallback);
    DMainWindow->SetDestroyEventCallback(this, MainWindowDestroyCallback);
   
    DMainWindow->SetKeyPressEventCallback(this, MainWindowKeyPressEventCallback);
    DMainWindow->SetKeyReleaseEventCallback(this, MainWindowKeyReleaseEventCallback);    

    CreateConsoleWidgets();
    
    if(DDebugMode){
        CreateDebugWidgets();
    }

    DMainWindow->Add(DConsoleDebugBox ? DConsoleDebugBox : DConsoleBox);
    DMainWindow->ShowAll();
    
    DApplication->SetTimer(1,this,TimeoutCallback);
}

bool CRISCVConsoleApplication::Timeout(){
    if(DRISCVConsole->VideoTimerTick(DDoubleBufferSurface)){
        DConsoleVideo->Invalidate();
    }

    return true;
}

bool CRISCVConsoleApplication::MainWindowDeleteEvent(std::shared_ptr<CGUIWidget> widget){
    DDeleted = true;
    return false;  
}

void CRISCVConsoleApplication::MainWindowDestroy(std::shared_ptr<CGUIWidget> widget){
    DMainWindow = nullptr;
}

bool CRISCVConsoleApplication::MainWindowKeyPressEvent(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event){
    auto ButtonSearch = DKeyControllerMapping.find(event.DValue.DValue);
    if(ButtonSearch != DKeyControllerMapping.end()){
        ButtonSearch->second->SetActive(true);
        UpdateConsoleButtonChange(ButtonSearch->second);
    }
    return true;
}

bool CRISCVConsoleApplication::MainWindowKeyReleaseEvent(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event){
    auto ButtonSearch = DKeyControllerMapping.find(event.DValue.DValue);
    if(ButtonSearch != DKeyControllerMapping.end()){
        ButtonSearch->second->SetActive(false);
        UpdateConsoleButtonChange(ButtonSearch->second);
    }
    return true;
}

bool CRISCVConsoleApplication::MainWindowConfigureEvent(std::shared_ptr<CGUIWidget> widget, SGUIConfigureEvent &event){
    return true;
}

bool CRISCVConsoleApplication::DrawingAreaDraw(std::shared_ptr<CGUIWidget> widget, std::shared_ptr<CGraphicResourceContext> rc){
    rc->DrawSurface(DDoubleBufferSurface, 0, 0, -1, -1, 0, 0);
    return true;
}

bool CRISCVConsoleApplication::FirmwareButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event){
    std::string Filename;
    {
        auto FileChooser = CGUIFactory::NewFileChooserDialog("Select Firmware",true,DMainWindow);
        FileChooser->SetCurrentFolder(DFileOpenFolder);
        if(FileChooser->Run()){
            Filename = FileChooser->GetFilename();
            DFileOpenFolder = FileChooser->GetCurrentFolder();
            auto InFile = std::make_shared<CFileDataSource>(Filename);
            if(DRISCVConsole->ProgramFirmware(InFile)){
                if(DDebugMode){
                    std::string FirmwareRelativePath = CPath::RelativePath(CPath::CurrentPath(),Filename);
                    DInputRecorder->AddFWEvent(FirmwareRelativePath);
                    DDebugInstructions->SetBufferedLines(DRISCVConsole->InstructionStrings());
                    DDebugInstructionComboBox->ClearItems();
                    for(auto &Label : DRISCVConsole->InstructionLabels()){
                        DDebugInstructionComboBox->AppendItem(Label);
                    }
                    DFollowingInstruction = true;
                    RefreshDebugRegisters();
                }
            }
        }
    }
    

    return true;
}

bool CRISCVConsoleApplication::CartridgeButtonToggledEvent(std::shared_ptr<CGUIWidget> widget){
    if(DCartridgeButton->GetActive()){
        DCartridgeInLoading = true;
        std::string Filename;
        auto FileChooser = CGUIFactory::NewFileChooserDialog("Select Cartridge",true,DMainWindow);
        FileChooser->SetCurrentFolder(DFileOpenFolder);
        if(FileChooser->Run()){
            Filename = FileChooser->GetFilename();
            DFileOpenFolder = FileChooser->GetCurrentFolder();
            auto InFile = std::make_shared<CFileDataSource>(Filename);
            auto EventCycle = DRISCVConsole->InsertCartridge(InFile);
            if(EventCycle != std::numeric_limits<uint64_t>::max()){
                if(DDebugMode){
                    std::string CartridgeRelativePath = CPath::RelativePath(CPath::CurrentPath(),Filename);
                    DInputRecorder->AddInsertCREvent(CartridgeRelativePath,EventCycle);
                    DDebugInstructions->SetBufferedLines(DRISCVConsole->InstructionStrings());
                    DDebugInstructionComboBox->ClearItems();
                    for(auto &Label : DRISCVConsole->InstructionLabels()){
                        DDebugInstructionComboBox->AppendItem(Label);
                    }
                    RefreshDebugRegisters();
                }
            }
            else{
                DCartridgeButton->SetActive(false);
            }
        }
        DCartridgeInLoading = false;
    }
    else if(!DCartridgeInLoading){
        auto EventCycle = DRISCVConsole->RemoveCartridge();
        if(DDebugMode){
            DInputRecorder->AddRemoveCREvent(EventCycle);
            DDebugInstructions->SetBufferedLines(DRISCVConsole->InstructionStrings());
            DDebugInstructionComboBox->ClearItems();
            for(auto &Label : DRISCVConsole->InstructionLabels()){
                DDebugInstructionComboBox->AppendItem(Label);
            }
            RefreshDebugRegisters();
        }
    }
    
    return true;
}

bool CRISCVConsoleApplication::ControllerButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event){
    auto ToggleButton = std::dynamic_pointer_cast<CGUIToggleButton>(widget);
    ToggleButton->SetActive(!ToggleButton->GetActive());
    UpdateConsoleButtonChange(ToggleButton);
    return true;
}

bool CRISCVConsoleApplication::CommandButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event){
    auto Cycle = DRISCVConsole->PressCommand();
    DInputRecorder->AddCommandPressEvent(Cycle);
    return true;
}

bool CRISCVConsoleApplication::ResetButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event){
    DRISCVConsole->Reset();
    if(DDebugMode){
        RefreshDebugRegisters();
    }
    return true;
}

bool CRISCVConsoleApplication::PowerButtonToggledEvent(std::shared_ptr<CGUIWidget> widget){
    if(std::dynamic_pointer_cast<CGUIToggleButton>(widget)->GetActive()){
        DRISCVConsole->PowerOn();
    }
    else{
        DRISCVConsole->PowerOff();
        if(DDebugMode){
            DDebugRunButton->SetActive(false);
        }
    }
    
    return true;
}

void CRISCVConsoleApplication::UpdateConsoleButtonChange(std::shared_ptr<CGUIToggleButton> button){
    auto DirectionSearch = DDirectionButtonMapping.find(button);
    uint64_t Cycle;
    if(DirectionSearch != DDirectionButtonMapping.end()){
        if(button->GetActive()){
            Cycle = DRISCVConsole->PressDirection(DirectionSearch->second);
            DInputRecorder->AddDirectionPressEvent(DirectionSearch->second, Cycle);
        }
        else{
            Cycle = DRISCVConsole->ReleaseDirection(DirectionSearch->second);
            DInputRecorder->AddDirectionReleaseEvent(DirectionSearch->second, Cycle);
        }
        return;
    }
    auto ButtonSearch = DButtonNumberButtonMapping.find(button);
    if(ButtonSearch != DButtonNumberButtonMapping.end()){
        if(button->GetActive()){
            Cycle = DRISCVConsole->PressButton(ButtonSearch->second);
            DInputRecorder->AddButtonPressEvent(ButtonSearch->second, Cycle);
        }
        else{
            Cycle = DRISCVConsole->ReleaseButton(ButtonSearch->second);
            DInputRecorder->AddButtonReleaseEvent(ButtonSearch->second, Cycle);
        }
    }
}

bool CRISCVConsoleApplication::DebugMemoryButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event){
    auto MemoryButton = std::dynamic_pointer_cast<CGUIButton>(widget);
    auto Search = DDebugMemoryButtonMapping.find(MemoryButton);
    if(Search != DDebugMemoryButtonMapping.end()){
        DDebugMemory->SetBaseAddress(Search->second,true);
    }
    else{
        auto RegisterBase = DRISCVConsole->CPU()->Register(DDebugMemoryGlobalPointerRegisterIndex);
        RegisterBase = RegisterBase < 0x800 ? RegisterBase : RegisterBase - 0x800;
        DDebugMemory->SetBaseAddress(RegisterBase,true);
        
    }
    if(DDebugMemoryStackButton->GetActive()){
        DDebugMemoryStackButton->SetActive(false);
    }
    return true;
}

bool CRISCVConsoleApplication::DebugMemoryStackButtonToggledEvent(std::shared_ptr<CGUIWidget> widget){
    auto StackButton = std::dynamic_pointer_cast<CGUIToggleButton>(widget);

    if(StackButton->GetActive()){
        auto RegisterBase = DRISCVConsole->CPU()->Register(DDebugMemoryStackPointerRegisterIndex);
        DDebugMemory->SetBaseAddress(RegisterBase,false);
        DDebugMemory->Refresh();
    }
    return false;
}

bool CRISCVConsoleApplication::RunButtonToggledEvent(std::shared_ptr<CGUIWidget> widget){
    if(std::dynamic_pointer_cast<CGUIToggleButton>(widget)->GetActive()){
        if(!DPowerButton->GetActive()){
            DPowerButton->SetActive(true);
        }
        DRISCVConsole->Run();
    }
    else{
        DRISCVConsole->Stop();
        RefreshDebugRegisters();
    }
    
    return true;
}

bool CRISCVConsoleApplication::StepButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event){
    if(!DPowerButton->GetActive()){
        DPowerButton->SetActive(true);
    }
    DRISCVConsole->Step();
    RefreshDebugRegisters();
    return true;
}

bool CRISCVConsoleApplication::ClearButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event){
    // Clear break points
    for(auto &Addr : DRISCVConsole->Breakpoints()){
        auto LineIndex = DRISCVConsole->InstructionAddressesToIndices(Addr);
        auto Line = DDebugInstructions->GetBufferedLine(LineIndex);
        Line[0] = ' ';
        DDebugInstructions->UpdateBufferedLine(LineIndex, Line);
    }
    DRISCVConsole->ClearBreakpoints();
    return true;
}

bool CRISCVConsoleApplication::RecordButtonToggledEvent(std::shared_ptr<CGUIWidget> widget){

    if(!DDebugRecordButton->GetActive()){
        std::string Filename;
        auto FileChooser = CGUIFactory::NewFileChooserDialog("Save Record",false,DMainWindow);
        FileChooser->SetCurrentFolder(DFileOpenFolder);
        if(FileChooser->Run()){
            Filename = FileChooser->GetFilename();
            DRISCVConsole->Stop();
            DInputRecorder->OutputJSONFile(Filename);
            RefreshDebugRegisters();
        }
    }
    else{
        DInputRecorder->ResetRecord();
    }
    
    return true;
}

bool CRISCVConsoleApplication::InstructionComboBoxChangedEvent(std::shared_ptr<CGUIWidget> widget){
    auto ItemNumber = DDebugInstructionComboBox->GetActiveItem();
    if(ItemNumber < DRISCVConsole->InstructionLabelIndices().size()){
        DDebugInstructions->SetBaseLine(DRISCVConsole->InstructionLabelIndices()[ItemNumber]);
    }
    
    return true;
}

bool CRISCVConsoleApplication::InstructionBoxButtonEvent(std::shared_ptr<CGUIScrollableLabelBox> widget, SGUIButtonEvent &event, size_t line){
    if(event.DType.IsDoubleButtonPress()){
        uint32_t Address;
        bool Breakpoint;
        if(ParseInstructionLine(line,Address,Breakpoint)){
            auto Line = DDebugInstructions->GetBufferedLine(line);
            Line[0] = Breakpoint ? ' ' : '@';
            DDebugInstructions->UpdateBufferedLine(line, Line);
            
            if(Breakpoint){
                DRISCVConsole->RemoveBreakpoint(Address);
            }
            else{
                DRISCVConsole->AddBreakpoint(Address);
            }
        }
    }
    return true;
}

bool CRISCVConsoleApplication::InstructionBoxScrollEvent(std::shared_ptr<CGUIScrollableLabelBox> widget){
    DFollowingInstruction = (widget->GetBaseLine() <= widget->GetHighlightedBufferedLine()) && (widget->GetHighlightedBufferedLine() < widget->GetBaseLine() + widget->GetLineCount());
    return true;
}

void CRISCVConsoleApplication::BreakpointEvent(){
    DFollowingInstruction = true;
    DDebugRunButton->SetActive(false);
}

std::shared_ptr< CRISCVConsoleApplication > CRISCVConsoleApplication::Instance(const std::string &appname){
    if(!DApplicationPointer){
        DApplicationPointer = std::make_shared< CRISCVConsoleApplication >(appname, SPrivateConstructionType{});   
    }
    return DApplicationPointer;
}

void CRISCVConsoleApplication::CreateConsoleWidgets(){
    DConsoleBox = CGUIFactory::NewBox(CGUIBox::EOrientation::Vertical,GetWidgetSpacing());
    DConsoleVideo = CGUIFactory::NewDrawingArea();
    CreateControllerWidgets();
    CreateSystemControlWidgets();

    DControlsBox = CGUIFactory::NewBox(CGUIBox::EOrientation::Horizontal,GetWidgetSpacing());
    DControlsBox->PackStart(DControllerGrid,false,false,GetWidgetSpacing());
    DControlsBox->PackStart(DSystemControlGrid,false,false,GetWidgetSpacing());

    DConsoleBox->PackStart(DConsoleVideo,false,false,GetWidgetSpacing());
    DConsoleBox->PackStart(DControlsBox,false,false,GetWidgetSpacing());
    
    DDoubleBufferSurface = CGraphicFactory::CreateSurface(DRISCVConsole->ScreenWidth(), DRISCVConsole->ScreenHeight(), ESurfaceFormat::ARGB32);
    DWorkingBufferSurface = CGraphicFactory::CreateSurface(DRISCVConsole->ScreenWidth(), DRISCVConsole->ScreenHeight(), ESurfaceFormat::ARGB32);
    DConsoleVideo->SetSizeRequest(DRISCVConsole->ScreenWidth(), DRISCVConsole->ScreenHeight());
    DConsoleVideo->SetDrawEventCallback(this, DrawingAreaDrawCallback);
    auto TempRC = DDoubleBufferSurface->CreateResourceContext();
    TempRC->SetSourceRGB(0x0);
    TempRC->SetLineWidth(1);
    TempRC->Rectangle(0,0,DRISCVConsole->ScreenWidth(), DRISCVConsole->ScreenHeight());
    TempRC->StrokePreserve();
    TempRC->Fill();    
    DConsoleVideo->Invalidate();
}

void CRISCVConsoleApplication::CreateControllerWidgets(){
    DUpButton = CGUIFactory::NewToggleButton();
    DDownButton = CGUIFactory::NewToggleButton();
    DLeftButton = CGUIFactory::NewToggleButton();
    DRightButton = CGUIFactory::NewToggleButton();
    DButton1 = CGUIFactory::NewToggleButton();
    DButton2 = CGUIFactory::NewToggleButton();
    DButton3 = CGUIFactory::NewToggleButton();
    DButton4 = CGUIFactory::NewToggleButton();
    DCommandButton = CGUIFactory::NewButton();
    DControllerGrid = CGUIFactory::NewGrid();
    DControllerGrid->Attach(DUpButton,1,0,1,1);
    DControllerGrid->Attach(DLeftButton,0,1,1,1);
    DControllerGrid->Attach(DRightButton,2,1,1,1);
    DControllerGrid->Attach(DDownButton,1,2,1,1);
    DControllerGrid->Attach(DCommandButton,3,1,1,1);
    DControllerGrid->Attach(DButton1,4,0,1,1);
    DControllerGrid->Attach(DButton2,6,0,1,1);
    DControllerGrid->Attach(DButton3,4,2,1,1);
    DControllerGrid->Attach(DButton4,6,2,1,1);
    DControllerGrid->SetRowSpacing(GetWidgetSpacing());
    DControllerGrid->SetColumnSpacing(GetWidgetSpacing());

    SetKeyControllerMapping(DConfiguration.GetStringParameter(CRISCVConsoleApplicationConfiguration::EParameter::UpKey), DUpButton);
    DUpButton->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DUpButton->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    SetKeyControllerMapping(DConfiguration.GetStringParameter(CRISCVConsoleApplicationConfiguration::EParameter::DownKey), DDownButton);
    DDownButton->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DDownButton->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    SetKeyControllerMapping(DConfiguration.GetStringParameter(CRISCVConsoleApplicationConfiguration::EParameter::LeftKey), DLeftButton);
    DLeftButton->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DLeftButton->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    SetKeyControllerMapping(DConfiguration.GetStringParameter(CRISCVConsoleApplicationConfiguration::EParameter::RightKey), DRightButton);
    DRightButton->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DRightButton->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    SetKeyControllerMapping(DConfiguration.GetStringParameter(CRISCVConsoleApplicationConfiguration::EParameter::Button1Key), DButton1);
    DButton1->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DButton1->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    SetKeyControllerMapping(DConfiguration.GetStringParameter(CRISCVConsoleApplicationConfiguration::EParameter::Button2Key), DButton2);
    DButton2->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DButton2->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    SetKeyControllerMapping(DConfiguration.GetStringParameter(CRISCVConsoleApplicationConfiguration::EParameter::Button3Key), DButton3);
    DButton3->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DButton3->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    SetKeyControllerMapping(DConfiguration.GetStringParameter(CRISCVConsoleApplicationConfiguration::EParameter::Button4Key), DButton4);
    DButton4->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DButton4->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    DCommandButton->SetLabel("CMD");
    DCommandButton->SetButtonPressEventCallback(this,CommandButtonClickEventCallback);

    DDirectionButtonMapping[DUpButton] = CRISCVConsole::EDirection::Up;
    DDirectionButtonMapping[DDownButton] = CRISCVConsole::EDirection::Down;
    DDirectionButtonMapping[DLeftButton] = CRISCVConsole::EDirection::Left;
    DDirectionButtonMapping[DRightButton] = CRISCVConsole::EDirection::Right;

    DButtonNumberButtonMapping[DButton1] = CRISCVConsole::EButtonNumber::Button1;
    DButtonNumberButtonMapping[DButton2] = CRISCVConsole::EButtonNumber::Button2;
    DButtonNumberButtonMapping[DButton3] = CRISCVConsole::EButtonNumber::Button3;
    DButtonNumberButtonMapping[DButton4] = CRISCVConsole::EButtonNumber::Button4;
}

void CRISCVConsoleApplication::CreateSystemControlWidgets(){
    DPowerButton = CGUIFactory::NewToggleButton();
    DResetButton = CGUIFactory::NewButton();
    DFirmwareButton = CGUIFactory::NewButton();
    DCartridgeButton = CGUIFactory::NewToggleButton();
    DSystemControlGrid = CGUIFactory::NewGrid();
    DSystemControlGrid->Attach(DCartridgeButton,0,0,2,1);
    DSystemControlGrid->Attach(DResetButton,0,1,1,1);
    DSystemControlGrid->Attach(DPowerButton,1,1,1,1);
    DSystemControlGrid->Attach(DFirmwareButton,0,2,2,1);
    DSystemControlGrid->SetRowSpacing(GetWidgetSpacing());
    DSystemControlGrid->SetColumnSpacing(GetWidgetSpacing());

    DPowerButton->SetLabel("PWR");
    DPowerButton->SetToggledEventCallback(this, PowerButtonToggledEventCallback);

    DResetButton->SetLabel("RST");
    DResetButton->SetButtonPressEventCallback(this,ResetButtonClickEventCallback);

    DFirmwareButton->SetLabel("Firmware");
    DFirmwareButton->SetButtonPressEventCallback(this,FirmwareButtonClickEventCallback);

    DCartridgeButton->SetLabel("Cartridge");
    DCartridgeButton->SetToggledEventCallback(this, CartridgeButtonToggledEventCallback);
}

void CRISCVConsoleApplication::CreateDebugWidgets(){
    DConsoleDebugBox = CGUIFactory::NewBox(CGUIBox::EOrientation::Horizontal,GetWidgetSpacing());
    DDebugBox = CGUIFactory::NewBox(CGUIBox::EOrientation::Vertical,GetWidgetSpacing());
    CreateDebugRegisterWidgets();
    auto RegisterBox = CGUIFactory::NewBox(CGUIBox::EOrientation::Horizontal,0);
    RegisterBox->PackStart(DRegisterGrid,false,false,GetWidgetSpacing());
    DDebugBox->PackStart(RegisterBox,false,false,GetWidgetSpacing());

    CreateDebugControlWidgets();
    //DDebugBox->PackStart(DDebugControlBox,false,false,GetWidgetSpacing());

    CreateDebugInstructionWidgets();
    CreateDebugCSRWidgets();
    auto InstCSRGrid = CGUIFactory::NewGrid();
    auto InstLabel = CGUIFactory::NewLabel("Instructions");
    auto CSRLabel = CGUIFactory::NewLabel("CSRs");
    InstLabel->SetJustification(SGUIJustificationType::Left);
    CSRLabel->SetJustification(SGUIJustificationType::Left);
    InstCSRGrid->Attach(InstLabel,0,0,1,1);
    InstCSRGrid->Attach(DDebugInstructionComboBox,1,0,1,1);
    InstCSRGrid->Attach(DDebugInstructions->ContainingWidget(),0,1,2,1);
    
    InstCSRGrid->Attach(DDebugControlBox,2,1,1,1);
    InstCSRGrid->Attach(CSRLabel,3,0,1,1);
    InstCSRGrid->Attach(DDebugCSRegisters->ContainingWidget(),3,1,1,1);
    InstCSRGrid->SetColumnSpacing(GetWidgetSpacing());
    InstCSRGrid->SetRowSpacing(GetWidgetSpacing());
    DDebugBox->PackStart(InstCSRGrid,false,false,GetWidgetSpacing());
    CreateDebugMemoryWidgets();
    auto MemoryGrid = CGUIFactory::NewGrid();
    auto MemoryLabel = CGUIFactory::NewLabel("Memory");
    DDebugMemoryFirmwareButton = CGUIFactory::NewButton();
    DDebugMemoryCartridgeButton = CGUIFactory::NewButton();
    DDebugMemoryRegistersButton = CGUIFactory::NewButton();
    DDebugMemoryVideoButton = CGUIFactory::NewButton();
    DDebugMemoryDataButton = CGUIFactory::NewButton();
    DDebugMemoryStackButton = CGUIFactory::NewToggleButton();
    MemoryLabel->SetJustification(SGUIJustificationType::Left);
    MemoryGrid->Attach(MemoryLabel,0,0,1,1);
    MemoryGrid->Attach(DDebugMemoryFirmwareButton,1,0,1,1);
    MemoryGrid->Attach(DDebugMemoryCartridgeButton,2,0,1,1);
    MemoryGrid->Attach(DDebugMemoryRegistersButton,3,0,1,1);
    MemoryGrid->Attach(DDebugMemoryVideoButton,4,0,1,1);
    MemoryGrid->Attach(DDebugMemoryDataButton,5,0,1,1);
    MemoryGrid->Attach(DDebugMemoryStackButton,6,0,1,1);
    MemoryGrid->Attach(DDebugMemory->ContainingWidget(),0,1,8,1);
    MemoryGrid->SetRowSpacing(GetWidgetSpacing());
    MemoryGrid->SetColumnSpacing(GetWidgetSpacing());
    DDebugBox->PackStart(MemoryGrid,false,false,GetWidgetSpacing());

    DDebugMemoryFirmwareButton->SetLabel("FW");
    DDebugMemoryFirmwareButton->SetButtonPressEventCallback(this,DebugMemoryButtonClickEventCallback);
    DDebugMemoryButtonMapping[DDebugMemoryFirmwareButton] = DRISCVConsole->FirmwareMemoryBase();
    DDebugMemoryCartridgeButton->SetLabel("CTR");
    DDebugMemoryCartridgeButton->SetButtonPressEventCallback(this,DebugMemoryButtonClickEventCallback);
    DDebugMemoryButtonMapping[DDebugMemoryCartridgeButton] = DRISCVConsole->CartridgeMemoryBase();
    DDebugMemoryRegistersButton->SetLabel("CS");
    DDebugMemoryRegistersButton->SetButtonPressEventCallback(this,DebugMemoryButtonClickEventCallback);
    DDebugMemoryButtonMapping[DDebugMemoryRegistersButton] = DRISCVConsole->RegisterMemoryBase();
    DDebugMemoryVideoButton->SetLabel("VID");
    DDebugMemoryVideoButton->SetButtonPressEventCallback(this,DebugMemoryButtonClickEventCallback);
    DDebugMemoryButtonMapping[DDebugMemoryVideoButton] = DRISCVConsole->VideoMemoryBase();
    DDebugMemoryDataButton->SetLabel("GP");
    DDebugMemoryDataButton->SetButtonPressEventCallback(this,DebugMemoryButtonClickEventCallback);
    DDebugMemoryStackButton->SetLabel("SP");
    DDebugMemoryStackButton->SetToggledEventCallback(this,DebugMemoryStackButtonToggledEventCallback);
    

    DConsoleDebugBox->PackStart(DConsoleBox,false,false,GetWidgetSpacing());
    DConsoleDebugBox->PackStart(DDebugBox,false,false,GetWidgetSpacing());
    RefreshDebugRegisters();
}

void CRISCVConsoleApplication::CreateDebugRegisterWidgets(){
    DRegisterGrid = CGUIFactory::NewGrid(); 
    auto QuarterCount = CRISCVCPU::RegisterCount() / 4;
    size_t MaxChar = 0;
    for(size_t RegisterIndex = 0; RegisterIndex < CRISCVCPU::RegisterCount(); RegisterIndex++){
        MaxChar = std::max(CRISCVCPU::CInstruction::RegisterName(RegisterIndex).length(),MaxChar);
    }
    MaxChar++;
    auto Title = CGUIFactory::NewLabel("CPU Regs");
    Title->SetJustification(SGUIJustificationType::Left);
    DRegisterGrid->Attach(Title,0,0,8,1);
    for(size_t RegisterIndex = 0; RegisterIndex < CRISCVCPU::RegisterCount(); RegisterIndex++){
        auto RegisterName = CRISCVCPU::CInstruction::RegisterName(RegisterIndex);
        if(RegisterName == "gp"){
            DDebugMemoryGlobalPointerRegisterIndex = RegisterIndex;
        }
        else if(RegisterName == "sp"){
            DDebugMemoryStackPointerRegisterIndex = RegisterIndex;
        }
        DGeneralRegisterNameLabels.push_back(CGUIFactory::NewLabel(RegisterName + ":"));
        DGeneralRegisterNameLabels.back()->SetJustification(SGUIJustificationType::Right);
        DGeneralRegisterNameLabels.back()->SetHorizontalExpand(true);
        DGeneralRegisterNameLabels.back()->SetFontFamily("monospace");
        DGeneralRegisterNameLabels.back()->SetWidthCharacters(MaxChar);
        DRegisterGrid->Attach(DGeneralRegisterNameLabels.back(),(RegisterIndex/QuarterCount)*2,(RegisterIndex%QuarterCount)+1,1,1);
        DGeneralRegisterValueLabels.push_back(CGUIFactory::NewLabel("XXXXXXXX"));
        DGeneralRegisterValueLabels.back()->SetFontFamily("monospace");
        DGeneralRegisterValueLabels.back()->SetWidthCharacters(8);
        DRegisterGrid->Attach(DGeneralRegisterValueLabels.back(),(RegisterIndex/QuarterCount)*2+1,(RegisterIndex%QuarterCount)+1,1,1);
    }
    DProgramCounterNameLabel = CGUIFactory::NewLabel("pc:");
    DProgramCounterNameLabel->SetJustification(SGUIJustificationType::Right);
    DProgramCounterNameLabel->SetHorizontalExpand(true);
    DProgramCounterNameLabel->SetFontFamily("monospace");
    DProgramCounterNameLabel->SetWidthCharacters(MaxChar);
    DRegisterGrid->Attach(DProgramCounterNameLabel,0,QuarterCount+1,1,1);
    DProgramCounterValueLabel = CGUIFactory::NewLabel("XXXXXXXX");
    DProgramCounterValueLabel->SetFontFamily("monospace");
    DProgramCounterValueLabel->SetWidthCharacters(8);
    DRegisterGrid->Attach(DProgramCounterValueLabel,1,QuarterCount+1,1,1);

    DRegisterGrid->SetHorizontalExpand(false);
}

void CRISCVConsoleApplication::CreateDebugControlWidgets(){
    DDebugControlBox = CGUIFactory::NewBox(CGUIBox::EOrientation::Vertical,GetWidgetSpacing());

    DDebugRunButton = CGUIFactory::NewToggleButton();
    DDebugStepButton = CGUIFactory::NewButton();
    DDebugClearButton = CGUIFactory::NewButton();
    DDebugRecordButton = CGUIFactory::NewToggleButton();
    DDebugRunButton->SetLabel("Run");
    DDebugRunButton->SetToggledEventCallback(this, RunButtonToggledEventCallback);

    DDebugStepButton->SetLabel("Step");
    DDebugStepButton->SetButtonPressEventCallback(this,StepButtonClickEventCallback);

    DDebugClearButton->SetLabel("Clear");
    DDebugClearButton->SetButtonPressEventCallback(this,ClearButtonClickEventCallback);

    DDebugRecordButton->SetLabel("Record");
    DDebugRecordButton->SetToggledEventCallback(this, RecordButtonToggledEventCallback);

    DDebugControlBox->PackStart(DDebugRunButton,false,false,GetWidgetSpacing());
    DDebugControlBox->PackStart(DDebugStepButton,false,false,GetWidgetSpacing());
    DDebugControlBox->PackStart(DDebugClearButton,false,false,GetWidgetSpacing());
    DDebugControlBox->PackStart(DDebugRecordButton,false,false,GetWidgetSpacing());

}

void CRISCVConsoleApplication::CreateDebugInstructionWidgets(){
    DDebugInstructions = std::make_shared<CGUIScrollableLabelBox>();
    // Assume @01234567: abcdef zero,zero,-2147483648
    DDebugInstructions->SetWidthCharacters(38);
    DDebugInstructions->SetLineCount(GetInstructionLineCount());
    DDebugInstructions->SetButtonPressEventCallback(this,InstructionBoxButtonEventCallback);
    DDebugInstructions->SetScrollEventCallback(this,InstructionBoxScrollEventCallback);

    DDebugInstructionComboBox = CGUIFactory::NewComboBox();
    DDebugInstructionComboBox->SetFontFamily("monospace");
    DDebugInstructionComboBox->SetChangedEventCallback(this,InstructionComboBoxChangedEventCallback);

}

void CRISCVConsoleApplication::CreateDebugCSRWidgets(){
    DDebugCSRegisters = std::make_shared<CGUIScrollableLabelBox>();
    // Assume mcounteren: 01234567
    DDebugCSRegisters->SetWidthCharacters(20);
    std::vector< std::string > InitialCSR;
    size_t MaxLen = 0;
    for(auto CSRAddr : DRISCVConsole->CPU()->ControlStatusRegisterKeys()){
        InitialCSR.push_back(DRISCVConsole->CPU()->ControlStatusRegisterName(CSRAddr) + std::string(": XXXXXXXX"));
        MaxLen = std::max(MaxLen,InitialCSR.back().length());
    }
    for(auto &Line : InitialCSR){
        if(Line.length() < MaxLen){
            Line = std::string(MaxLen - Line.length(),' ') + Line;
        }
    }
    DDebugCSRegisters->SetBufferedLines(InitialCSR);
    DDebugCSRegisters->SetLineCount(GetInstructionLineCount());
}

void CRISCVConsoleApplication::CreateDebugMemoryWidgets(){
    std::unordered_map< uint32_t, uint32_t > MemoryRegions = {
        {DRISCVConsole->FirmwareMemoryBase(), DRISCVConsole->FirmwareMemorySize()},
        {DRISCVConsole->CartridgeMemoryBase(), DRISCVConsole->CartridgeMemorySize()},
        {DRISCVConsole->RegisterMemoryBase(), DRISCVConsole->RegisterMemorySize()},
        {DRISCVConsole->VideoMemoryBase(), DRISCVConsole->VideoMemorySize()},
        {DRISCVConsole->MainMemoryBase(), DRISCVConsole->MainMemorySize()}
    };
    DDebugMemory = std::make_shared<CGUIScrollableMemoryLabelBox>(DRISCVConsole->Memory(), MemoryRegions);
    DDebugMemory->SetLineCount(GetMemoryLineCount());
}

bool CRISCVConsoleApplication::ParseInstructionLine(size_t line, uint32_t &addr, bool &breakpoint){
    auto Line = DDebugInstructions->GetBufferedLine(line);
    if(Line.length() < 10){
        return false;
    }
    breakpoint = Line[0] == '@';
    addr = std::stoull(Line.substr(1,8), nullptr, 16);
    return true;
}

void CRISCVConsoleApplication::SetKeyControllerMapping(const std::string &label, std::shared_ptr<CGUIToggleButton> button){
    auto NewLabel = label.substr(0,1);

    if(('a' <= NewLabel[0])&&('z' >= NewLabel[0])){
        auto Delta = NewLabel[0] - 'a';
        DKeyControllerMapping[SGUIKeyType::KeyA + Delta] = button;
        DKeyControllerMapping[SGUIKeyType::Keya + Delta] = button;
    }
    else if(('A' <= NewLabel[0])&&('Z' >= NewLabel[0])){
        auto Delta = NewLabel[0] - 'A';
        DKeyControllerMapping[SGUIKeyType::KeyA + Delta] = button;
        DKeyControllerMapping[SGUIKeyType::Keya + Delta] = button;
    }
    else if(('0' <= NewLabel[0])&&('9' >= NewLabel[0])){
        auto Delta = NewLabel[0] - '0';
        DKeyControllerMapping[SGUIKeyType::Key0 + Delta] = button;
    }
    button->SetLabel(NewLabel);
}

uint32_t CRISCVConsoleApplication::GetScreenTimeoutMS(){
    auto Timeout = DConfiguration.GetIntegerParameter(CRISCVConsoleApplicationConfiguration::EParameter::ScreenTimer);
    if(2 > Timeout){
        Timeout = 2;
    }
    if(500 < Timeout){
        Timeout = 500;
    }
    return Timeout;
}

uint32_t CRISCVConsoleApplication::GetWidgetSpacing(){
    auto Spacing = DConfiguration.GetIntegerParameter(CRISCVConsoleApplicationConfiguration::EParameter::WidgetSpacing);
    if(0 >= Spacing){
        Spacing = 1;
    }
    if(10 < Spacing){
        Spacing = 10;
    }
    return Spacing;
}

uint32_t CRISCVConsoleApplication::GetInstructionLineCount(){
    auto LineCount = DConfiguration.GetIntegerParameter(CRISCVConsoleApplicationConfiguration::EParameter::InstructionLines);
    if(4 > LineCount){
        LineCount = 4;
    }
    if(128 < LineCount){
        LineCount = 128;
    }
    return LineCount;
}

uint32_t CRISCVConsoleApplication::GetMemoryLineCount(){
    auto LineCount = DConfiguration.GetIntegerParameter(CRISCVConsoleApplicationConfiguration::EParameter::MemoryLines);
    if(4 > LineCount){
        LineCount = 4;
    }
    if(128 < LineCount){
        LineCount = 128;
    }
    return LineCount;
}

uint32_t CRISCVConsoleApplication::GetTimerUS(){
    auto TimerUS = DConfiguration.GetIntegerParameter(CRISCVConsoleApplicationConfiguration::EParameter::TimerUS);
    if(100 > TimerUS){
        TimerUS = 100;
    }
    if(500000 < TimerUS){
        TimerUS = 500000;
    }
    return TimerUS;
}

uint32_t CRISCVConsoleApplication::GetCPUFrequency(){
    auto CPUFreq = DConfiguration.GetIntegerParameter(CRISCVConsoleApplicationConfiguration::EParameter::TimerUS);
    if(1000000 > CPUFreq){
        CPUFreq = 1000000;
    }
    return CPUFreq;
}

void CRISCVConsoleApplication::RefreshDebugRegisters(){
    for(size_t Index = 0; Index < CRISCVCPU::RegisterCount(); Index++){
        DGeneralRegisterValueLabels[Index]->SetText(FormatHex32Bit(DRISCVConsole->CPU()->Register(Index)));
    }
    auto PC = DRISCVConsole->CPU()->ProgramCounter();
    DProgramCounterValueLabel->SetText(FormatHex32Bit(PC));
    size_t LineIndex = 0;
    for(auto CSRAddr : DRISCVConsole->CPU()->ControlStatusRegisterKeys()){
        auto NewLine = DDebugCSRegisters->GetBufferedLine(LineIndex);
        NewLine.replace(NewLine.length() - 8, 8, FormatHex32Bit(DRISCVConsole->CPU()->ControlStatusRegister(CSRAddr)));
        DDebugCSRegisters->UpdateBufferedLine(LineIndex,NewLine);
        LineIndex++;
    }
    if(DDebugMemoryStackButton->GetActive()){
        auto RegisterBase = DRISCVConsole->CPU()->Register(DDebugMemoryStackPointerRegisterIndex);
        DDebugMemory->SetBaseAddress(RegisterBase,false);
    }
    DDebugMemory->Refresh();
    LineIndex = DRISCVConsole->InstructionAddressesToIndices(PC);
    DDebugInstructions->HighlightBufferedLine(LineIndex);
    if(DFollowingInstruction){
        if((DDebugInstructions->GetBaseLine() > LineIndex)||(DDebugInstructions->GetBaseLine() + DDebugInstructions->GetLineCount()/2 <= LineIndex)){
            DDebugInstructions->SetBaseLine(LineIndex < DDebugInstructions->GetLineCount()/2 ? 0 : LineIndex - (DDebugInstructions->GetLineCount()/2 - 1));
        }
    }
}

void CRISCVConsoleApplication::ParseArguments(int &argc, char *argv[]){
    for(int Index = 1; Index < argc; ){
        if(std::string("-d") == argv[Index]){
            DDebugMode = true;
            for(int OIndex = Index+1; OIndex < argc; OIndex++){
                argv[OIndex-1] = argv[OIndex];
            }
            argc--;
        }
        else{
            Index++;
        }
    }
}

std::string CRISCVConsoleApplication::FormatHex32Bit(uint32_t val){
    std::stringstream Stream;
    Stream<<std::setfill('0') << std::setw(8) << std::hex << val;
    return Stream.str();
}

int CRISCVConsoleApplication::Run(int argc, char *argv[]){
    ParseArguments(argc,argv);
    return DApplication->Run(argc, argv);
}