
#ifndef GUIEVENT_H
#define GUIEVENT_H
#include <memory>
#include <cstdint>
#include <unordered_set>

class CGUIWidget;
class CGraphicResourceContext;

using TGUICalldata = void *;

struct SGUIButtonEventType{
    enum EType : uint32_t{
        ButtonPress,
        DoubleButtonPress,
        TripleButtonPress,
        ButtonRelease
    };
    SGUIButtonEventType() = default;
    constexpr SGUIButtonEventType(EType type) : DType(type){};
    constexpr operator EType() const { return DType; };
    explicit operator bool() const = delete;

    bool IsButtonPress() const;
    bool IsDoubleButtonPress() const;
    bool IsTripleButtonPress() const;
    bool IsButtonRelease() const;
    
    private:
        EType DType;

};

struct SGUIModifierType{
    enum EType : uint32_t{
        Shift,
        Lock,
        Control,
        Alt,
        Button1,
        Button2,
        Button3
    };
    
    SGUIModifierType() = default;
    SGUIModifierType(const SGUIModifierType &type) = default;
    SGUIModifierType &operator=(const SGUIModifierType &type) = default;
    constexpr SGUIModifierType(uint32_t state) : DState(state){};
    bool ModifierIsSet(EType val) const;
    void SetModifier(EType val);  
    void ClearModifier(EType val);
    std::unordered_set<EType> AsSet() const;
    private:
        uint32_t DState;
};
  
struct SGUIKeyType{
    enum EValue : uint32_t{
        LeftShift,
        RightShift,
        LeftControl,
        RightControl,
        LeftAlt,
        RightAlt,
        Escape,
        Space,
        Delete,
        Period,
        BackSpace,
        Minus,
        Plus,
        Equal,
        Comma,
        Colon,
        SemiColon,
        Slash,
        QuestionMark,
        Exclamation,
        AtSymbol,
        Ampersand,
        Pound,
        Dollar,
        Percent,
        Caret,
        UpArrow,
        DownArrow,
        LeftArrow,
        RightArrow,
        Key0,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,
        KeyA,
        KeyB,
        KeyC,
        KeyD,
        KeyE,
        KeyF,
        KeyG,
        KeyH,
        KeyI,
        KeyJ,
        KeyK,
        KeyL,
        KeyM,
        KeyN,
        KeyO,
        KeyP,
        KeyQ,
        KeyR,
        KeyS,
        KeyT,
        KeyU,
        KeyV,
        KeyW,
        KeyX,
        KeyY,
        KeyZ,
        Keya,
        Keyb,
        Keyc,
        Keyd,
        Keye,
        Keyf,
        Keyg,
        Keyh,
        Keyi,
        Keyj,
        Keyk,
        Keyl,
        Keym,
        Keyn,
        Keyo,
        Keyp,
        Keyq,
        Keyr,
        Keys,
        Keyt,
        Keyu,
        Keyv,
        Keyw,
        Keyx,
        Keyy,
        Keyz
    };
    SGUIKeyType() = default;
    constexpr SGUIKeyType(EValue value) : DValue(value){};
    constexpr operator EValue() const { return DValue; };
    explicit operator bool() const = delete;

    void SetKey(EValue val);
    bool IsAlpha() const;
    bool IsAlphaNumeric() const;
    bool IsDigit() const;
    bool IsUpper() const;
    bool IsLower() const;
    static EValue ASCIIToValue(char ascii);
    private:
        EValue DValue;
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

enum class EGUIEvent{Expose, Motion, ButtonPress, ButtonRelease, KeyPress, KeyRelease, Scroll, Configure, Property};

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

