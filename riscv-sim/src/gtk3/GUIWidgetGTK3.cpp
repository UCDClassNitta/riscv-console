#include "GUIWidgetGTK3.h"
#include "GUICursorGTK3.h"
#include "GUIEventGTK3.h"
#include "GraphicResourceContextCairo.h"

CGUIWidgetGTK3::CGUIWidgetGTK3(GtkWidget *widget, bool reference){
    DWidget = widget;
    //if(reference){ // Seems to fail on exit if not always referenced.
        g_object_ref(DWidget);
    //}
    DPendingSetCursor = false;

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
    g_signal_connect(DWidget, "realize", G_CALLBACK(RealizeEventCallback), this);
}

CGUIWidgetGTK3::~CGUIWidgetGTK3(){
    if(DWidget){
        gtk_widget_destroy(DWidget);
    }
}

void CGUIWidgetGTK3::RealizeEventCallback(GtkWidget *widget, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DPendingSetCursor){
        Widget->SetCursor(Widget->DCursorToSet);
        Widget->DCursorToSet.reset();
        Widget->DPendingSetCursor = false;
    }
    Widget->Realize();
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
        TempEvent.DType = SGUIButtonEventTypeGTK3::TranslateFromGTK3Type(event->type);
        //TempEvent.DType.DType = event->type;
        TempEvent.DWindowX = event->x;
        TempEvent.DWindowY = event->y;
        TempEvent.DModifier = SGUIModifierTypeGTK3::TranslateFromGTK3Type(event->state);
        //TempEvent.DModifier.DState = event->state;
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
        TempEvent.DType = SGUIButtonEventTypeGTK3::TranslateFromGTK3Type(event->type);
        //TempEvent.DType.DType = event->type;
        TempEvent.DWindowX = event->x;
        TempEvent.DWindowY = event->y;
        TempEvent.DModifier = SGUIModifierTypeGTK3::TranslateFromGTK3Type(event->state);
        //TempEvent.DModifier.DState = event->state;
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
        TempEvent.DModifier = SGUIModifierTypeGTK3::TranslateFromGTK3Type(event->state);
        //TempEvent.DModifier.DState = event->state;
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
        TempEvent.DValue = SGUIKeyTypeGTK3::TranslateFromGTK3Type(event->keyval);
        //TempEvent.DValue.DValue = event->keyval;
        TempEvent.DModifier = SGUIModifierTypeGTK3::TranslateFromGTK3Type(event->state);
        //TempEvent.DModifier.DState = event->state;
        
        return Widget->DKeyPressCallback(Widget->shared_from_this(), TempEvent, Widget->DKeyPressCalldata);
    }
    return FALSE;
}

gboolean CGUIWidgetGTK3::KeyReleaseEventCallback(GtkWidget *widget, GdkEventKey *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DKeyReleaseCallback){
        SGUIKeyEvent TempEvent;
        TempEvent.DValue = SGUIKeyTypeGTK3::TranslateFromGTK3Type(event->keyval);
        //TempEvent.DValue.DValue = event->keyval;
        TempEvent.DModifier = SGUIModifierTypeGTK3::TranslateFromGTK3Type(event->state);
        //TempEvent.DModifier.DState = event->state;
        
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
    GdkCursor *NewCursor = NULL;
    if(cursor){
        std::shared_ptr<CGUICursorGTK3> Cursor = std::dynamic_pointer_cast<CGUICursorGTK3>(cursor);
        NewCursor = Cursor->GetCursor();
    }
    auto WidgetWindow = gtk_widget_get_window(DWidget);
    if(WidgetWindow){
        gdk_window_set_cursor(WidgetWindow, NewCursor);
    }
    else{
        DPendingSetCursor = true;
        DCursorToSet = cursor;
    }
}

void CGUIWidgetGTK3::SetTooltipText(const std::string &tip){
    gtk_widget_set_tooltip_text(DWidget,tip.c_str());
}

void CGUIWidgetGTK3::SetTooltipMarkup(const std::string &markup){
    gtk_widget_set_tooltip_markup(DWidget,markup.c_str());
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
        case EGUIEvent::Configure:      //CurrentMask |= GDK_STRUCTURE_MASK;
                                        {
                                            auto Window = gtk_widget_get_window(DWidget);
                                            if(Window){
                                                gdk_window_set_events(Window, GdkEventMask(gdk_window_get_events(Window) | GDK_STRUCTURE_MASK));
                                            }
                                        }
                                        break;
        case EGUIEvent::Property:       CurrentMask |= GDK_PROPERTY_CHANGE_MASK;
                                        {
                                            auto Window = gtk_widget_get_window(DWidget);
                                            if(Window){
                                                gdk_window_set_events(Window, GdkEventMask(gdk_window_get_events(Window) | GDK_PROPERTY_CHANGE_MASK));
                                            }
                                        }
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
        case EGUIEvent::Configure:      CurrentMask &= ~GDK_STRUCTURE_MASK;
                                        break;
        case EGUIEvent::Property:       CurrentMask &= ~GDK_PROPERTY_CHANGE_MASK;
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
