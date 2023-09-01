#include "GUIEventGTK3.h"
#include <glib.h>
#include <gdk/gdk.h>
#include <glib/gi18n.h>




const std::unordered_map<uint32_t, SGUIButtonEventType::EType> SGUIButtonEventTypeGTK3::DFromGTK3Mapping = {{GDK_BUTTON_PRESS,SGUIButtonEventType::ButtonPress},
                                                                                                {GDK_2BUTTON_PRESS,SGUIButtonEventType::DoubleButtonPress},
                                                                                                {GDK_3BUTTON_PRESS,SGUIButtonEventType::TripleButtonPress},
                                                                                                {GDK_BUTTON_RELEASE,SGUIButtonEventType::ButtonRelease},
                                                                                                };

const std::unordered_map<SGUIButtonEventType::EType, uint32_t> SGUIButtonEventTypeGTK3::DToGTK3Mapping = {{SGUIButtonEventType::ButtonPress, GDK_BUTTON_PRESS},
                                                                                                {SGUIButtonEventType::DoubleButtonPress, GDK_2BUTTON_PRESS},
                                                                                                {SGUIButtonEventType::TripleButtonPress, GDK_3BUTTON_PRESS},
                                                                                                {SGUIButtonEventType::ButtonRelease, GDK_BUTTON_RELEASE},
                                                                                                };


SGUIButtonEventType SGUIButtonEventTypeGTK3::TranslateFromGTK3Type(uint32_t gtk3type){
    auto Search = DFromGTK3Mapping.find(gtk3type);
    if(Search != DFromGTK3Mapping.end()){
        return Search->second;
    }
    return SGUIButtonEventType();
}

uint32_t SGUIButtonEventTypeGTK3::TranslateToGTK3Type(SGUIButtonEventType guitype){
    auto Search = DToGTK3Mapping.find(guitype);
    if(Search != DToGTK3Mapping.end()){
        return Search->second;
    }
    return -1;
}

const std::unordered_map<uint32_t, SGUIModifierType::EType> SGUIModifierTypeGTK3::DFromGTK3Mapping = {{GDK_SHIFT_MASK, SGUIModifierType::Shift},
                                                                                                    {GDK_LOCK_MASK, SGUIModifierType::Lock},
                                                                                                    {GDK_CONTROL_MASK, SGUIModifierType::Control},
                                                                                                    {GDK_MOD1_MASK, SGUIModifierType::Button1},
                                                                                                    {GDK_BUTTON2_MASK, SGUIModifierType::Button2},
                                                                                                    {GDK_BUTTON3_MASK, SGUIModifierType::Button3}
                                                                                                    };
const std::unordered_map<SGUIModifierType::EType, uint32_t> SGUIModifierTypeGTK3::DToGTK3Mapping = {{SGUIModifierType::Shift, GDK_SHIFT_MASK},
                                                                                                    {SGUIModifierType::Lock, GDK_LOCK_MASK},
                                                                                                    {SGUIModifierType::Control, GDK_CONTROL_MASK},
                                                                                                    {SGUIModifierType::Button1, GDK_MOD1_MASK},
                                                                                                    {SGUIModifierType::Button2, GDK_BUTTON2_MASK},
                                                                                                    {SGUIModifierType::Button3, GDK_BUTTON3_MASK}
                                                                                                    };

SGUIModifierType SGUIModifierTypeGTK3::TranslateFromGTK3Type(uint32_t gtk3type){
    SGUIModifierType ReturnType;
    uint32_t Index = 0;
    while(gtk3type){
        if(gtk3type & 0x1){
            auto Search = DFromGTK3Mapping.find(1<<Index);
            if(Search != DFromGTK3Mapping.end()){
                ReturnType.SetModifier(Search->second);
            }
        }
        Index++;
        gtk3type >>= 1;
    }
    return ReturnType;
}

