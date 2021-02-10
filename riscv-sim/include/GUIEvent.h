
#ifndef GUIEVENT_H
#define GUIEVENT_H
#include <memory>
#include <cstdint>

class CGUIWidget;
class CGraphicResourceContext;

using TGUICalldata = void *;

struct SGUIButtonEventType{
    uint32_t DType;
    static const uint32_t DButtonPress;
    static const uint32_t DDoubleButtonPress;
    static const uint32_t DTripleButtonPress;
    static const uint32_t DButtonRelease;
    bool IsButtonPress() const;
    bool IsDoubleButtonPress() const;
    bool IsTripleButtonPress() const;
    bool IsButtonRelease() const;
};

struct SGUIModifierType{
    uint32_t DState; 
    static const uint32_t Shift;
    static const uint32_t Lock;
    static const uint32_t Control;
    static const uint32_t Alt;
    static const uint32_t Button1;
    static const uint32_t Button2;
    static const uint32_t Button3;
    bool ModifierIsSet(uint32_t val) const;
    void SetModifier(uint32_t val);  
    void ClearModifier(uint32_t val);
};
  
struct SGUIKeyType{
    uint32_t DValue;
    static const uint32_t LeftShift;
    static const uint32_t RightShift;
    static const uint32_t LeftControl;
    static const uint32_t RightControl;
    static const uint32_t LeftAlt;
    static const uint32_t RightAlt;
    static const uint32_t Escape;
    static const uint32_t Space;
    static const uint32_t Delete;
    static const uint32_t Period;
    static const uint32_t BackSpace;
    static const uint32_t UpArrow;
    static const uint32_t DownArrow;
    static const uint32_t LeftArrow;
    static const uint32_t RightArrow;
    static const uint32_t Key0;
    static const uint32_t Key1;
    static const uint32_t Key2;
    static const uint32_t Key3;
    static const uint32_t Key4;
    static const uint32_t Key5;
    static const uint32_t Key6;
    static const uint32_t Key7;
    static const uint32_t Key8;
    static const uint32_t Key9;
    static const uint32_t KeyA;
    static const uint32_t KeyB;
    static const uint32_t KeyC;
    static const uint32_t KeyD;
    static const uint32_t KeyE;
    static const uint32_t KeyF;
    static const uint32_t KeyG;
    static const uint32_t KeyH;
    static const uint32_t KeyI;
    static const uint32_t KeyJ;
    static const uint32_t KeyK;
    static const uint32_t KeyL;
    static const uint32_t KeyM;
    static const uint32_t KeyN;
    static const uint32_t KeyO;
    static const uint32_t KeyP;
    static const uint32_t KeyQ;
    static const uint32_t KeyR;
    static const uint32_t KeyS;
    static const uint32_t KeyT;
    static const uint32_t KeyU;
    static const uint32_t KeyV;
    static const uint32_t KeyW;
    static const uint32_t KeyX;
    static const uint32_t KeyY;
    static const uint32_t KeyZ;
    static const uint32_t Keya;
    static const uint32_t Keyb;
    static const uint32_t Keyc;
    static const uint32_t Keyd;
    static const uint32_t Keye;
    static const uint32_t Keyf;
    static const uint32_t Keyg;
    static const uint32_t Keyh;
    static const uint32_t Keyi;
    static const uint32_t Keyj;
    static const uint32_t Keyk;
    static const uint32_t Keyl;
    static const uint32_t Keym;
    static const uint32_t Keyn;
    static const uint32_t Keyo;
    static const uint32_t Keyp;
    static const uint32_t Keyq;
    static const uint32_t Keyr;
    static const uint32_t Keys;
    static const uint32_t Keyt;
    static const uint32_t Keyu;
    static const uint32_t Keyv;
    static const uint32_t Keyw;
    static const uint32_t Keyx;
    static const uint32_t Keyy;
    static const uint32_t Keyz;
    bool IsKey(uint32_t val) const;
    void SetKey(uint32_t val);
    bool IsAlpha() const;
    bool IsAlphaNumeric() const;
    bool IsDigit() const;
    bool IsUpper() const;
    bool IsLower() const;
    bool IsASCII() const;
};

struct SGUIButtonEvent{
    SGUIButtonEventType DType;
    double DWindowX;
    double DWindowY;
    SGUIModifierType DModifier;
    int DButton;
    double DScreenX;
    double DScreenY;
};

struct SGUIMotionEvent{
    double DWindowX;
    double DWindowY;          
    SGUIModifierType DModifier;
    double DScreenX;
    double DScreenY;
};

struct SGUIConfigureEvent{
    int DX;
    int DY;
    int DWidth;
    int DHeight;
};

struct SGUIKeyEvent{
    SGUIKeyType DValue;
    SGUIModifierType DModifier;
};

enum class EGUIScrollDirection{Up, Down, Left, Right, Smooth};

struct SGUIScrollEvent{
    EGUIScrollDirection DDirection;
    double DDeltaX;
    double DDeltaY;
};

enum class EGUIEvent{Expose, Motion, ButtonPress, ButtonRelease, KeyPress, KeyRelease, Scroll};

using TGUIApplicationCallback = void (*)(TGUICalldata data);
using TGUITimeoutCallback = bool (*)(TGUICalldata data);
using TGUIActivateEventCallback = void (*)(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
using TGUIDeleteEventCallback = bool (*)(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
using TGUIDestroyEventCallback = void (*)(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
using TGUIToggledEventCallback = bool (*)(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
using TGUIChangedEventCallback = bool (*)(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
using TGUIValueChangedEventCallback = bool (*)(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
using TGUIButtonEventCallback = bool (*)(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
using TGUIMotionEventCallback = bool (*)(std::shared_ptr<CGUIWidget> widget, SGUIMotionEvent &event, TGUICalldata data);
using TGUIConfigureEventCallback = bool (*)(std::shared_ptr<CGUIWidget> widget, SGUIConfigureEvent &event, TGUICalldata data);
using TGUIDrawEventCallback = bool (*)(std::shared_ptr<CGUIWidget> widget, std::shared_ptr<CGraphicResourceContext> rc, TGUICalldata data);
using TGUIKeyEventCallback = bool (*)(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event, TGUICalldata data);
using TGUIScrollEventCallback = bool (*)(std::shared_ptr<CGUIWidget> widget, SGUIScrollEvent &event, TGUICalldata data);


#endif

