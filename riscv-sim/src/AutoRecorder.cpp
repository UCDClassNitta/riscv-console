#include "AutoRecorder.h"
#include "AutoRunner.h"
#include "FileDataSink.h"
#include "Path.h"
#include "JSONWriter.h"

std::unordered_map< CRISCVConsole::EDirection, std::string > CAutoRecorder::DDirectionPressMapping = {{CRISCVConsole::EDirection::Left,CAutoRunner::DIRECTION_LEFT_STRING},
                                                                                                        {CRISCVConsole::EDirection::Up,CAutoRunner::DIRECTION_UP_STRING},
                                                                                                        {CRISCVConsole::EDirection::Right,CAutoRunner::DIRECTION_RIGHT_STRING},
                                                                                                        {CRISCVConsole::EDirection::Down,CAutoRunner::DIRECTION_DOWN_STRING}};
std::unordered_map< CRISCVConsole::EDirection, std::string > CAutoRecorder::DDirectionReleaseMapping = {{CRISCVConsole::EDirection::Left,CAutoRunner::DIRECTION_LEFT_RELEASE_STRING},
                                                                                                        {CRISCVConsole::EDirection::Up,CAutoRunner::DIRECTION_UP_RELEASE_STRING},
                                                                                                        {CRISCVConsole::EDirection::Right,CAutoRunner::DIRECTION_RIGHT_RELEASE_STRING},
                                                                                                        {CRISCVConsole::EDirection::Down,CAutoRunner::DIRECTION_DOWN_RELEASE_STRING}};
std::unordered_map< CRISCVConsole::EButtonNumber, std::string > CAutoRecorder::DButtonPressMapping = {{CRISCVConsole::EButtonNumber::Button1,CAutoRunner::BUTTON_1_STRING},
                                                                                                        {CRISCVConsole::EButtonNumber::Button2,CAutoRunner::BUTTON_2_STRING},
                                                                                                        {CRISCVConsole::EButtonNumber::Button3,CAutoRunner::BUTTON_3_STRING},
                                                                                                        {CRISCVConsole::EButtonNumber::Button4,CAutoRunner::BUTTON_4_STRING}};
std::unordered_map< CRISCVConsole::EButtonNumber, std::string > CAutoRecorder::DButtonReleaseMapping = {{CRISCVConsole::EButtonNumber::Button1,CAutoRunner::BUTTON_1_RELEASE_STRING},
                                                                                                        {CRISCVConsole::EButtonNumber::Button2,CAutoRunner::BUTTON_2_RELEASE_STRING},
                                                                                                        {CRISCVConsole::EButtonNumber::Button3,CAutoRunner::BUTTON_3_RELEASE_STRING},
                                                                                                        {CRISCVConsole::EButtonNumber::Button4,CAutoRunner::BUTTON_4_RELEASE_STRING},};

CAutoRecorder::CAutoRecorder(uint32_t timerus, uint32_t videoms, uint32_t cpufreq, uint32_t videomodel){
    DTimerDelayUS = timerus;
    DVideoDelayMS = videoms;
    DDebugCPUFreq = cpufreq;
    DVideoModel = videomodel;
}

bool CAutoRecorder::AddFWEvent(const std::string &data){
    DFirmwareFilename = data;
    // Firware programming will always reset console.
    DRecordedEvents.push_back({.DCycle=0,.DType=CAutoRunner::INSERT_FW_STRING,.DData=data});
    return true;
}

bool CAutoRecorder::AddInsertCREvent(const std::string &data, uint64_t cycle){
    DCartridgeFilename = data;
    DRecordedEvents.push_back({.DCycle=cycle,.DType=CAutoRunner::INSERT_CR_STRING,.DData=data});
    return true;
}

bool CAutoRecorder::AddRemoveCREvent(uint64_t cycle){
    DCartridgeFilename.clear();
    DRecordedEvents.push_back({.DCycle=cycle,.DType=CAutoRunner::REMOVE_CR_STRING,.DData=std::string()});
    return true;
}

bool CAutoRecorder::AddCommandPressEvent(uint64_t cycle){
    DRecordedEvents.push_back({.DCycle=cycle,.DType=CAutoRunner::CMD_BUTTON_STRING,.DData=std::string()});
    return true;
}

