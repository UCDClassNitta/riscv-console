
#include "GUIFactoryGTK3.h"
#include "GraphicFactoryCairo.h"
#include <glib.h>
#include <glib/gi18n.h>

const uint32_t SGUIButtonEventType::DButtonPress = GDK_BUTTON_PRESS;
const uint32_t SGUIButtonEventType::DDoubleButtonPress = GDK_2BUTTON_PRESS;
const uint32_t SGUIButtonEventType::DTripleButtonPress = GDK_3BUTTON_PRESS;
const uint32_t SGUIButtonEventType::DButtonRelease = GDK_BUTTON_RELEASE;

bool SGUIButtonEventType::IsButtonPress() const{
    return DType == DButtonPress;
}

bool SGUIButtonEventType::IsDoubleButtonPress() const{
    return DType == DDoubleButtonPress;
}

bool SGUIButtonEventType::IsTripleButtonPress() const{
    return DType == DTripleButtonPress;
}

bool SGUIButtonEventType::IsButtonRelease() const{
    return DType == DButtonRelease;
}

const uint32_t SGUIModifierType::Shift = GDK_SHIFT_MASK;
const uint32_t SGUIModifierType::Lock = GDK_LOCK_MASK;
const uint32_t SGUIModifierType::Control = GDK_CONTROL_MASK;
const uint32_t SGUIModifierType::Alt = GDK_MOD1_MASK;
const uint32_t SGUIModifierType::Button1 = GDK_BUTTON1_MASK;
const uint32_t SGUIModifierType::Button2 = GDK_BUTTON2_MASK;
const uint32_t SGUIModifierType::Button3 = GDK_BUTTON3_MASK;
bool SGUIModifierType::ModifierIsSet(uint32_t val) const{
    return DState & val;
}

void SGUIModifierType::SetModifier(uint32_t val){
    DState |= val;
}

void SGUIModifierType::ClearModifier(uint32_t val){
    DState &= ~val;
}

const uint32_t SGUIKeyType::LeftShift = GDK_KEY_Shift_L;
const uint32_t SGUIKeyType::RightShift = GDK_KEY_Shift_R;
const uint32_t SGUIKeyType::LeftControl = GDK_KEY_Control_L;
const uint32_t SGUIKeyType::RightControl = GDK_KEY_Control_R;
const uint32_t SGUIKeyType::LeftAlt = GDK_KEY_Alt_L;
const uint32_t SGUIKeyType::RightAlt = GDK_KEY_Alt_R;
const uint32_t SGUIKeyType::Escape = GDK_KEY_Escape;
const uint32_t SGUIKeyType::Space = GDK_KEY_space;
const uint32_t SGUIKeyType::Delete = GDK_KEY_Delete;
const uint32_t SGUIKeyType::Period = GDK_KEY_period;
const uint32_t SGUIKeyType::BackSpace = GDK_KEY_BackSpace;
const uint32_t SGUIKeyType::UpArrow = GDK_KEY_Up;
const uint32_t SGUIKeyType::DownArrow = GDK_KEY_Down;
const uint32_t SGUIKeyType::LeftArrow = GDK_KEY_Left;
const uint32_t SGUIKeyType::RightArrow = GDK_KEY_Right;
const uint32_t SGUIKeyType::Key0 = GDK_KEY_0;
const uint32_t SGUIKeyType::Key1 = GDK_KEY_1;
const uint32_t SGUIKeyType::Key2 = GDK_KEY_2;
const uint32_t SGUIKeyType::Key3 = GDK_KEY_3;
const uint32_t SGUIKeyType::Key4 = GDK_KEY_4;
const uint32_t SGUIKeyType::Key5 = GDK_KEY_5;
const uint32_t SGUIKeyType::Key6 = GDK_KEY_6;
const uint32_t SGUIKeyType::Key7 = GDK_KEY_7;
const uint32_t SGUIKeyType::Key8 = GDK_KEY_8;
const uint32_t SGUIKeyType::Key9 = GDK_KEY_9;
const uint32_t SGUIKeyType::KeyA = GDK_KEY_A;
const uint32_t SGUIKeyType::KeyB = GDK_KEY_B;
const uint32_t SGUIKeyType::KeyC = GDK_KEY_C;
const uint32_t SGUIKeyType::KeyD = GDK_KEY_D;
const uint32_t SGUIKeyType::KeyE = GDK_KEY_E;
const uint32_t SGUIKeyType::KeyF = GDK_KEY_F;
const uint32_t SGUIKeyType::KeyG = GDK_KEY_G;
const uint32_t SGUIKeyType::KeyH = GDK_KEY_H;
const uint32_t SGUIKeyType::KeyI = GDK_KEY_I;
const uint32_t SGUIKeyType::KeyJ = GDK_KEY_J;
const uint32_t SGUIKeyType::KeyK = GDK_KEY_K;
const uint32_t SGUIKeyType::KeyL = GDK_KEY_L;
const uint32_t SGUIKeyType::KeyM = GDK_KEY_M;
const uint32_t SGUIKeyType::KeyN = GDK_KEY_N;
const uint32_t SGUIKeyType::KeyO = GDK_KEY_O;
const uint32_t SGUIKeyType::KeyP = GDK_KEY_P;
const uint32_t SGUIKeyType::KeyQ = GDK_KEY_Q;
const uint32_t SGUIKeyType::KeyR = GDK_KEY_R;
const uint32_t SGUIKeyType::KeyS = GDK_KEY_S;
const uint32_t SGUIKeyType::KeyT = GDK_KEY_T;
const uint32_t SGUIKeyType::KeyU = GDK_KEY_U;
const uint32_t SGUIKeyType::KeyV = GDK_KEY_V;
const uint32_t SGUIKeyType::KeyW = GDK_KEY_W;
const uint32_t SGUIKeyType::KeyX = GDK_KEY_X;
const uint32_t SGUIKeyType::KeyY = GDK_KEY_Y;
const uint32_t SGUIKeyType::KeyZ = GDK_KEY_Z;
const uint32_t SGUIKeyType::Keya = GDK_KEY_a;
const uint32_t SGUIKeyType::Keyb = GDK_KEY_b;
const uint32_t SGUIKeyType::Keyc = GDK_KEY_c;
const uint32_t SGUIKeyType::Keyd = GDK_KEY_d;
const uint32_t SGUIKeyType::Keye = GDK_KEY_e;
const uint32_t SGUIKeyType::Keyf = GDK_KEY_f;
const uint32_t SGUIKeyType::Keyg = GDK_KEY_g;
const uint32_t SGUIKeyType::Keyh = GDK_KEY_h;
const uint32_t SGUIKeyType::Keyi = GDK_KEY_i;
const uint32_t SGUIKeyType::Keyj = GDK_KEY_j;
const uint32_t SGUIKeyType::Keyk = GDK_KEY_k;
const uint32_t SGUIKeyType::Keyl = GDK_KEY_l;
const uint32_t SGUIKeyType::Keym = GDK_KEY_m;
const uint32_t SGUIKeyType::Keyn = GDK_KEY_n;
const uint32_t SGUIKeyType::Keyo = GDK_KEY_o;
const uint32_t SGUIKeyType::Keyp = GDK_KEY_p;
const uint32_t SGUIKeyType::Keyq = GDK_KEY_q;
const uint32_t SGUIKeyType::Keyr = GDK_KEY_r;
const uint32_t SGUIKeyType::Keys = GDK_KEY_s;
const uint32_t SGUIKeyType::Keyt = GDK_KEY_t;
const uint32_t SGUIKeyType::Keyu = GDK_KEY_u;
const uint32_t SGUIKeyType::Keyv = GDK_KEY_v;
const uint32_t SGUIKeyType::Keyw = GDK_KEY_w;
const uint32_t SGUIKeyType::Keyx = GDK_KEY_x;
const uint32_t SGUIKeyType::Keyy = GDK_KEY_y;
const uint32_t SGUIKeyType::Keyz = GDK_KEY_z;

