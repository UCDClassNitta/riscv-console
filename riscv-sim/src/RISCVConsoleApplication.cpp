#include "RISCVConsoleApplication.h"
#include "FileDataSink.h"
#include "FileDataSource.h"
#include "Path.h"
#include <cstdio>

std::shared_ptr< CRISCVConsoleApplication > CRISCVConsoleApplication::DApplicationPointer;

CRISCVConsoleApplication::CRISCVConsoleApplication(const std::string &appname, const SPrivateConstructionType &key){
    DFileOpenFolder = CPath::CurrentPath().ToString();
    DApplication = CGUIFactory::ApplicationInstance(appname);
    
    DRISCVConsole = std::make_shared<CRISCVConsole>();

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

bool CRISCVConsoleApplication::CartridgeButtonClickEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data){
    CRISCVConsoleApplication *App = static_cast<CRISCVConsoleApplication *>(data);
    return App->CartridgeButtonClickEvent(widget,event);
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

void CRISCVConsoleApplication::Activate(){
    DMainWindow = DApplication->NewWindow();
    DMainWindow->SetDeleteEventCallback(this, MainWindowDeleteEventCallback);
    DMainWindow->SetDestroyEventCallback(this, MainWindowDestroyCallback);
   
    DMainWindow->SetKeyPressEventCallback(this, MainWindowKeyPressEventCallback);
    DMainWindow->SetKeyReleaseEventCallback(this, MainWindowKeyReleaseEventCallback);    

    DConsoleBox = CGUIFactory::NewBox(CGUIBox::EOrientation::Vertical,5);
    DConsoleVideo = CGUIFactory::NewDrawingArea();
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
    DControllerGrid->SetRowSpacing(5);
    DControllerGrid->SetColumnSpacing(5);

    DPowerButton = CGUIFactory::NewToggleButton();
    DResetButton = CGUIFactory::NewButton();
    DFirmwareButton = CGUIFactory::NewButton();
    DCartridgeButton = CGUIFactory::NewButton();
    DSystemControlGrid = CGUIFactory::NewGrid();
    DSystemControlGrid->Attach(DCartridgeButton,0,0,2,1);
    DSystemControlGrid->Attach(DResetButton,0,1,1,1);
    DSystemControlGrid->Attach(DPowerButton,1,1,1,1);
    DSystemControlGrid->Attach(DFirmwareButton,0,2,2,1);
    DSystemControlGrid->SetRowSpacing(5);
    DSystemControlGrid->SetColumnSpacing(5);

    DControlsBox = CGUIFactory::NewBox(CGUIBox::EOrientation::Horizontal,5);
    DControlsBox->PackStart(DControllerGrid,false,false,4);
    DControlsBox->PackStart(DSystemControlGrid,false,false,4);
    

    DConsoleBox->PackStart(DConsoleVideo,true,true,4);
    DConsoleBox->PackStart(DControlsBox,false,false,4);

    DMainWindow->Add(DConsoleBox);
    DDoubleBufferSurface = CGraphicFactory::CreateSurface(DRISCVConsole->ScreenWidth(), DRISCVConsole->ScreenHeight(), ESurfaceFormat::ARGB32);
    DWorkingBufferSurface = CGraphicFactory::CreateSurface(DRISCVConsole->ScreenWidth(), DRISCVConsole->ScreenHeight(), ESurfaceFormat::ARGB32);
    DConsoleVideo->SetSizeRequest(DRISCVConsole->ScreenWidth(), DRISCVConsole->ScreenHeight());
    DControllerGrid->ShowAll();
    DConsoleVideo->ShowAll();
    DConsoleBox->ShowAll();
    DMainWindow->ShowAll();
    
    DConsoleVideo->SetDrawEventCallback(this, DrawingAreaDrawCallback);
    auto TempRC = DDoubleBufferSurface->CreateResourceContext();
    TempRC->SetSourceRGB(0x0);
    TempRC->SetLineWidth(1);
    TempRC->Rectangle(0,0,DRISCVConsole->ScreenWidth(), DRISCVConsole->ScreenHeight());
    TempRC->StrokePreserve();
    TempRC->Fill();
    //DDoubleBufferSurface->Refresh(DDoubleBufferSurface);
    
    DConsoleVideo->Invalidate();

    DKeyControllerMapping[SGUIKeyType::KeyW] = DUpButton;
    DKeyControllerMapping[SGUIKeyType::Keyw] = DUpButton;
    DUpButton->SetLabel("w");
    DUpButton->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DUpButton->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);
    //DUpButton->SetToggledEventCallback(this, ControllerButtonToggledEventCallback);
    DKeyControllerMapping[SGUIKeyType::KeyX] = DDownButton;
    DKeyControllerMapping[SGUIKeyType::Keyx] = DDownButton;
    DDownButton->SetLabel("x");
    DDownButton->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DDownButton->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);
    //DDownButton->SetToggledEventCallback(this, ControllerButtonToggledEventCallback);
    DKeyControllerMapping[SGUIKeyType::KeyA] = DLeftButton;
    DKeyControllerMapping[SGUIKeyType::Keya] = DLeftButton;
    DLeftButton->SetLabel("a");
    DLeftButton->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DLeftButton->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);
    DKeyControllerMapping[SGUIKeyType::KeyD] = DRightButton;
    DKeyControllerMapping[SGUIKeyType::Keyd] = DRightButton;
    DRightButton->SetLabel("d");
    DRightButton->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DRightButton->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    DKeyControllerMapping[SGUIKeyType::KeyU] = DButton1;
    DKeyControllerMapping[SGUIKeyType::Keyu] = DButton1;
    DButton1->SetLabel("u");
    DButton1->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DButton1->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    DKeyControllerMapping[SGUIKeyType::KeyI] = DButton2;
    DKeyControllerMapping[SGUIKeyType::Keyi] = DButton2;
    DButton2->SetLabel("i");
    DButton2->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DButton2->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    DKeyControllerMapping[SGUIKeyType::KeyJ] = DButton3;
    DKeyControllerMapping[SGUIKeyType::Keyj] = DButton3;
    DButton3->SetLabel("j");
    DButton3->SetButtonPressEventCallback(this, ControllerButtonClickEventCallback);
    DButton3->SetButtonReleaseEventCallback(this, ControllerButtonClickEventCallback);

    DKeyControllerMapping[SGUIKeyType::KeyK] = DButton4;
    DKeyControllerMapping[SGUIKeyType::Keyk] = DButton4;
    DButton4->SetLabel("k");
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

    DPowerButton->SetLabel("PWR");
    DPowerButton->SetToggledEventCallback(this, PowerButtonToggledEventCallback);

    DResetButton->SetLabel("RST");
    DResetButton->SetButtonPressEventCallback(this,ResetButtonClickEventCallback);
    DFirmwareButton->SetLabel("Firmware");
    DFirmwareButton->SetButtonPressEventCallback(this,FirmwareButtonClickEventCallback);
    DCartridgeButton->SetLabel("Cartridge");
    DCartridgeButton->SetButtonPressEventCallback(this,CartridgeButtonClickEventCallback);
    DApplication->SetTimer(10,this,TimeoutCallback);
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
            DRISCVConsole->ProgramFirmware(InFile);
        }
    }
    

    return true;
}

