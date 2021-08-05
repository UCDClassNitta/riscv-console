#include "AutoRecorder.h"
#include "AutoRunner.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/memorystream.h"
#include "rapidjson/filewritestream.h"
#include "RISCVConsoleApplication.h"
#include "RISCVConsole.h"
#include "FileDataSink.h"
#include "FileDataSource.h"
#include "Path.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <map>

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

CAutoRecorder::CAutoRecorder(uint32_t timerus, uint32_t videoms, uint32_t cpufreq){
    DTimerDelayUS = timerus;
    DVideoDelayMS = videoms;
    DDebugCPUFreq = cpufreq;
       
    DOutputJSONDocument.SetObject();
    rapidjson::Value TempValue(rapidjson::kArrayType);
    DOutputJSONObjectArray = TempValue;
}

bool CAutoRecorder::AddFWEvent(const std::string &data){
    rapidjson::Document::AllocatorType &Allocator = DOutputJSONDocument.GetAllocator();
    rapidjson::Value Root(rapidjson::kObjectType);
    rapidjson::Value Data(rapidjson::kStringType);
    Data.SetString(data.c_str(), Allocator);

    DFirmwareFilename = data;
    // Firware programming will always reset console.
    Root.AddMember(rapidjson::StringRef(CAutoRunner::CYCLE_STRING), 0, Allocator);  
    Root.AddMember(rapidjson::StringRef(CAutoRunner::TYPE_STRING), rapidjson::StringRef(CAutoRunner::INSERT_FW_STRING), Allocator);
    Root.AddMember(rapidjson::StringRef(CAutoRunner::DATA_STRING), Data, Allocator);

    DOutputJSONObjectArray.PushBack(Root, Allocator);

    return true;
}

bool CAutoRecorder::AddInsertCREvent(const std::string &data, uint64_t cycle){
    rapidjson::Document::AllocatorType &Allocator = DOutputJSONDocument.GetAllocator();
    rapidjson::Value Root(rapidjson::kObjectType);
    rapidjson::Value Data(rapidjson::kStringType);
    Data.SetString(data.c_str(), Allocator);

    DCartridgeFilename = data;
    Root.AddMember(rapidjson::StringRef(CAutoRunner::CYCLE_STRING), cycle, Allocator);  
    Root.AddMember(rapidjson::StringRef(CAutoRunner::TYPE_STRING), rapidjson::StringRef(CAutoRunner::INSERT_CR_STRING), Allocator);
    Root.AddMember(rapidjson::StringRef(CAutoRunner::DATA_STRING), Data, Allocator);

    DOutputJSONObjectArray.PushBack(Root, Allocator);

    return true;
}

bool CAutoRecorder::AddRemoveCREvent(uint64_t cycle){
    rapidjson::Document::AllocatorType &Allocator = DOutputJSONDocument.GetAllocator();
    rapidjson::Value Root(rapidjson::kObjectType);
    rapidjson::Value Data(rapidjson::kStringType);
    
    DCartridgeFilename.clear();
    Root.AddMember(rapidjson::StringRef(CAutoRunner::CYCLE_STRING), cycle, Allocator);  
    Root.AddMember(rapidjson::StringRef(CAutoRunner::TYPE_STRING), rapidjson::StringRef(CAutoRunner::REMOVE_CR_STRING), Allocator);
    Root.AddMember(rapidjson::StringRef(CAutoRunner::DATA_STRING), "", Allocator);

    DOutputJSONObjectArray.PushBack(Root, Allocator);

    return true;
}

bool CAutoRecorder::AddCommandPressEvent(uint64_t cycle){
    rapidjson::Document::AllocatorType &Allocator = DOutputJSONDocument.GetAllocator();
    rapidjson::Value Root(rapidjson::kObjectType);
    rapidjson::Value Type(rapidjson::kStringType);
    
    Root.AddMember(rapidjson::StringRef(CAutoRunner::CYCLE_STRING), cycle, Allocator);  
    Root.AddMember(rapidjson::StringRef(CAutoRunner::TYPE_STRING), rapidjson::StringRef(CAutoRunner::CMD_BUTTON_STRING), Allocator);
    Root.AddMember(rapidjson::StringRef(CAutoRunner::DATA_STRING), "", Allocator);

    DOutputJSONObjectArray.PushBack(Root, Allocator);

    return true;
}

bool CAutoRecorder::AddDirectionPressEvent(CRISCVConsole::EDirection direction, uint64_t cycle){
    rapidjson::Document::AllocatorType &Allocator = DOutputJSONDocument.GetAllocator();
    rapidjson::Value Root(rapidjson::kObjectType);
    rapidjson::Value Type(rapidjson::kStringType);
    auto Search = DDirectionPressMapping.find(direction);

    if(Search != DDirectionPressMapping.end()){
        DDepressedDiretions.insert(direction);
        Type.SetString(Search->second.c_str(), Allocator);

        Root.AddMember(rapidjson::StringRef(CAutoRunner::CYCLE_STRING), cycle, Allocator);  
        Root.AddMember(rapidjson::StringRef(CAutoRunner::TYPE_STRING), Type, Allocator);
        Root.AddMember(rapidjson::StringRef(CAutoRunner::DATA_STRING), "", Allocator);

        DOutputJSONObjectArray.PushBack(Root, Allocator);

        return true;
    }
    return false;
}