const uint32_t SGUIJustificationType::Left = GTK_JUSTIFY_LEFT;
const uint32_t SGUIJustificationType::Right = GTK_JUSTIFY_RIGHT;
const uint32_t SGUIJustificationType::Center = GTK_JUSTIFY_CENTER;
const uint32_t SGUIJustificationType::Fill = GTK_JUSTIFY_FILL;

bool SGUIKeyType::IsKey(uint32_t val) const{
    return DValue == val;
}

void SGUIKeyType::SetKey(uint32_t val){
    DValue = val;
}

bool SGUIKeyType::IsAlpha() const{
    return IsUpper() || IsLower();
}

bool SGUIKeyType::IsAlphaNumeric() const{
    return IsAlpha() || IsDigit();
}

bool SGUIKeyType::IsDigit() const{
    return ((Key0 <= DValue)&&(Key9 >= DValue));
}

bool SGUIKeyType::IsUpper() const{
    return ((KeyA <= DValue)&&(KeyZ >= DValue));
}

bool SGUIKeyType::IsLower() const{
    return ((Keya <= DValue)&&(Keyz >= DValue));
}

bool SGUIKeyType::IsASCII() const{
    return 127 >= DValue;
}


std::shared_ptr<CGUIApplication> CGUIFactory::DApplicationPointer = nullptr;

std::shared_ptr<CGUIApplication> CGUIFactory::ApplicationInstance(const std::string &appname){
    if(!DApplicationPointer && appname.length()){
        DApplicationPointer = std::make_shared<CGUIApplicationGTK3>(appname);
    }
    return DApplicationPointer;
}

std::shared_ptr<CGUIDisplay> CGUIFactory::DefaultDisplay(){
    return std::make_shared<CGUIDisplayGTK3>(gdk_display_get_default());   
}

std::shared_ptr<CGUIDrawingArea> CGUIFactory::NewDrawingArea(){
    return std::make_shared<CGUIDrawingAreaGTK3>(gtk_drawing_area_new());
}

std::shared_ptr<CGUIBox> CGUIFactory::NewBox(CGUIBox::EOrientation orientation, int spacing){
    return std::make_shared<CGUIBoxGTK3>(gtk_box_new(orientation == CGUIBox::EOrientation::Horizontal ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL, spacing));
}

std::shared_ptr<CGUIEventBox> CGUIFactory::NewEventBox(){
    return std::make_shared<CGUIEventBoxGTK3>(gtk_event_box_new());
}

std::shared_ptr<CGUIFrame> CGUIFactory::NewFrame(){
    return std::make_shared<CGUIFrameGTK3>(gtk_frame_new(NULL));
}

std::shared_ptr<CGUIButton> CGUIFactory::NewButton(){
    return std::make_shared<CGUIButtonGTK3>(gtk_button_new());
}

std::shared_ptr<CGUIComboBox> CGUIFactory::NewComboBox(){
    auto Model = gtk_list_store_new(1,G_TYPE_STRING);
    auto ComboBox = gtk_combo_box_new_with_model(GTK_TREE_MODEL(Model));
    
    return std::make_shared<CGUIComboBoxGTK3>(ComboBox);
}

std::shared_ptr<CGUILabel> CGUIFactory::NewLabel(const std::string &text){
    return std::make_shared<CGUILabelGTK3>(gtk_label_new(text.c_str()));
}

std::shared_ptr<CGUIToggleButton> CGUIFactory::NewToggleButton(){
    return std::make_shared<CGUIToggleButtonGTK3>(gtk_toggle_button_new());
}

std::shared_ptr<CGUIGrid> CGUIFactory::NewGrid(){
    return std::make_shared<CGUIGridGTK3>(gtk_grid_new());
}

std::shared_ptr<CGUIMenu> CGUIFactory::NewMenu(){
    return std::make_shared<CGUIMenuGTK3>(gtk_menu_new());
}

std::shared_ptr<CGUIMenuBar> CGUIFactory::NewMenuBar(){
    return std::make_shared<CGUIMenuBarGTK3>(gtk_menu_bar_new());
}

std::shared_ptr<CGUIMenuItem> CGUIFactory::NewMenuItem(const std::string &label){
    return std::make_shared<CGUIMenuItemGTK3>(gtk_menu_item_new_with_label(label.c_str()));
}

