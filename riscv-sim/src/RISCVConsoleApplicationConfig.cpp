#include "RISCVConsoleApplicationConfig.h"
#include "CommentSkipLineDataSource.h"
#include <cstdlib>
#include <sstream>
#include <cstdio>

CRISCVConsoleApplicationConfiguration::CRISCVConsoleApplicationConfiguration(){
    DParameterNames["UP_KEY"] = EParameter::UpKey;
    DParameterNames["DOWN_KEY"] = EParameter::DownKey;
    DParameterNames["LEFT_KEY"] = EParameter::LeftKey;
    DParameterNames["RIGHT_KEY"] = EParameter::RightKey;
    DParameterNames["BUTTON1_KEY"] = EParameter::Button1Key;
    DParameterNames["BUTTON2_KEY"] = EParameter::Button2Key;
    DParameterNames["BUTTON3_KEY"] = EParameter::Button3Key;
    DParameterNames["BUTTON4_KEY"] = EParameter::Button4Key;
    DParameterNames["WIDGET_SPACING"] = EParameter::WidgetSpacing;
    DParameterNames["INSTRUCTION_LINES"] = EParameter::InstructionLines;
    DParameterNames["MEMORY_LINES"] = EParameter::MemoryLines;

    DParameterNames["SCREEN_TIMERMS"] = EParameter::ScreenTimer;
    DParameterNames["TIMERUS"] = EParameter::TimerUS;   
    DParameterNames["CPU_FREQ"] = EParameter::CPUFrequency;   

    DParameters[EParameter::UpKey] = "w";
    DParameters[EParameter::DownKey] = "x";
    DParameters[EParameter::LeftKey] = "a";
    DParameters[EParameter::RightKey] = "d";

    DParameters[EParameter::Button1Key] = "u";
    DParameters[EParameter::Button2Key] = "i";
    DParameters[EParameter::Button3Key] = "j";
    DParameters[EParameter::Button4Key] = "k";

    DParameters[EParameter::WidgetSpacing] = "5";
    DParameters[EParameter::InstructionLines] = "14";
    DParameters[EParameter::MemoryLines] = "10";

    DParameters[EParameter::ScreenTimer] = "10";
    DParameters[EParameter::TimerUS] = "1000";
    DParameters[EParameter::CPUFrequency] = "10000000";
    
}

CRISCVConsoleApplicationConfiguration::~CRISCVConsoleApplicationConfiguration(){

}

bool CRISCVConsoleApplicationConfiguration::Load(std::shared_ptr< CDataSource> src){
    CCommentSkipLineDataSource InputSource(src,'#');
    std::string Line;
    while(InputSource.Read(Line)){
        if(Line.length()){
            std::stringstream Stream(Line);
            std::string Key, Value;
            Stream>>Key>>Value;
            auto Search = DParameterNames.find(Key);
            if((Search != DParameterNames.end())&&(Value.length())){
                DParameters[Search->second] = Value;
            }
        }
    }
    return true;
}

int64_t CRISCVConsoleApplicationConfiguration::GetIntegerParameter(EParameter param) const{
    auto Search = DParameters.find(param);
    if(Search == DParameters.end()){
        return -1;
    }
    return strtoll(Search->second.c_str(),NULL,0);
}

std::string CRISCVConsoleApplicationConfiguration::GetStringParameter(EParameter param) const{
    auto Search = DParameters.find(param);
    if(Search == DParameters.end()){
        return std::string();
    }
    return Search->second;
}