bool CRISCVConsoleApplication::CartridgeButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event){
    std::string Filename;
    {
        auto FileChooser = CGUIFactory::NewFileChooserDialog("Select Cartridge",true,DMainWindow);
        FileChooser->SetCurrentFolder(DFileOpenFolder);
        if(FileChooser->Run()){
            Filename = FileChooser->GetFilename();
            DFileOpenFolder = FileChooser->GetCurrentFolder();
            auto InFile = std::make_shared<CFileDataSource>(Filename);
            DRISCVConsole->InsertCartridge(InFile);
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
    DRISCVConsole->PressCommand();
    return true;
}

bool CRISCVConsoleApplication::ResetButtonClickEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event){
    DRISCVConsole->Reset();
    return true;
}

bool CRISCVConsoleApplication::PowerButtonToggledEvent(std::shared_ptr<CGUIWidget> widget){
    if(std::dynamic_pointer_cast<CGUIToggleButton>(widget)->GetActive()){
        DRISCVConsole->PowerOn();
    }
    else{
        DRISCVConsole->PowerOff();
    }
    
    return true;
}

void CRISCVConsoleApplication::UpdateConsoleButtonChange(std::shared_ptr<CGUIToggleButton> button){
    auto DirectionSearch = DDirectionButtonMapping.find(button);
    if(DirectionSearch != DDirectionButtonMapping.end()){
        if(button->GetActive()){
            DRISCVConsole->PressDirection(DirectionSearch->second);
        }
        else{
            DRISCVConsole->ReleaseDirection(DirectionSearch->second);
        }
        return;
    }
    auto ButtonSearch = DButtonNumberButtonMapping.find(button);
    if(ButtonSearch != DButtonNumberButtonMapping.end()){
        if(button->GetActive()){
            DRISCVConsole->PressButton(ButtonSearch->second);
        }
        else{
            DRISCVConsole->ReleaseButton(ButtonSearch->second);
        }
    }
}

std::shared_ptr< CRISCVConsoleApplication > CRISCVConsoleApplication::Instance(const std::string &appname){
    if(!DApplicationPointer){
        DApplicationPointer = std::make_shared< CRISCVConsoleApplication >(appname, SPrivateConstructionType{});   
    }
    return DApplicationPointer;
}

int CRISCVConsoleApplication::Run(int argc, char *argv[]){
    return DApplication->Run(argc, argv);
}