uint32_t SGUIModifierTypeGTK3::TranslateToGTK3Type(SGUIModifierType guitype){
    uint32_t ReturnModifier = 0;
    for(auto Modifier : guitype.AsSet()){
        auto Search = DToGTK3Mapping.find(Modifier);
        if(Search != DToGTK3Mapping.end()){
            ReturnModifier |= Search->second;
        }
    }
    return ReturnModifier;
}

const std::unordered_map<uint32_t, SGUIKeyType::EValue> SGUIKeyTypeGTK3::DFromGTK3Mapping = {{GDK_KEY_Shift_L, SGUIKeyType::LeftShift},
                                                                                            {GDK_KEY_Shift_R, SGUIKeyType::RightShift},
                                                                                            {GDK_KEY_Control_L, SGUIKeyType::LeftControl},
                                                                                            {GDK_KEY_Control_R, SGUIKeyType::RightControl},
                                                                                            {GDK_KEY_Alt_L, SGUIKeyType::LeftAlt},
                                                                                            {GDK_KEY_Alt_R, SGUIKeyType::RightAlt},
                                                                                            {GDK_KEY_Escape, SGUIKeyType::Escape},
                                                                                            {GDK_KEY_space, SGUIKeyType::Space},
                                                                                            {GDK_KEY_Delete, SGUIKeyType::Delete},
                                                                                            {GDK_KEY_period, SGUIKeyType::Period},
                                                                                            {GDK_KEY_BackSpace, SGUIKeyType::BackSpace},
                                                                                            {GDK_KEY_minus, SGUIKeyType::Minus},
                                                                                            {GDK_KEY_plus, SGUIKeyType::Plus},
                                                                                            {GDK_KEY_equal, SGUIKeyType::Equal},
                                                                                            {GDK_KEY_comma, SGUIKeyType::Comma},
                                                                                            {GDK_KEY_colon, SGUIKeyType::Colon},
                                                                                            {GDK_KEY_semicolon, SGUIKeyType::SemiColon},
                                                                                            {GDK_KEY_slash, SGUIKeyType::Slash},
                                                                                            {GDK_KEY_question, SGUIKeyType::QuestionMark},
                                                                                            {GDK_KEY_exclam, SGUIKeyType::Exclamation},
                                                                                            {GDK_KEY_at, SGUIKeyType::AtSymbol},
                                                                                            {GDK_KEY_ampersand, SGUIKeyType::Ampersand},
                                                                                            {GDK_KEY_numbersign, SGUIKeyType::Pound},
                                                                                            {GDK_KEY_dollar, SGUIKeyType::Dollar},
                                                                                            {GDK_KEY_percent, SGUIKeyType::Percent},
                                                                                            {GDK_KEY_caret, SGUIKeyType::Caret},
                                                                                            {GDK_KEY_Up, SGUIKeyType::UpArrow},
                                                                                            {GDK_KEY_Down, SGUIKeyType::DownArrow},
                                                                                            {GDK_KEY_Left, SGUIKeyType::LeftArrow},
                                                                                            {GDK_KEY_Right, SGUIKeyType::RightArrow},
                                                                                            {GDK_KEY_0, SGUIKeyType::Key0},
                                                                                            {GDK_KEY_1, SGUIKeyType::Key1},
                                                                                            {GDK_KEY_2, SGUIKeyType::Key2},
                                                                                            {GDK_KEY_3, SGUIKeyType::Key3},
                                                                                            {GDK_KEY_4, SGUIKeyType::Key4},
                                                                                            {GDK_KEY_5, SGUIKeyType::Key5},
                                                                                            {GDK_KEY_6, SGUIKeyType::Key6},
                                                                                            {GDK_KEY_7, SGUIKeyType::Key7},
                                                                                            {GDK_KEY_8, SGUIKeyType::Key8},
                                                                                            {GDK_KEY_9, SGUIKeyType::Key9},
                                                                                            {GDK_KEY_A, SGUIKeyType::KeyA},
                                                                                            {GDK_KEY_B, SGUIKeyType::KeyB},
                                                                                            {GDK_KEY_C, SGUIKeyType::KeyC},
                                                                                            {GDK_KEY_D, SGUIKeyType::KeyD},
                                                                                            {GDK_KEY_E, SGUIKeyType::KeyE},
                                                                                            {GDK_KEY_F, SGUIKeyType::KeyF},
                                                                                            {GDK_KEY_G, SGUIKeyType::KeyG},
                                                                                            {GDK_KEY_H, SGUIKeyType::KeyH},
                                                                                            {GDK_KEY_I, SGUIKeyType::KeyI},
                                                                                            {GDK_KEY_J, SGUIKeyType::KeyJ},
                                                                                            {GDK_KEY_K, SGUIKeyType::KeyK},
                                                                                            {GDK_KEY_L, SGUIKeyType::KeyL},
                                                                                            {GDK_KEY_M, SGUIKeyType::KeyM},
                                                                                            {GDK_KEY_N, SGUIKeyType::KeyN},
                                                                                            {GDK_KEY_O, SGUIKeyType::KeyO},
                                                                                            {GDK_KEY_P, SGUIKeyType::KeyP},
                                                                                            {GDK_KEY_Q, SGUIKeyType::KeyQ},
                                                                                            {GDK_KEY_R, SGUIKeyType::KeyR},
                                                                                            {GDK_KEY_S, SGUIKeyType::KeyS},
                                                                                            {GDK_KEY_T, SGUIKeyType::KeyT},
                                                                                            {GDK_KEY_U, SGUIKeyType::KeyU},
                                                                                            {GDK_KEY_V, SGUIKeyType::KeyV},
                                                                                            {GDK_KEY_W, SGUIKeyType::KeyW},
                                                                                            {GDK_KEY_X, SGUIKeyType::KeyX},
                                                                                            {GDK_KEY_Y, SGUIKeyType::KeyY},
                                                                                            {GDK_KEY_Z, SGUIKeyType::KeyZ},
                                                                                            {GDK_KEY_a, SGUIKeyType::Keya},
                                                                                            {GDK_KEY_b, SGUIKeyType::Keyb},
                                                                                            {GDK_KEY_c, SGUIKeyType::Keyc},
                                                                                            {GDK_KEY_d, SGUIKeyType::Keyd},
                                                                                            {GDK_KEY_e, SGUIKeyType::Keye},
                                                                                            {GDK_KEY_f, SGUIKeyType::Keyf},
                                                                                            {GDK_KEY_g, SGUIKeyType::Keyg},
                                                                                            {GDK_KEY_h, SGUIKeyType::Keyh},
                                                                                            {GDK_KEY_i, SGUIKeyType::Keyi},
                                                                                            {GDK_KEY_j, SGUIKeyType::Keyj},
                                                                                            {GDK_KEY_k, SGUIKeyType::Keyk},
                                                                                            {GDK_KEY_l, SGUIKeyType::Keyl},
                                                                                            {GDK_KEY_m, SGUIKeyType::Keym},
                                                                                            {GDK_KEY_n, SGUIKeyType::Keyn},
                                                                                            {GDK_KEY_o, SGUIKeyType::Keyo},
                                                                                            {GDK_KEY_p, SGUIKeyType::Keyp},
                                                                                            {GDK_KEY_q, SGUIKeyType::Keyq},
                                                                                            {GDK_KEY_r, SGUIKeyType::Keyr},
                                                                                            {GDK_KEY_s, SGUIKeyType::Keys},
                                                                                            {GDK_KEY_t, SGUIKeyType::Keyt},
                                                                                            {GDK_KEY_u, SGUIKeyType::Keyu},
                                                                                            {GDK_KEY_v, SGUIKeyType::Keyv},
                                                                                            {GDK_KEY_w, SGUIKeyType::Keyw},
                                                                                            {GDK_KEY_x, SGUIKeyType::Keyx},
                                                                                            {GDK_KEY_y, SGUIKeyType::Keyy},
                                                                                            {GDK_KEY_z, SGUIKeyType::Keyz}};