bool CAutoRecorder::AddDirectionPressEvent(CRISCVConsole::EDirection direction, uint64_t cycle){
    auto Search = DDirectionPressMapping.find(direction);

    if(Search != DDirectionPressMapping.end()){
        DDepressedDirections.insert(direction);
        DRecordedEvents.push_back({.DCycle=cycle,.DType=Search->second.c_str(),.DData=std::string()});
        return true;
    }
    return false;
}

bool CAutoRecorder::AddDirectionReleaseEvent(CRISCVConsole::EDirection direction, uint64_t cycle){
    auto Search = DDirectionReleaseMapping.find(direction);

    if(Search != DDirectionReleaseMapping.end()){
        DDepressedDirections.erase(direction);
        DRecordedEvents.push_back({.DCycle=cycle,.DType=Search->second.c_str(),.DData=std::string()});
        return true;
    }
    return false;
}

bool CAutoRecorder::AddButtonPressEvent(CRISCVConsole::EButtonNumber button, uint64_t cycle){
    auto Search = DButtonPressMapping.find(button);

    if(Search != DButtonPressMapping.end()){
        DDepressedButtons.insert(button);
        DRecordedEvents.push_back({.DCycle=cycle,.DType=Search->second.c_str(),.DData=std::string()});
        return true;
    }
    return false;
}

bool CAutoRecorder::AddButtonReleaseEvent(CRISCVConsole::EButtonNumber button, uint64_t cycle){
    auto Search = DButtonReleaseMapping.find(button);

    if(Search != DButtonReleaseMapping.end()){
        DDepressedButtons.erase(button);
        DRecordedEvents.push_back({.DCycle=cycle,.DType=Search->second.c_str(),.DData=std::string()});
        return true;
    }
    return false;
}

void CAutoRecorder::OutputJSONFile(const std::string &path){
    std::shared_ptr< CJSONElement > Root = std::make_shared< CJSONElement >();
    std::shared_ptr< CJSONElement > Init = std::make_shared< CJSONElement >();
    std::shared_ptr< CJSONElement > Commands = std::make_shared< CJSONElement >();
    Root->Type(CJSONElement::EType::Object);
    Init->Type(CJSONElement::EType::Object);
    Init->AssignMember(CAutoRunner::TIMER_US_STRING, DTimerDelayUS);
    Init->AssignMember(CAutoRunner::VIDEO_MS_STRING, DVideoDelayMS);
    Init->AssignMember(CAutoRunner::CPU_FREQ_STRING, DDebugCPUFreq);
    Init->AssignMember(CAutoRunner::VIDEO_MODEL_STRING, DVideoModel);

    Commands->Type(CJSONElement::EType::Array);
    for(size_t Index = 0; Index < DRecordedEvents.size(); Index++){
        auto Event = DRecordedEvents[Index];
        auto TempCommand = std::make_shared< CJSONElement >();
        TempCommand->Type(CJSONElement::EType::Object);
        TempCommand->AssignMember(CAutoRunner::CYCLE_STRING, Event.DCycle);
        TempCommand->AssignMember(CAutoRunner::TYPE_STRING, Event.DType);
        TempCommand->AssignMember(CAutoRunner::DATA_STRING, Event.DData);
        Commands->PushBack(TempCommand);
    }
    Root->AssignMember(CAutoRunner::INIT_STRING,Init);
    Root->AssignMember(CAutoRunner::COMMANDS_STRING,Commands);

    auto OutFile = std::make_shared<CFileDataSink>(path);
    CJSONWriter JSONWriter;
    JSONWriter.Write(OutFile,Root);
}

void CAutoRecorder::ResetRecord(){
    DRecordedEvents.clear();
    if(!DFirmwareFilename.empty()){
        AddFWEvent(DFirmwareFilename);
    }
    if(!DCartridgeFilename.empty()){
        AddInsertCREvent(DCartridgeFilename,0);
    }
    for(auto Direction : DDepressedDirections){
        AddDirectionPressEvent(Direction,0);
    }
    for(auto Button : DDepressedButtons){
        AddButtonPressEvent(Button,0);
    }
}