std::shared_ptr<CGUIScrollBar> CGUIFactory::NewScrollBar(CGUIScrollBar::EOrientation orientation){
    return std::make_shared<CGUIScrollBarGTK3>(gtk_scrollbar_new(orientation == CGUIScrollBar::EOrientation::Horizontal ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL, NULL));
}

std::shared_ptr<CGUIFileFilter> CGUIFactory::NewFileFilter(){
    return std::make_shared<CGUIFileFilterGTK3>(gtk_file_filter_new());   
}

std::shared_ptr<CGUIFileChooserDialog> CGUIFactory::NewFileChooserDialog(const std::string &title, bool openfile, std::shared_ptr<CGUIWindow> parent){
    GtkWindow *ParentWidget = NULL;
    if(parent){
        std::shared_ptr<CGUIWindowGTK3> ParentWindow = std::dynamic_pointer_cast<CGUIWindowGTK3>(parent);
    
        ParentWidget = GTK_WINDOW(ParentWindow->Widget());
    }   
    if(openfile){
        return std::make_shared<CGUIFileChooserDialogGTK3>(gtk_file_chooser_dialog_new(title.c_str(), ParentWidget, GTK_FILE_CHOOSER_ACTION_OPEN, _("_Cancel"), GTK_RESPONSE_CANCEL, _("_Open"), GTK_RESPONSE_ACCEPT, NULL));
    }
    else{
        return std::make_shared<CGUIFileChooserDialogGTK3>(gtk_file_chooser_dialog_new(title.c_str(), ParentWidget, GTK_FILE_CHOOSER_ACTION_SAVE, _("_Cancel"), GTK_RESPONSE_CANCEL, _("_Save"), GTK_RESPONSE_ACCEPT, NULL));
    }
}

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
    return std::make_shared<CGUIWindowGTK3>(gtk_application_window_new(DApplication));
}

CGUIDisplayGTK3::CGUIDisplayGTK3(GdkDisplay *display) : CGUIDisplay(){
    DDisplay = display;
}

CGUIDisplayGTK3::~CGUIDisplayGTK3(){

}

void CGUIDisplayGTK3::Sync(){
    gdk_display_sync(DDisplay);
}

void CGUIDisplayGTK3::Flush(){
    gdk_display_flush(DDisplay);
}

std::shared_ptr< CGUICursor > CGUIDisplayGTK3::NewCursor(CGUICursor::ECursorType type){
    GdkCursorType CursorType = GDK_BLANK_CURSOR;
    switch(type){
        case CGUICursor::ECursorType::RightPointer:     CursorType = GDK_RIGHT_PTR;
                                                        break;
        case CGUICursor::ECursorType::LeftPointer:      CursorType = GDK_LEFT_PTR;
                                                        break;
        case CGUICursor::ECursorType::CenterPointer:    CursorType = GDK_CENTER_PTR;
                                                        break;
        case CGUICursor::ECursorType::Crosshair:        CursorType = GDK_CROSSHAIR;
                                                        break;
        default:                                        break;
    }
    
    return std::make_shared<CGUICursorGTK3>(gdk_cursor_new_for_display(DDisplay, CursorType));
}
        
CGUICursorGTK3::CGUICursorGTK3(GdkCursor *cursor) : CGUICursor(){
    DCursor = cursor;
}

CGUICursorGTK3::~CGUICursorGTK3(){
    
}

CGUICursor::ECursorType CGUICursorGTK3::Type(){
    switch(gdk_cursor_get_cursor_type(DCursor)){
        case GDK_CENTER_PTR:    return ECursorType::CenterPointer;
        case GDK_LEFT_PTR:      return ECursorType::LeftPointer;
        case GDK_RIGHT_PTR:     return ECursorType::RightPointer;
        case GDK_CROSSHAIR:     return ECursorType::Crosshair;
        default:                return ECursorType::Blank;
    }
}


CGUIWidgetGTK3::CGUIWidgetGTK3(GtkWidget *widget, bool reference){
    DWidget = widget;
    //if(reference){ // Seems to fail on exit if not always referenced.
        g_object_ref(DWidget);
    //}
    
    DActivateCalldata = nullptr;
    DActivateCallback = nullptr;
    
    DButtonPressCalldata = nullptr;
    DButtonPressCallback = nullptr;
    
    DButtonReleaseCalldata = nullptr;
    DButtonReleaseCallback = nullptr;
    
    DMotionCalldata = nullptr;
    DMotionCallback = nullptr;
    
    DKeyPressCalldata = nullptr; 
    DKeyPressCallback = nullptr;
    
    DKeyReleaseCalldata = nullptr; 
    DKeyReleaseCallback = nullptr;
    
    DExposeCalldata = nullptr;
    DExposeCallback = nullptr;
    
    DConfigureCalldata = nullptr;
    DConfigureCallback = nullptr;
    
    DDrawCalldata = nullptr;
    DDrawCallback = nullptr;
}

CGUIWidgetGTK3::~CGUIWidgetGTK3(){
    if(DWidget){
        gtk_widget_destroy(DWidget);
    }
}


void CGUIWidgetGTK3::ActivateEventCallback(GtkWidget *widget, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DActivateCallback){
        Widget->DActivateCallback(Widget->shared_from_this(),Widget->DActivateCalldata);
    }
}

gboolean CGUIWidgetGTK3::ButtonPressEventCallback(GtkWidget *widget, GdkEventButton *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DButtonPressCallback){
        SGUIButtonEvent TempEvent;
        TempEvent.DType.DType = event->type;
        TempEvent.DWindowX = event->x;
        TempEvent.DWindowY = event->y;
        TempEvent.DModifier.DState = event->state;
        TempEvent.DButton = event->button;
        TempEvent.DScreenX = event->x_root;
        TempEvent.DScreenY = event->y_root;

        return Widget->DButtonPressCallback(Widget->shared_from_this(), TempEvent, Widget->DButtonPressCalldata);
    }
    return FALSE;
}