const std::unordered_map<SGUIKeyType::EValue, uint32_t> SGUIKeyTypeGTK3::DToGTK3Mapping = {{SGUIKeyType::LeftShift, GDK_KEY_Shift_L},
                                                                                            {SGUIKeyType::RightShift, GDK_KEY_Shift_R},
                                                                                            {SGUIKeyType::LeftControl, GDK_KEY_Control_L},
                                                                                            {SGUIKeyType::RightControl, GDK_KEY_Control_R},
                                                                                            {SGUIKeyType::LeftAlt, GDK_KEY_Alt_L},
                                                                                            {SGUIKeyType::RightAlt, GDK_KEY_Alt_R},
                                                                                            {SGUIKeyType::Escape, GDK_KEY_Escape},
                                                                                            {SGUIKeyType::Space, GDK_KEY_space},
                                                                                            {SGUIKeyType::Delete, GDK_KEY_Delete},
                                                                                            {SGUIKeyType::Period, GDK_KEY_period},
                                                                                            {SGUIKeyType::BackSpace, GDK_KEY_BackSpace},
                                                                                            {SGUIKeyType::Minus, GDK_KEY_minus},
                                                                                            {SGUIKeyType::Plus, GDK_KEY_plus},
                                                                                            {SGUIKeyType::Equal, GDK_KEY_equal},
                                                                                            {SGUIKeyType::Comma, GDK_KEY_comma},
                                                                                            {SGUIKeyType::Colon, GDK_KEY_colon},
                                                                                            {SGUIKeyType::SemiColon, GDK_KEY_semicolon},
                                                                                            {SGUIKeyType::Slash, GDK_KEY_slash},
                                                                                            {SGUIKeyType::QuestionMark, GDK_KEY_question},
                                                                                            {SGUIKeyType::Exclamation, GDK_KEY_exclam},
                                                                                            {SGUIKeyType::AtSymbol, GDK_KEY_at},
                                                                                            {SGUIKeyType::Ampersand, GDK_KEY_ampersand},
                                                                                            {SGUIKeyType::Pound, GDK_KEY_numbersign},
                                                                                            {SGUIKeyType::Dollar, GDK_KEY_dollar},
                                                                                            {SGUIKeyType::Percent, GDK_KEY_percent},
                                                                                            {SGUIKeyType::Caret, GDK_KEY_caret},
                                                                                            {SGUIKeyType::UpArrow, GDK_KEY_Up},
                                                                                            {SGUIKeyType::DownArrow, GDK_KEY_Down},
                                                                                            {SGUIKeyType::LeftArrow, GDK_KEY_Left},
                                                                                            {SGUIKeyType::RightArrow, GDK_KEY_Right},
                                                                                            {SGUIKeyType::Key0, GDK_KEY_0},
                                                                                            {SGUIKeyType::Key1, GDK_KEY_1},
                                                                                            {SGUIKeyType::Key2, GDK_KEY_2},
                                                                                            {SGUIKeyType::Key3, GDK_KEY_3},
                                                                                            {SGUIKeyType::Key4, GDK_KEY_4},
                                                                                            {SGUIKeyType::Key5, GDK_KEY_5},
                                                                                            {SGUIKeyType::Key6, GDK_KEY_6},
                                                                                            {SGUIKeyType::Key7, GDK_KEY_7},
                                                                                            {SGUIKeyType::Key8, GDK_KEY_8},
                                                                                            {SGUIKeyType::Key9, GDK_KEY_9},
                                                                                            {SGUIKeyType::KeyA, GDK_KEY_A},
                                                                                            {SGUIKeyType::KeyB, GDK_KEY_B},
                                                                                            {SGUIKeyType::KeyC, GDK_KEY_C},
                                                                                            {SGUIKeyType::KeyD, GDK_KEY_D},
                                                                                            {SGUIKeyType::KeyE, GDK_KEY_E},
                                                                                            {SGUIKeyType::KeyF, GDK_KEY_F},
                                                                                            {SGUIKeyType::KeyG, GDK_KEY_G},
                                                                                            {SGUIKeyType::KeyH, GDK_KEY_H},
                                                                                            {SGUIKeyType::KeyI, GDK_KEY_I},
                                                                                            {SGUIKeyType::KeyJ, GDK_KEY_J},
                                                                                            {SGUIKeyType::KeyK, GDK_KEY_K},
                                                                                            {SGUIKeyType::KeyL, GDK_KEY_L},
                                                                                            {SGUIKeyType::KeyM, GDK_KEY_M},
                                                                                            {SGUIKeyType::KeyN, GDK_KEY_N},
                                                                                            {SGUIKeyType::KeyO, GDK_KEY_O},
                                                                                            {SGUIKeyType::KeyP, GDK_KEY_P},
                                                                                            {SGUIKeyType::KeyQ, GDK_KEY_Q},
                                                                                            {SGUIKeyType::KeyR, GDK_KEY_R},
                                                                                            {SGUIKeyType::KeyS, GDK_KEY_S},
                                                                                            {SGUIKeyType::KeyT, GDK_KEY_T},
                                                                                            {SGUIKeyType::KeyU, GDK_KEY_U},
                                                                                            {SGUIKeyType::KeyV, GDK_KEY_V},
                                                                                            {SGUIKeyType::KeyW, GDK_KEY_W},
                                                                                            {SGUIKeyType::KeyX, GDK_KEY_X},
                                                                                            {SGUIKeyType::KeyY, GDK_KEY_Y},
                                                                                            {SGUIKeyType::KeyZ, GDK_KEY_Z},
                                                                                            {SGUIKeyType::Keya, GDK_KEY_a},
                                                                                            {SGUIKeyType::Keyb, GDK_KEY_b},
                                                                                            {SGUIKeyType::Keyc, GDK_KEY_c},
                                                                                            {SGUIKeyType::Keyd, GDK_KEY_d},
                                                                                            {SGUIKeyType::Keye, GDK_KEY_e},
                                                                                            {SGUIKeyType::Keyf, GDK_KEY_f},
                                                                                            {SGUIKeyType::Keyg, GDK_KEY_g},
                                                                                            {SGUIKeyType::Keyh, GDK_KEY_h},
                                                                                            {SGUIKeyType::Keyi, GDK_KEY_i},
                                                                                            {SGUIKeyType::Keyj, GDK_KEY_j},
                                                                                            {SGUIKeyType::Keyk, GDK_KEY_k},
                                                                                            {SGUIKeyType::Keyl, GDK_KEY_l},
                                                                                            {SGUIKeyType::Keym, GDK_KEY_m},
                                                                                            {SGUIKeyType::Keyn, GDK_KEY_n},
                                                                                            {SGUIKeyType::Keyo, GDK_KEY_o},
                                                                                            {SGUIKeyType::Keyp, GDK_KEY_p},
                                                                                            {SGUIKeyType::Keyq, GDK_KEY_q},
                                                                                            {SGUIKeyType::Keyr, GDK_KEY_r},
                                                                                            {SGUIKeyType::Keys, GDK_KEY_s},
                                                                                            {SGUIKeyType::Keyt, GDK_KEY_t},
                                                                                            {SGUIKeyType::Keyu, GDK_KEY_u},
                                                                                            {SGUIKeyType::Keyv, GDK_KEY_v},
                                                                                            {SGUIKeyType::Keyw, GDK_KEY_w},
                                                                                            {SGUIKeyType::Keyx, GDK_KEY_x},
                                                                                            {SGUIKeyType::Keyy, GDK_KEY_y},
                                                                                            {SGUIKeyType::Keyz, GDK_KEY_z}};

SGUIKeyType SGUIKeyTypeGTK3::TranslateFromGTK3Type(uint32_t gtk3type){
    auto Search = DFromGTK3Mapping.find(gtk3type);
    if(Search != DFromGTK3Mapping.end()){
        return Search->second;
    }
    return SGUIKeyType();
}

uint32_t SGUIKeyTypeGTK3::TranslateToGTK3Type(SGUIKeyType guitype){
    auto Search = DToGTK3Mapping.find(guitype);
    if(Search != DToGTK3Mapping.end()){
        return Search->second;
    }
    return -1;
}
