#include "GUITextTagGTK3.h"
#include <algorithm>

CGUITextTagGTK3::CGUITextTagGTK3(GtkTextTag *texttag){
    DTextTag = texttag;
}

CGUITextTagGTK3::~CGUITextTagGTK3(){
}

void CGUITextTagGTK3::TransformValue(GValue *value, const std::string &str_value){
    try{
        // https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case
        auto LowerCase = str_value;
        std::transform(LowerCase.begin(), LowerCase.end(), LowerCase.begin(), [](unsigned char c){ return std::tolower(c); });
        switch(G_VALUE_TYPE(value)){
            case G_TYPE_BOOLEAN:    if(LowerCase == "true"){
                                        g_value_set_boolean(value,true);
                                    }
                                    else if(LowerCase == "false"){
                                        g_value_set_boolean(value,false);
                                    }
                                    break;
            case G_TYPE_CHAR:       g_value_set_schar(value,std::stoi(str_value));
                                    break;
            case G_TYPE_INT:        g_value_set_int(value,std::stoi(str_value));
                                    break;
            case G_TYPE_LONG:       g_value_set_long(value,std::stol(str_value));
                                    break;
            case G_TYPE_INT64:      g_value_set_int64(value,std::stoll(str_value));
                                    break;
            case G_TYPE_UCHAR:      g_value_set_uchar(value,std::stoul(str_value));
                                    break;
            case G_TYPE_UINT:       g_value_set_uint(value,std::stoul(str_value));
                                    break;
            case G_TYPE_ULONG:      g_value_set_ulong(value,std::stoul(str_value));
                                    break;
            case G_TYPE_UINT64:     g_value_set_uint64(value,std::stoull(str_value));
                                    break;
            case G_TYPE_FLOAT:      g_value_set_float(value,std::stod(str_value));
                                    break;
            case G_TYPE_DOUBLE:     g_value_set_double(value,std::stod(str_value));
                                    break;
            case G_TYPE_STRING:     g_value_set_string(value,str_value.c_str());
            default:                break;
        }
    }
    catch(const std::exception &e){

    }
}

int CGUITextTagGTK3::GetPriority(){
    return gtk_text_tag_get_priority(DTextTag);
}

void CGUITextTagGTK3::SetPriority(int prio){
    return gtk_text_tag_set_priority(DTextTag, prio);
}
