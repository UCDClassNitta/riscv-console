
#ifndef GUIWIDGET_H
#define GUIWIDGET_H
#include "GUIEvent.h"
#include "GUICursor.h"
#include "GraphicSurface.h"
#include <string>

class CGUIWidget{
    public:
        virtual void Show() = 0;
        virtual void ShowAll() = 0;
        
        virtual void Hide() = 0;
        
        virtual int AllocatedWidth() = 0;
        virtual int AllocatedHeight() = 0;
        virtual void SetSizeRequest(int width, int height) = 0;

        virtual void SetVerticalExpand(bool exp) = 0;
        virtual void SetHorizontalExpand(bool exp) = 0;

        virtual void Invalidate() = 0;
        
        virtual void SetCursor(std::shared_ptr< CGUICursor > cursor) = 0;

        virtual void SetTooltipText(const std::string &tip) = 0;
        virtual void SetTooltipMarkup(const std::string &markup) = 0;
        
        virtual std::shared_ptr<CGraphicSurface> CreateSimilarSurface(int width, int height) = 0;
        virtual std::shared_ptr<CGraphicResourceContext> CreateResourceContext() = 0;
        
        virtual void EnableEvent(EGUIEvent event) = 0;
        virtual void DisableEvent(EGUIEvent event) = 0;
        
        virtual void SetActivateEventCallback(TGUICalldata calldata, TGUIActivateEventCallback callback) = 0;
        virtual void SetButtonPressEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback) = 0;
        virtual void SetButtonReleaseEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback) = 0;
        virtual void SetDeleteEventCallback(TGUICalldata calldata, TGUIDeleteEventCallback callback) = 0;
        virtual void SetDestroyEventCallback(TGUICalldata calldata, TGUIDestroyEventCallback callback) = 0;
        virtual void SetMotionEventCallback(TGUICalldata calldata, TGUIMotionEventCallback callback) = 0;
        virtual void SetKeyPressEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback) = 0;
        virtual void SetKeyReleaseEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback) = 0;
        virtual void SetExposeEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback) = 0;
        virtual void SetConfigureEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback) = 0;
        virtual void SetDrawEventCallback(TGUICalldata calldata, TGUIDrawEventCallback callback) = 0;
        virtual void SetToggledEventCallback(TGUICalldata calldata, TGUIToggledEventCallback callback) = 0;
        virtual void SetChangedEventCallback(TGUICalldata calldata, TGUIChangedEventCallback callback) = 0;
        virtual void SetValueChangedEventCallback(TGUICalldata calldata, TGUIValueChangedEventCallback callback) = 0;
        virtual void SetScrollEventCallback(TGUICalldata calldata, TGUIScrollEventCallback callback) = 0;
};

#endif