gboolean CGUIWidgetGTK3::ButtonReleaseEventCallback(GtkWidget *widget, GdkEventButton *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DButtonPressCallback){
        SGUIButtonEvent TempEvent;
        TempEvent.DType.DType = event->type;
        TempEvent.DWindowX = event->x;
        TempEvent.DWindowY = event->y;
        TempEvent.DModifier.DState = event->state;
        TempEvent.DButton = event->button;
        TempEvent.DScreenX = event->x_root;
        TempEvent.DScreenY = event->y_root;

        return Widget->DButtonReleaseCallback(Widget->shared_from_this(), TempEvent, Widget->DButtonReleaseCalldata);
    }
    return FALSE;
}

gboolean CGUIWidgetGTK3::DeleteEventCallback(GtkWidget *widget, GdkEvent *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DDeleteCallback){
        Widget->DDeleteCallback(Widget->shared_from_this(), Widget->DDeleteCalldata);
    }
    return FALSE;
}

void CGUIWidgetGTK3::DestroyEventCallback(GtkWidget *widget, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DDestroyCallback){
        Widget->DDestroyCallback(Widget->shared_from_this(), Widget->DDestroyCalldata);
    }
}


gboolean CGUIWidgetGTK3::MotionNotifyEventCallback(GtkWidget *widget, GdkEventMotion *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DMotionCallback){
        SGUIMotionEvent TempEvent;

        TempEvent.DWindowX = event->x;
        TempEvent.DWindowY = event->y;
        TempEvent.DModifier.DState = event->state;
        TempEvent.DScreenX = event->x_root;
        TempEvent.DScreenY = event->y_root;
        
        return Widget->DMotionCallback(Widget->shared_from_this(), TempEvent, Widget->DMotionCalldata);
    }
    return FALSE;
}

gboolean CGUIWidgetGTK3::KeyPressEventCallback(GtkWidget *widget, GdkEventKey *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DKeyPressCallback){
        SGUIKeyEvent TempEvent;

        TempEvent.DValue.DValue = event->keyval;
        TempEvent.DModifier.DState = event->state;
        
        return Widget->DKeyPressCallback(Widget->shared_from_this(), TempEvent, Widget->DKeyPressCalldata);
    }
    return FALSE;
}

gboolean CGUIWidgetGTK3::KeyReleaseEventCallback(GtkWidget *widget, GdkEventKey *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DKeyReleaseCallback){
        SGUIKeyEvent TempEvent;

        TempEvent.DValue.DValue = event->keyval;
        TempEvent.DModifier.DState = event->state;
        
        return Widget->DKeyReleaseCallback(Widget->shared_from_this(), TempEvent, Widget->DKeyReleaseCalldata);
    }
    return FALSE;
}

gboolean CGUIWidgetGTK3::ExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DExposeCallback){
        SGUIConfigureEvent TempEvent;

        TempEvent.DX = event->area.x;
        TempEvent.DY = event->area.y;
        TempEvent.DWidth = event->area.width;
        TempEvent.DHeight = event->area.height;
        
        return Widget->DExposeCallback(Widget->shared_from_this(), TempEvent, Widget->DExposeCalldata);
    }
    return FALSE;
}

gboolean CGUIWidgetGTK3::ConfigureEventCallback(GtkWidget *widget, GdkEventConfigure *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DConfigureCallback){
        SGUIConfigureEvent TempEvent;

        TempEvent.DX = event->x;
        TempEvent.DY = event->y;
        TempEvent.DWidth = event->width;
        TempEvent.DHeight = event->height;
        
        return Widget->DConfigureCallback(Widget->shared_from_this(), TempEvent, Widget->DConfigureCalldata);
    }
    return FALSE;
}

gboolean CGUIWidgetGTK3::DrawEventCallback(GtkWidget *widget, cairo_t *cr, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DDrawCallback){
        return Widget->DDrawCallback(Widget->shared_from_this(), std::make_shared<CGraphicResourceContextCairo>(cr, true), Widget->DDrawCalldata);
    }
    return FALSE;    
}

gboolean CGUIWidgetGTK3::ToggledEventCallback(GtkWidget *widget, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DToggledCallback){
        return Widget->DToggledCallback(Widget->shared_from_this(), Widget->DToggledCalldata);
    }
    return FALSE;    
}

gboolean CGUIWidgetGTK3::ChangedEventCallback(GtkWidget *widget, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DChangedCallback){
        return Widget->DChangedCallback(Widget->shared_from_this(), Widget->DChangedCalldata);
    }
    return FALSE;    
}

gboolean CGUIWidgetGTK3::ValueChangedEventCallback(GtkWidget *widget, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DValueChangedCallback){
        return Widget->DValueChangedCallback(Widget->shared_from_this(), Widget->DValueChangedCalldata);
    }
    return FALSE;    
}

gboolean CGUIWidgetGTK3::ScrollEventCallback(GtkWidget *widget, GdkEventScroll *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DScrollCallback){
        SGUIScrollEvent TempEvent;
        switch(event->direction){
            case GDK_SCROLL_UP:     TempEvent.DDirection = EGUIScrollDirection::Up;
                                    break;
            case GDK_SCROLL_DOWN:   TempEvent.DDirection = EGUIScrollDirection::Down;
                                    break;
            case GDK_SCROLL_LEFT:   TempEvent.DDirection = EGUIScrollDirection::Left;
                                    break;
            case GDK_SCROLL_RIGHT:  TempEvent.DDirection = EGUIScrollDirection::Right;
                                    break;
            case GDK_SCROLL_SMOOTH:
            default:                TempEvent.DDirection = EGUIScrollDirection::Smooth;
                                    break;
        }
        TempEvent.DDeltaX = event->delta_x;
        TempEvent.DDeltaY = event->delta_y;

        return Widget->DScrollCallback(Widget->shared_from_this(), TempEvent, Widget->DScrollCalldata);
    }
    return FALSE;    
}


void CGUIWidgetGTK3::Show(){
    gtk_widget_show(DWidget);
}

void CGUIWidgetGTK3::ShowAll(){
    gtk_widget_show_all(DWidget);
}


void CGUIWidgetGTK3::Hide(){
    gtk_widget_hide(DWidget);
}


