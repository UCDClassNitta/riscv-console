
#ifndef GUIFACTORYGTK3_H
#define GUIFACTORYGTK3_H
#include "GUIFactory.h"
#include "PeriodicTimeout.h"
#include <gtk/gtk.h>
#include <set>

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

class CGUIDisplayGTK3 : public CGUIDisplay{
    protected:
        GdkDisplay *DDisplay;
    public:
        CGUIDisplayGTK3(GdkDisplay *display);
        virtual ~CGUIDisplayGTK3();
        
        void Sync() override;
        void Flush() override;
        
        std::shared_ptr< CGUICursor > NewCursor(CGUICursor::ECursorType type) override;
};

class CGUICursorGTK3 : public CGUICursor{
    friend class CGUIWidgetGTK3;
    protected:
        GdkCursor *DCursor;
    public:
        
        CGUICursorGTK3(GdkCursor *cursor);
        virtual ~CGUICursorGTK3();
        
        ECursorType Type() override;
};

class CGUIWidgetGTK3 : public virtual CGUIWidget, public std::enable_shared_from_this<CGUIWidget>{
    protected:
        GtkWidget *DWidget;
        
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

class CGUIContainerGTK3 : public virtual CGUIContainer, public CGUIWidgetGTK3{
    protected:
        std::set< std::shared_ptr<CGUIWidget> > DChildren;
    public:
        CGUIContainerGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIContainerGTK3();
        
        void SetBorderWidth(int width) override;
        void Add(std::shared_ptr<CGUIWidget> widget) override;
        void Remove(std::shared_ptr<CGUIWidget> widget) override;
};

class CGUILabelGTK3 : public virtual CGUILabel, public CGUIWidgetGTK3{
    public:
        CGUILabelGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUILabelGTK3();
        
        std::string GetText() override;
        void SetText(const std::string &str) override;
        void SetMarkup(const std::string &str) override;
        void SetJustification(const SGUIJustificationType &justify) override;
        void SetFontFamily(const std::string &family) override;
        void SetBold(bool bold) override;
        void SetWidthCharacters(int chars) override;
        void SetMaxWidthCharacters(int chars) override;
};

class CGUIDrawingAreaGTK3 : public virtual CGUIDrawingArea, public CGUIWidgetGTK3{
    public:
        CGUIDrawingAreaGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIDrawingAreaGTK3();
};

class CGUIBoxGTK3 : public virtual CGUIBox, public CGUIContainerGTK3{
    public:
        CGUIBoxGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIBoxGTK3();
        
        void PackStart(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding) override;
        void PackEnd(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding) override;
};

class CGUIEventBoxGTK3 : public virtual CGUIEventBox, public CGUIContainerGTK3{
    public:
        CGUIEventBoxGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIEventBoxGTK3();
};

class CGUIFrameGTK3 : public virtual CGUIFrame, public CGUIContainerGTK3{
    public:
        CGUIFrameGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIFrameGTK3();
        
        void SetLabel(const std::string &label) override;
};

class CGUIButtonGTK3 : public virtual CGUIButton, public CGUIContainerGTK3{
    public:
        CGUIButtonGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIButtonGTK3();
        
        virtual std::string GetLabel() const;
        virtual void SetLabel(const std::string &label);
};

class CGUIComboBoxGTK3 : public virtual CGUIComboBox, public CGUIContainerGTK3{
    protected:
        int DItemCount;
        GtkCellRenderer *DCellRenderer;
    public:
        CGUIComboBoxGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIComboBoxGTK3();

        virtual void SetFontFamily(const std::string &family);
        
        virtual int ItemCount() const;
        virtual std::string GetItem(int item) const;
        virtual void SetItem(int item, const std::string &str);
        virtual void AppendItem(const std::string &str);
        virtual void ClearItems();

        virtual int GetActiveItem() const;
        virtual void SetActiveItem(int item);
};

class CGUIToggleButtonGTK3 : public virtual CGUIToggleButton, public CGUIButtonGTK3{
    public:
        CGUIToggleButtonGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIToggleButtonGTK3();

        virtual bool GetActive() const;
        virtual void SetActive(bool active);
};

class CGUIGridGTK3 : public virtual CGUIGrid, public CGUIContainerGTK3{
    public:
        CGUIGridGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIGridGTK3();
        
        virtual unsigned int GetRowSpacing() const override;
        virtual void SetRowSpacing(unsigned int spacing) override;
        virtual unsigned int GetColumnSpacing() const override;
        virtual void SetColumnSpacing(unsigned int spacing) override;
        void Attach(std::shared_ptr<CGUIWidget> widget, int left, int top, int width, int height) override;
};

class CGUIMenuShellGTK3 : public virtual CGUIMenuShell, public CGUIContainerGTK3{
    public:
        CGUIMenuShellGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuShellGTK3();
        
        void Append(std::shared_ptr<CGUIWidget> widget) override;
};

class CGUIMenuGTK3 : public virtual CGUIMenu, public CGUIMenuShellGTK3{
    public:
        CGUIMenuGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuGTK3();
        
};

class CGUIMenuBarGTK3 : public virtual CGUIMenuBar, public CGUIMenuShellGTK3{
    public:
        CGUIMenuBarGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuBarGTK3();
        
};

class CGUIMenuItemGTK3 : public virtual CGUIMenuItem, public CGUIContainerGTK3{
    public:
        CGUIMenuItemGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuItemGTK3();
        
        std::shared_ptr<CGUILabel> GetLabel() override;
        void SetSubMenu(std::shared_ptr<CGUIWidget> widget) override;
};

class CGUIScrollBarGTK3 : public virtual CGUIScrollBar, public CGUIWidgetGTK3{
    public:
        CGUIScrollBarGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIScrollBarGTK3();
        
        double GetValue() override;
        void SetValue(double val) override;
        void SetLower(double lower) override;
        void SetUpper(double upper)  override;
        void SetStepIncrement(double inc) override;
        void SetPageIncrement(double inc) override;
        void SetPageSize(double size) override;
};

class CGUIWindowGTK3 : public virtual CGUIWindow, public CGUIContainerGTK3{
    public:
        CGUIWindowGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIWindowGTK3();
        
        void SetMinSize(int width, int height) override;
        void SetMaxSize(int width, int height) override;
        void SetMinSize(std::shared_ptr<CGUIWidget> widget, int width, int height) override;
        void SetMaxSize(std::shared_ptr<CGUIWidget> widget, int width, int height) override;
        void Resize(int width, int height) override;
        void SetTitle(const std::string &title) override;
        void Close() override;
};

class CGUIFileFilterGTK3 : public virtual CGUIFileFilter{
    friend class CGUIFileChooserDialogGTK3;
    protected:
        GtkFileFilter *DFilter;
        
    public:
        CGUIFileFilterGTK3(GtkFileFilter *filter, bool reference = false);
        virtual ~CGUIFileFilterGTK3();
                
        void AddPattern(const std::string &pattern) override;
};

class CGUIFileChooserDialogGTK3 : public virtual CGUIFileChooserDialog, public CGUIWindowGTK3{
    public:
        CGUIFileChooserDialogGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIFileChooserDialogGTK3();
        
        void SetFilter(std::shared_ptr<CGUIFileFilter> filter) override;
        std::string GetCurrentFolder() override;
        void SetCurrentFolder(const std::string &folder) override;
        std::string GetFilename() override;
        
        int Run() override;
};

#endif