bool CAutoRecorder::AddDirectionReleaseEvent(CRISCVConsole::EDirection direction, uint64_t cycle){
    rapidjson::Document::AllocatorType &Allocator = DOutputJSONDocument.GetAllocator();
    rapidjson::Value Root(rapidjson::kObjectType);
    rapidjson::Value Type(rapidjson::kStringType);
    auto Search = DDirectionReleaseMapping.find(direction);

    if(Search != DDirectionReleaseMapping.end()){
        DDepressedDiretions.erase(direction);
        Type.SetString(Search->second.c_str(), Allocator);

        Root.AddMember(rapidjson::StringRef(CAutoRunner::CYCLE_STRING), cycle, Allocator);  
        Root.AddMember(rapidjson::StringRef(CAutoRunner::TYPE_STRING), Type, Allocator);
        Root.AddMember(rapidjson::StringRef(CAutoRunner::DATA_STRING), "", Allocator);

        DOutputJSONObjectArray.PushBack(Root, Allocator);

        return true;
    }
    return false;
}

bool CAutoRecorder::AddButtonPressEvent(CRISCVConsole::EButtonNumber button, uint64_t cycle){
    rapidjson::Document::AllocatorType &Allocator = DOutputJSONDocument.GetAllocator();
    rapidjson::Value Root(rapidjson::kObjectType);
    rapidjson::Value Type(rapidjson::kStringType);
    auto Search = DButtonPressMapping.find(button);

    if(Search != DButtonPressMapping.end()){
        DDepressedButtons.insert(button);
        Type.SetString(Search->second.c_str(), Allocator);

        Root.AddMember(rapidjson::StringRef(CAutoRunner::CYCLE_STRING), cycle, Allocator);  
        Root.AddMember(rapidjson::StringRef(CAutoRunner::TYPE_STRING), Type, Allocator);
        Root.AddMember(rapidjson::StringRef(CAutoRunner::DATA_STRING), "", Allocator);

        DOutputJSONObjectArray.PushBack(Root, Allocator);

        return true;
    }
    return false;
}

bool CAutoRecorder::AddButtonReleaseEvent(CRISCVConsole::EButtonNumber button, uint64_t cycle){
    rapidjson::Document::AllocatorType &Allocator = DOutputJSONDocument.GetAllocator();
    rapidjson::Value Root(rapidjson::kObjectType);
    rapidjson::Value Type(rapidjson::kStringType);
    auto Search = DButtonReleaseMapping.find(button);

    if(Search != DButtonReleaseMapping.end()){
        DDepressedButtons.erase(button);
        Type.SetString(Search->second.c_str(), Allocator);

        Root.AddMember(rapidjson::StringRef(CAutoRunner::CYCLE_STRING), cycle, Allocator);  
        Root.AddMember(rapidjson::StringRef(CAutoRunner::TYPE_STRING), Type, Allocator);
        Root.AddMember(rapidjson::StringRef(CAutoRunner::DATA_STRING), "", Allocator);

        DOutputJSONObjectArray.PushBack(Root, Allocator);

        return true;
    }
    return false;
}

void CAutoRecorder::OutputJSONFile(const std::string &path){
    rapidjson::Document::AllocatorType &OutputAllocator = DOutputJSONDocument.GetAllocator();
    rapidjson::Value Root(rapidjson::kObjectType);
    Root.AddMember(rapidjson::StringRef(CAutoRunner::TIMER_US_STRING), DTimerDelayUS, OutputAllocator);  
    Root.AddMember(rapidjson::StringRef(CAutoRunner::VIDEO_MS_STRING), DVideoDelayMS, OutputAllocator);
    Root.AddMember(rapidjson::StringRef(CAutoRunner::CPU_FREQ_STRING), DDebugCPUFreq, OutputAllocator);
    DOutputJSONDocument.AddMember(rapidjson::StringRef(CAutoRunner::INIT_STRING), Root, OutputAllocator);
    DOutputJSONDocument.AddMember(rapidjson::StringRef(CAutoRunner::COMMANDS_STRING), DOutputJSONObjectArray, OutputAllocator);

    FILE* f = fopen(path.c_str(), "w");
	char WriteBuffer[65535];
	rapidjson::FileWriteStream os(f, WriteBuffer, sizeof(WriteBuffer));

	rapidjson::PrettyWriter<rapidjson::FileWriteStream> Writer(os);
	DOutputJSONDocument.Accept(Writer);
	fclose(f);
}

void CAutoRecorder::ResetRecord(){
    DOutputJSONDocument.SetObject();
    DOutputJSONObjectArray.Clear();
    if(!DFirmwareFilename.empty()){
        AddFWEvent(DFirmwareFilename);
    }
    if(!DCartridgeFilename.empty()){
        AddInsertCREvent(DCartridgeFilename,0);
    }
    for(auto Direction : DDepressedDiretions){
        AddDirectionPressEvent(Direction,0);
    }
    for(auto Button : DDepressedButtons){
        AddButtonPressEvent(Button,0);
    }
}