int CGUIWidgetGTK3::AllocatedWidth(){
    GtkAllocation AllocatedArea;

    gtk_widget_get_allocation(DWidget, &AllocatedArea);
    return AllocatedArea.width;
}

int CGUIWidgetGTK3::AllocatedHeight(){
    GtkAllocation AllocatedArea;

    gtk_widget_get_allocation(DWidget, &AllocatedArea);
    return AllocatedArea.height;
}

void CGUIWidgetGTK3::SetSizeRequest(int width, int height){
    gtk_widget_set_size_request(DWidget, width, height);
}

void CGUIWidgetGTK3::SetVerticalExpand(bool exp){
    gtk_widget_set_vexpand(DWidget, exp ? TRUE : FALSE);
}

void CGUIWidgetGTK3::SetHorizontalExpand(bool exp){
    gtk_widget_set_hexpand(DWidget, exp ? TRUE : FALSE);
}

void CGUIWidgetGTK3::Invalidate(){
    GtkAllocation AllocatedArea;

    gtk_widget_get_allocation(DWidget, &AllocatedArea);
    
    gtk_widget_queue_draw_area(DWidget, 0, 0, AllocatedArea.width, AllocatedArea.height);
}

void CGUIWidgetGTK3::SetCursor(std::shared_ptr< CGUICursor > cursor){
    std::shared_ptr<CGUICursorGTK3> Cursor = std::dynamic_pointer_cast<CGUICursorGTK3>(cursor);
    
    gdk_window_set_cursor(gtk_widget_get_window(DWidget), Cursor->DCursor);
}

std::shared_ptr<CGraphicSurface> CGUIWidgetGTK3::CreateSimilarSurface(int width, int height){
    return std::make_shared<CGraphicSurfaceCairo>(gdk_window_create_similar_image_surface(gtk_widget_get_window(DWidget),
                                                       CAIRO_FORMAT_ARGB32,
                                                       width,
                                                       height, 0));
}

std::shared_ptr<CGraphicResourceContext> CGUIWidgetGTK3::CreateResourceContext(){
#if GTK_CHECK_VERSION(3, 22, 0) //(GDK_VERSION_MAX_ALLOWED >= GDK_VERSION_3_22)
    GtkAllocation AllocatedArea;

    gtk_widget_get_allocation(DWidget, &AllocatedArea);
    cairo_rectangle_int_t WidgetRectangle{AllocatedArea.x, AllocatedArea.y, AllocatedArea.width, AllocatedArea.height};
    cairo_region_t *Region = cairo_region_create_rectangle(&WidgetRectangle);
    GdkWindow *Window = gtk_widget_get_window(DWidget);
    GdkDrawingContext *DrawingContext = gdk_window_begin_draw_frame(Window, Region);
    cairo_region_destroy(Region);
    
    return std::make_shared<CGraphicResourceContextCairo>(Window, DrawingContext);
#else
    GdkWindow *Window = gtk_widget_get_window(DWidget);
    return std::make_shared<CGraphicResourceContextCairo>(gdk_cairo_create(Window));
#endif
}

void CGUIWidgetGTK3::EnableEvent(EGUIEvent event){
    gint CurrentMask = gtk_widget_get_events(DWidget);
    switch(event){
        case EGUIEvent::Expose:         CurrentMask |= GDK_EXPOSURE_MASK;
                                        break;
        case EGUIEvent::Motion:         CurrentMask |= GDK_POINTER_MOTION_MASK;
                                        break;
        case EGUIEvent::ButtonPress:    CurrentMask |= GDK_BUTTON_PRESS_MASK;
                                        break;
        case EGUIEvent::ButtonRelease:  CurrentMask |= GDK_BUTTON_RELEASE_MASK;
                                        break;
        case EGUIEvent::KeyPress:       CurrentMask |= GDK_KEY_PRESS_MASK;
                                        break;
        case EGUIEvent::KeyRelease:     CurrentMask |= GDK_KEY_RELEASE_MASK;
                                        break;
        case EGUIEvent::Scroll:         CurrentMask |= GDK_SCROLL_MASK;
                                        break;
        default:                        break;
    }
    gtk_widget_set_events(DWidget, CurrentMask);
}

void CGUIWidgetGTK3::DisableEvent(EGUIEvent event){
    gint CurrentMask = gtk_widget_get_events(DWidget);
    switch(event){
        case EGUIEvent::Expose:         CurrentMask &= ~GDK_EXPOSURE_MASK;
                                        break;
        case EGUIEvent::Motion:         CurrentMask &= ~GDK_POINTER_MOTION_MASK;
                                        break;
        case EGUIEvent::ButtonPress:    CurrentMask &= ~GDK_BUTTON_PRESS_MASK;
                                        break;
        case EGUIEvent::ButtonRelease:  CurrentMask &= ~GDK_BUTTON_RELEASE_MASK;
                                        break;
        case EGUIEvent::KeyPress:       CurrentMask &= ~GDK_KEY_PRESS_MASK;
                                        break;
        case EGUIEvent::KeyRelease:     CurrentMask &= ~GDK_KEY_RELEASE_MASK;
                                        break;
        case EGUIEvent::Scroll:         CurrentMask &= ~GDK_SCROLL_MASK;
                                        break;
        default:                        break;
    }
    gtk_widget_set_events(DWidget, CurrentMask);
}

void CGUIWidgetGTK3::SetActivateEventCallback(TGUICalldata calldata, TGUIActivateEventCallback callback){
    DActivateCalldata = calldata;
    DActivateCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "activate", G_CALLBACK(ActivateEventCallback), this);
    }
}
                                       
void CGUIWidgetGTK3::SetButtonPressEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback){
    DButtonPressCalldata = calldata;
    DButtonPressCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "button-press-event", G_CALLBACK(ButtonPressEventCallback), this);
    }
}

void CGUIWidgetGTK3::SetButtonReleaseEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback){
    DButtonReleaseCalldata = calldata;
    DButtonReleaseCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "button-release-event", G_CALLBACK(ButtonReleaseEventCallback), this);
    }
}

void CGUIWidgetGTK3::SetDeleteEventCallback(TGUICalldata calldata, TGUIDeleteEventCallback callback){
    DDeleteCalldata = calldata;
    DDeleteCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "delete-event", G_CALLBACK(DeleteEventCallback), this);
    }
}

