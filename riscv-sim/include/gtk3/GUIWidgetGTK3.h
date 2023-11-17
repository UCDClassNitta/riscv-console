#ifndef GUIWIDGETGTK3_H
#define GUIWIDGETGTK3_H
#include "GUIWidget.h"
#include <gtk/gtk.h>

class CGUIWidgetGTK3 : public virtual CGUIWidget, public std::enable_shared_from_this<CGUIWidget>{
    protected:
        GtkWidget *DWidget;
        bool DPendingSetCursor;
        std::shared_ptr< CGUICursor > DCursorToSet;

        TGUICalldata DActivateCalldata;
        TGUIActivateEventCallback DActivateCallback;
        
        TGUICalldata DButtonPressCalldata;
        TGUIButtonEventCallback DButtonPressCallback;
        
        TGUICalldata DButtonReleaseCalldata;
        TGUIButtonEventCallback DButtonReleaseCallback;
        
        TGUICalldata DDeleteCalldata;
        TGUIDeleteEventCallback DDeleteCallback;
        
        TGUICalldata DDestroyCalldata;
        TGUIDestroyEventCallback DDestroyCallback;
        
        TGUICalldata DMotionCalldata;
        TGUIMotionEventCallback DMotionCallback;
        
        TGUICalldata DKeyPressCalldata; 
        TGUIKeyEventCallback DKeyPressCallback;
        
        TGUICalldata DKeyReleaseCalldata; 
        TGUIKeyEventCallback DKeyReleaseCallback;
        
        TGUICalldata DExposeCalldata;
        TGUIConfigureEventCallback DExposeCallback;
        
        TGUICalldata DConfigureCalldata;
        TGUIConfigureEventCallback DConfigureCallback;
        
        TGUICalldata DDrawCalldata;
        TGUIDrawEventCallback DDrawCallback;

        TGUICalldata DToggledCalldata;
        TGUIToggledEventCallback DToggledCallback;

        TGUICalldata DChangedCalldata;
        TGUIChangedEventCallback DChangedCallback;

        TGUICalldata DValueChangedCalldata;
        TGUIValueChangedEventCallback DValueChangedCallback;

        TGUICalldata DScrollCalldata;
        TGUIScrollEventCallback DScrollCallback;
        
        static void RealizeEventCallback(GtkWidget *widget, gpointer data);
        static void ActivateEventCallback(GtkWidget *widget, gpointer data);
        static gboolean ButtonPressEventCallback(GtkWidget *widget, GdkEventButton *event, gpointer data);
        static gboolean ButtonReleaseEventCallback(GtkWidget *widget, GdkEventButton *event, gpointer data);
        static gboolean DeleteEventCallback(GtkWidget *widget, GdkEvent *event, gpointer data);
        static void DestroyEventCallback(GtkWidget *widget, gpointer data);
        static gboolean MotionNotifyEventCallback(GtkWidget *widget, GdkEventMotion *event, gpointer data);
        static gboolean KeyPressEventCallback(GtkWidget *widget, GdkEventKey *event, gpointer data);
        static gboolean KeyReleaseEventCallback(GtkWidget *widget, GdkEventKey *event, gpointer data);
        static gboolean ExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data);
        static gboolean ConfigureEventCallback(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
        static gboolean DrawEventCallback(GtkWidget *widget, cairo_t *cr, gpointer data);
        static gboolean ToggledEventCallback(GtkWidget *widget, gpointer data);
        static gboolean ChangedEventCallback(GtkWidget *widget, gpointer data);
        static gboolean ValueChangedEventCallback(GtkWidget *widget, gpointer data);
        static gboolean ScrollEventCallback(GtkWidget *widget, GdkEventScroll *event, gpointer data);
        
    public:
        CGUIWidgetGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIWidgetGTK3();
        
        GtkWidget *Widget(){
            return DWidget;  
        };
        
        virtual void Realize(){};
        virtual void Show() override;
        virtual void ShowAll() override;
        
        virtual void Hide() override;
        
        virtual int AllocatedWidth() override;
        virtual int AllocatedHeight() override;
        
        virtual void SetSizeRequest(int width, int height) override;

        virtual void SetVerticalExpand(bool exp) override;
        virtual void SetHorizontalExpand(bool exp) override;

        virtual void Invalidate() override;
        
        virtual void SetCursor(std::shared_ptr< CGUICursor > cursor) override;

        virtual void SetTooltipText(const std::string &tip) override;
        virtual void SetTooltipMarkup(const std::string &markup) override;
        
        virtual std::shared_ptr<CGraphicSurface> CreateSimilarSurface(int width, int height) override;
        virtual std::shared_ptr<CGraphicResourceContext> CreateResourceContext() override;
        
        virtual void EnableEvent(EGUIEvent event) override;
        virtual void DisableEvent(EGUIEvent event) override;
        
        virtual void SetActivateEventCallback(TGUICalldata calldata, TGUIActivateEventCallback callback) override;
        virtual void SetButtonPressEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback) override;
        virtual void SetButtonReleaseEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback) override;
        virtual void SetDeleteEventCallback(TGUICalldata calldata, TGUIDeleteEventCallback callback) override;
        virtual void SetDestroyEventCallback(TGUICalldata calldata, TGUIDestroyEventCallback callback) override;
        virtual void SetMotionEventCallback(TGUICalldata calldata, TGUIMotionEventCallback callback) override;
        virtual void SetKeyPressEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback) override;
        virtual void SetKeyReleaseEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback) override;
        virtual void SetExposeEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback) override;
        virtual void SetConfigureEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback) override;
        virtual void SetDrawEventCallback(TGUICalldata calldata, TGUIDrawEventCallback callback) override;
        virtual void SetToggledEventCallback(TGUICalldata calldata, TGUIToggledEventCallback callback) override;
        virtual void SetChangedEventCallback(TGUICalldata calldata, TGUIChangedEventCallback callback) override;
        virtual void SetValueChangedEventCallback(TGUICalldata calldata, TGUIValueChangedEventCallback callback) override;
        virtual void SetScrollEventCallback(TGUICalldata calldata, TGUIScrollEventCallback callback) override;
};
#endif

