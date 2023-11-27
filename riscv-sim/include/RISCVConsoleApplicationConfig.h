#ifndef RISCVCONSOLEAPPLICATIONCONFIG_H
#define RISCVCONSOLEAPPLICATIONCONFIG_H

#include "DataSource.h"
#include <string>
#include <cstdint>
#include <unordered_map>

class CRISCVConsoleApplicationConfiguration{
    public:
        enum class EParameter{  UpKey,DownKey,LeftKey,RightKey,Button1Key,Button2Key,Button3Key,Button4Key,
                                ZoomInKey,ZoomOutKey,SnapshotKey,
                                WidgetSpacing,InstructionLines,MemoryLines,
                                ScreenTimer,TimerUS,CPUFrequency,VideoModel,PointerDepth
                            };
    protected:
        std::unordered_map< std::string, EParameter > DParameterNames;
        std::unordered_map< EParameter, std::string > DParameters;

    public:
        CRISCVConsoleApplicationConfiguration();
        ~CRISCVConsoleApplicationConfiguration();
        
        bool Load(std::shared_ptr< CDataSource> src);

        int64_t GetIntegerParameter(EParameter param) const;
        std::string GetStringParameter(EParameter param) const;

};



#endif