void CGUIWidgetGTK3::SetDestroyEventCallback(TGUICalldata calldata, TGUIDestroyEventCallback callback){
    DDestroyCalldata = calldata;
    DDestroyCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "destroy", G_CALLBACK(DestroyEventCallback), this);
    }
}

void CGUIWidgetGTK3::SetMotionEventCallback(TGUICalldata calldata, TGUIMotionEventCallback callback){
    DMotionCalldata = calldata;
    DMotionCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "motion-notify-event", G_CALLBACK(MotionNotifyEventCallback), this);
    }
}                                                                   

void CGUIWidgetGTK3::SetKeyPressEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback){
    DKeyPressCalldata = calldata;
    DKeyPressCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "key-press-event", G_CALLBACK(KeyPressEventCallback), this);
    }
}

void CGUIWidgetGTK3::SetKeyReleaseEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback){
    DKeyReleaseCalldata = calldata;
    DKeyReleaseCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "key-release-event", G_CALLBACK(KeyReleaseEventCallback), this);
    }
}

void CGUIWidgetGTK3::SetExposeEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback){
    DExposeCalldata = calldata;
    DExposeCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "expose-event", G_CALLBACK(ExposeEventCallback), this);
    }
}

void CGUIWidgetGTK3::SetConfigureEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback){
    DConfigureCalldata = calldata;
    DConfigureCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "configure-event", G_CALLBACK(ConfigureEventCallback), this);
    }
}

void CGUIWidgetGTK3::SetDrawEventCallback(TGUICalldata calldata, TGUIDrawEventCallback callback){
    DDrawCalldata = calldata;
    DDrawCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "draw", G_CALLBACK(DrawEventCallback), this);
    }    
}

void CGUIWidgetGTK3::SetToggledEventCallback(TGUICalldata calldata, TGUIToggledEventCallback callback){
    DToggledCalldata = calldata;
    DToggledCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "toggled", G_CALLBACK(ToggledEventCallback), this);
    }    
}

void CGUIWidgetGTK3::SetChangedEventCallback(TGUICalldata calldata, TGUIChangedEventCallback callback){
    DChangedCalldata = calldata;
    DChangedCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "changed", G_CALLBACK(ChangedEventCallback), this);
    }    
}

void CGUIWidgetGTK3::SetValueChangedEventCallback(TGUICalldata calldata, TGUIValueChangedEventCallback callback){
    DValueChangedCalldata = calldata;
    DValueChangedCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "value-changed", G_CALLBACK(ValueChangedEventCallback), this);
    }    
}

void CGUIWidgetGTK3::SetScrollEventCallback(TGUICalldata calldata, TGUIScrollEventCallback callback){
    DScrollCalldata = calldata;
    DScrollCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "scroll-event", G_CALLBACK(ScrollEventCallback), this);
    }    
}

CGUIContainerGTK3::CGUIContainerGTK3(GtkWidget *widget, bool reference) : CGUIWidgetGTK3(widget, reference){
    
}

CGUIContainerGTK3::~CGUIContainerGTK3(){
    
}

void CGUIContainerGTK3::SetBorderWidth(int width){
    gtk_container_set_border_width(GTK_CONTAINER(DWidget), width);
}

void CGUIContainerGTK3::Add(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_container_add(GTK_CONTAINER(DWidget), WidgetToAdd->Widget());
    DChildren.insert(widget);
}

void CGUIContainerGTK3::Remove(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_container_remove(GTK_CONTAINER(DWidget), WidgetToAdd->Widget());    
    DChildren.erase(widget);
}

CGUILabelGTK3::CGUILabelGTK3(GtkWidget *widget, bool reference) : CGUIWidgetGTK3(widget, reference){
    
}

CGUILabelGTK3::~CGUILabelGTK3(){
    
}

std::string CGUILabelGTK3::GetText(){
    return gtk_label_get_text(GTK_LABEL(DWidget));
}

void CGUILabelGTK3::SetText(const std::string &str){
    gtk_label_set_text(GTK_LABEL(DWidget), str.c_str());
}

void CGUILabelGTK3::SetMarkup(const std::string &str){
    gtk_label_set_markup(GTK_LABEL(DWidget), str.c_str());
}

void CGUILabelGTK3::SetJustification(const SGUIJustificationType &justify){
    gtk_label_set_justify(GTK_LABEL(DWidget), GtkJustification(justify.DType));
    if(justify.DType == SGUIJustificationType::Right){
        gtk_label_set_xalign(GTK_LABEL(DWidget), 1.0);
    }
    else if(justify.DType == SGUIJustificationType::Center){
        gtk_label_set_xalign(GTK_LABEL(DWidget), 0.5);
    }
    else{
        gtk_label_set_xalign(GTK_LABEL(DWidget), 0.0);
    }
}

void CGUILabelGTK3::SetFontFamily(const std::string &family){
    auto AttrList = gtk_label_get_attributes(GTK_LABEL(DWidget));
    auto FontAttr = pango_attr_family_new(family.c_str());
    if(!AttrList){
        AttrList = pango_attr_list_new();    
        pango_attr_list_insert(AttrList,FontAttr);
    }
    else{
        pango_attr_list_ref(AttrList);
        pango_attr_list_change(AttrList,FontAttr);
    }
    gtk_label_set_attributes(GTK_LABEL(DWidget), AttrList);
    pango_attr_list_unref(AttrList);
    //pango_attribute_destroy(FontAttr); // destroying causes segfault must be handed over to list
    
}

void CGUILabelGTK3::SetBold(bool bold){
    auto AttrList = gtk_label_get_attributes(GTK_LABEL(DWidget));
    auto BoldAttr = pango_attr_weight_new(bold ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
    if(!AttrList){
        AttrList = pango_attr_list_new();    
        pango_attr_list_insert(AttrList,BoldAttr);
    }
    else{
        pango_attr_list_ref(AttrList);
        pango_attr_list_change(AttrList,BoldAttr);
    }
    gtk_label_set_attributes(GTK_LABEL(DWidget), AttrList);
    pango_attr_list_unref(AttrList);

}

void CGUILabelGTK3::SetWidthCharacters(int chars){
    gtk_label_set_width_chars(GTK_LABEL(DWidget), chars);
}

void CGUILabelGTK3::SetMaxWidthCharacters(int chars){
    gtk_label_set_max_width_chars(GTK_LABEL(DWidget), chars);
}

CGUIDrawingAreaGTK3::CGUIDrawingAreaGTK3(GtkWidget *widget, bool reference) : CGUIWidgetGTK3(widget, reference){
    
}

CGUIDrawingAreaGTK3::~CGUIDrawingAreaGTK3(){

}

CGUIBoxGTK3::CGUIBoxGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIBoxGTK3::~CGUIBoxGTK3(){

}

void CGUIBoxGTK3::PackStart(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_box_pack_start(GTK_BOX(DWidget), WidgetToAdd->Widget(), expand, fill, padding);
    DChildren.insert(widget);
}

void CGUIBoxGTK3::PackEnd(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_box_pack_end(GTK_BOX(DWidget), WidgetToAdd->Widget(), expand, fill, padding);
    DChildren.insert(widget);
}

CGUIEventBoxGTK3::CGUIEventBoxGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIEventBoxGTK3::~CGUIEventBoxGTK3(){

}

CGUIFrameGTK3::CGUIFrameGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIFrameGTK3::~CGUIFrameGTK3(){

}

void CGUIFrameGTK3::SetLabel(const std::string &label){
    gtk_frame_set_label(GTK_FRAME(DWidget), label.c_str());
}

CGUIButtonGTK3::CGUIButtonGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIButtonGTK3::~CGUIButtonGTK3(){

}

std::string CGUIButtonGTK3::GetLabel() const{
    if(auto Label = gtk_button_get_label(GTK_BUTTON(DWidget))){
        return std::string((const char *)Label);
    }
    return std::string();
}

void CGUIButtonGTK3::SetLabel(const std::string &label){
    gtk_button_set_label(GTK_BUTTON(DWidget),(const gchar *)label.c_str());
}

CGUIComboBoxGTK3::CGUIComboBoxGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){
    DItemCount = 0;
    gtk_combo_box_set_id_column(GTK_COMBO_BOX(DWidget),0);
    DCellRenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(DWidget), DCellRenderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(DWidget), DCellRenderer,
                                   "text", 0,
                                   NULL);

}

CGUIComboBoxGTK3::~CGUIComboBoxGTK3(){

}

void CGUIComboBoxGTK3::SetFontFamily(const std::string &family){
    g_object_set(DCellRenderer, "family", family.c_str(), NULL);
}

int CGUIComboBoxGTK3::ItemCount() const{
    return DItemCount;
}

std::string CGUIComboBoxGTK3::GetItem(int item) const{
    if((0 <= item)&&(item < DItemCount)){
        auto Model = gtk_combo_box_get_model(GTK_COMBO_BOX(DWidget));
        GtkTreeIter Iterator;
        if(gtk_tree_model_get_iter_from_string(Model,&Iterator,std::to_string(item).c_str())){
            gchar *StringData;
            gtk_tree_model_get(Model, &Iterator,0,&StringData,-1);
            return std::string((char *)StringData);
        }
    }
    return std::string();
}

void CGUIComboBoxGTK3::SetItem(int item, const std::string &str){
    if((0 <= item)&&(item < DItemCount)){
        auto Model = gtk_combo_box_get_model(GTK_COMBO_BOX(DWidget));
        GtkTreeIter Iterator;
        if(gtk_tree_model_get_iter_from_string(Model,&Iterator,std::to_string(item).c_str())){
            gtk_list_store_set(GTK_LIST_STORE(Model),&Iterator,0,str.c_str(),-1);
        }
    }
}

void CGUIComboBoxGTK3::AppendItem(const std::string &str){
    auto Model = gtk_combo_box_get_model(GTK_COMBO_BOX(DWidget));
    GtkTreeIter Iterator;
    gtk_list_store_append(GTK_LIST_STORE(Model),&Iterator);
    gtk_list_store_set(GTK_LIST_STORE(Model),&Iterator,0,str.c_str(),-1);
    DItemCount++;
}

void CGUIComboBoxGTK3::ClearItems(){
    auto Model = gtk_combo_box_get_model(GTK_COMBO_BOX(DWidget));
    gtk_list_store_clear(GTK_LIST_STORE(Model));
    DItemCount = 0;
}

int CGUIComboBoxGTK3::GetActiveItem() const{
    return gtk_combo_box_get_active(GTK_COMBO_BOX(DWidget));
}

void CGUIComboBoxGTK3::SetActiveItem(int item){
    if((0 <= item)&&(item < DItemCount)){
        gtk_combo_box_set_active(GTK_COMBO_BOX(DWidget),item);
    }
}

CGUIToggleButtonGTK3::CGUIToggleButtonGTK3(GtkWidget *widget, bool reference) : CGUIButtonGTK3(widget, reference){
    
}

CGUIToggleButtonGTK3::~CGUIToggleButtonGTK3(){

}

bool CGUIToggleButtonGTK3::GetActive() const{
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(DWidget));
}

void CGUIToggleButtonGTK3::SetActive(bool active){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(DWidget), active ? TRUE : FALSE);
}

CGUIGridGTK3::CGUIGridGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIGridGTK3::~CGUIGridGTK3(){

}

unsigned int CGUIGridGTK3::GetRowSpacing() const{
    return gtk_grid_get_row_spacing(GTK_GRID(DWidget));
}

void CGUIGridGTK3::SetRowSpacing(unsigned int spacing){
    gtk_grid_set_row_spacing(GTK_GRID(DWidget), spacing);
}

unsigned int CGUIGridGTK3::GetColumnSpacing() const{
    return gtk_grid_get_column_spacing(GTK_GRID(DWidget));
}

void CGUIGridGTK3::SetColumnSpacing(unsigned int spacing){
    gtk_grid_set_column_spacing(GTK_GRID(DWidget), spacing);
}

void CGUIGridGTK3::Attach(std::shared_ptr<CGUIWidget> widget, int left, int top, int width, int height){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);

    gtk_grid_attach(GTK_GRID(DWidget), WidgetToAdd->Widget(), left, top, width, height);
    DChildren.insert(widget);
}

CGUIMenuShellGTK3::CGUIMenuShellGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){
    
}

CGUIMenuShellGTK3::~CGUIMenuShellGTK3(){
    
}

void CGUIMenuShellGTK3::Append(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(DWidget), WidgetToAdd->Widget());
}

CGUIMenuGTK3::CGUIMenuGTK3(GtkWidget *widget, bool reference) : CGUIMenuShellGTK3(widget, reference){
    
}

CGUIMenuGTK3::~CGUIMenuGTK3(){
        
}

CGUIMenuBarGTK3::CGUIMenuBarGTK3(GtkWidget *widget, bool reference) : CGUIMenuShellGTK3(widget, reference){
    
}

CGUIMenuBarGTK3::~CGUIMenuBarGTK3(){
    
}

CGUIMenuItemGTK3::CGUIMenuItemGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){
    
}

CGUIMenuItemGTK3::~CGUIMenuItemGTK3(){
    
}

std::shared_ptr<CGUILabel> CGUIMenuItemGTK3::GetLabel(){
    GList *List = gtk_container_get_children(GTK_CONTAINER(DWidget));
    
    return std::make_shared<CGUILabelGTK3>(GTK_WIDGET(List->data), true);
}

void CGUIMenuItemGTK3::SetSubMenu(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(DWidget), WidgetToAdd->Widget());
}

CGUIScrollBarGTK3::CGUIScrollBarGTK3(GtkWidget *widget, bool reference) : CGUIWidgetGTK3(widget,reference){

}

CGUIScrollBarGTK3::~CGUIScrollBarGTK3(){
    
}

double CGUIScrollBarGTK3::GetValue(){
    return gtk_range_get_value(GTK_RANGE(DWidget));
}

void CGUIScrollBarGTK3::SetValue(double val){
    gtk_range_set_value(GTK_RANGE(DWidget),val);
}

void CGUIScrollBarGTK3::SetLower(double lower){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    gtk_adjustment_set_upper(Adjustment, lower);
}

void CGUIScrollBarGTK3::SetUpper(double upper){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    gtk_adjustment_set_upper(Adjustment, upper);
}

void CGUIScrollBarGTK3::SetStepIncrement(double inc){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    gtk_adjustment_set_step_increment(Adjustment, inc);
}

void CGUIScrollBarGTK3::SetPageIncrement(double inc){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    gtk_adjustment_set_page_increment(Adjustment, inc);
}

void CGUIScrollBarGTK3::SetPageSize(double size){
    auto Adjustment = gtk_range_get_adjustment(GTK_RANGE(DWidget));
    gtk_adjustment_set_page_size(Adjustment, size);
}

CGUIWindowGTK3::CGUIWindowGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIWindowGTK3::~CGUIWindowGTK3(){
    
}

void CGUIWindowGTK3::SetMinSize(int width, int height){
    GdkGeometry Geometry;
    Geometry.min_width = width;
    Geometry.min_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), NULL, &Geometry, GDK_HINT_MIN_SIZE);
}

void CGUIWindowGTK3::SetMaxSize(int width, int height){
    GdkGeometry Geometry;
    Geometry.max_width = width;
    Geometry.max_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), NULL, &Geometry, GDK_HINT_MAX_SIZE);    
}

void CGUIWindowGTK3::SetMinSize(std::shared_ptr<CGUIWidget> widget, int width, int height){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToSize = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    GdkGeometry Geometry;
    Geometry.min_width = width;
    Geometry.min_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), WidgetToSize->Widget(), &Geometry, GDK_HINT_MIN_SIZE);
}

void CGUIWindowGTK3::SetMaxSize(std::shared_ptr<CGUIWidget> widget, int width, int height){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToSize = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    GdkGeometry Geometry;
    Geometry.max_width = width;
    Geometry.max_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), WidgetToSize->Widget(), &Geometry, GDK_HINT_MAX_SIZE);    
}

void CGUIWindowGTK3::SetTitle(const std::string &title){
    gtk_window_set_title(GTK_WINDOW(DWidget), title.c_str());
}

void CGUIWindowGTK3::Close(){
    gtk_window_close(GTK_WINDOW(DWidget));
}

CGUIFileFilterGTK3::CGUIFileFilterGTK3(GtkFileFilter *filter, bool reference){
    DFilter = filter;
    //if(reference){
        g_object_ref(DFilter);   
    //}
}

CGUIFileFilterGTK3::~CGUIFileFilterGTK3(){
    g_object_unref(DFilter);    
}

void CGUIFileFilterGTK3::AddPattern(const std::string &pattern){
    gtk_file_filter_add_pattern(DFilter, pattern.c_str());
}

CGUIFileChooserDialogGTK3::CGUIFileChooserDialogGTK3(GtkWidget *widget, bool reference) : CGUIWindowGTK3(widget, reference){
    
}

CGUIFileChooserDialogGTK3::~CGUIFileChooserDialogGTK3(){

}

void CGUIFileChooserDialogGTK3::SetFilter(std::shared_ptr<CGUIFileFilter> filter){
    std::shared_ptr<CGUIFileFilterGTK3> FilterToAdd = std::dynamic_pointer_cast<CGUIFileFilterGTK3>(filter);
    
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(DWidget), FilterToAdd->DFilter);
}

std::string CGUIFileChooserDialogGTK3::GetCurrentFolder(){
    return std::string((char *)gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(DWidget)));
}

void CGUIFileChooserDialogGTK3::SetCurrentFolder(const std::string &folder){
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(DWidget), folder.c_str());    
}

std::string CGUIFileChooserDialogGTK3::GetFilename(){
    gchar *Filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(DWidget));
    std::string ReturnName((char *)Filename);
    g_free(Filename);
    return ReturnName;
}

int CGUIFileChooserDialogGTK3::Run(){
    return GTK_RESPONSE_CANCEL == gtk_dialog_run(GTK_DIALOG(DWidget)) ? 0 : 1;
}

