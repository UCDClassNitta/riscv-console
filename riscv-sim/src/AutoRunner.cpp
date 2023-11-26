#include "AutoRunner.h"
#include "RISCVConsole.h"
#include "VideoControllerAllocator.h"
#include "FileDataSink.h"
#include "FileDataSource.h"
#include "Path.h"
#include "JSONWriter.h"
#include "RISCVSYSTypeInstruction.h"

#include <iomanip>
#include <sstream>

const std::string CAutoRunner::INIT_STRING = "Init";
const std::string CAutoRunner::TIMER_US_STRING = "TimerUS";
const std::string CAutoRunner::VIDEO_MS_STRING = "VideoMS";
const std::string CAutoRunner::CPU_FREQ_STRING = "CPUFreq";
const std::string CAutoRunner::VIDEO_MODEL_STRING = "VideoModel";
const std::string CAutoRunner::COMMANDS_STRING = "Commands";
const std::string CAutoRunner::CYCLE_STRING = "Cycle";
const std::string CAutoRunner::TYPE_STRING = "Type";
const std::string CAutoRunner::DATA_STRING = "Data";
const std::string CAutoRunner::DIRECTION_UP_STRING = "DirectionUp";
const std::string CAutoRunner::DIRECTION_DOWN_STRING = "DirectionDown";
const std::string CAutoRunner::DIRECTION_LEFT_STRING = "DirectionLeft";
const std::string CAutoRunner::DIRECTION_RIGHT_STRING = "DirectionRight";
const std::string CAutoRunner::BUTTON_1_STRING = "Button1";
const std::string CAutoRunner::BUTTON_2_STRING = "Button2";
const std::string CAutoRunner::BUTTON_3_STRING = "Button3";
const std::string CAutoRunner::BUTTON_4_STRING = "Button4";
const std::string CAutoRunner::DIRECTION_UP_RELEASE_STRING = "DirectionUpRelease";
const std::string CAutoRunner::DIRECTION_DOWN_RELEASE_STRING = "DirectionDownRelease";
const std::string CAutoRunner::DIRECTION_LEFT_RELEASE_STRING = "DirectionLeftRelease";
const std::string CAutoRunner::DIRECTION_RIGHT_RELEASE_STRING = "DirectionRightRelease";
const std::string CAutoRunner::BUTTON_1_RELEASE_STRING = "Button1Release";
const std::string CAutoRunner::BUTTON_2_RELEASE_STRING = "Button2Release";
const std::string CAutoRunner::BUTTON_3_RELEASE_STRING = "Button3Release";
const std::string CAutoRunner::BUTTON_4_RELEASE_STRING = "Button4Release";
const std::string CAutoRunner::INSERT_FW_STRING = "InsertFW";
const std::string CAutoRunner::INSERT_CR_STRING = "InsertCart";
const std::string CAutoRunner::REMOVE_CR_STRING = "RemoveCart";
const std::string CAutoRunner::CMD_BUTTON_STRING = "CMDButton"; 
const std::string CAutoRunner::OUTPUT_REG_STRING = "OutputRegs";
const std::string CAutoRunner::OUTPUT_CSR_STRING = "OutputCSRs";
const std::string CAutoRunner::OUTPUT_MEM_STRING = "OutputMem";
const std::string CAutoRunner::OUTPUTS_STRING = "Outputs";
const std::string CAutoRunner::REGS_STRING = "Regs";
const std::string CAutoRunner::CSRS_STRING = "CSRs";
const std::string CAutoRunner::MEM_STRING = "Mem";


CAutoRunner::CAutoRunner(int argc, char *argv[], std::shared_ptr<CGraphicFactory> graphicfactory){
    DInputJSONPath = "./input.json";
    DOutputJSONPath = "./output.json";
    DReturnCode = 0;
    DOutputArray = std::make_shared< CJSONElement >();
    DOutputArray->Type(CJSONElement::EType::Array);
    ParseArguments(argc, argv);
    if(!LoadInputJSONDocument()){
        DReturnCode = -1;
        return;
    }

    if(!ParseInitData(graphicfactory)){
        DReturnCode = -1;
        return;
    }
    if(!ParseCommandData()){
        DReturnCode = -1;
        return;
    }
    if(!OutputJSONFile()){
        DReturnCode = -1;
        return;
    }
}

void CAutoRunner::ParseArguments(int &argc, char *argv[]){
    if(argc >= 2){
        DInputJSONPath = argv[1];
        if(argc >= 3){
            DOutputJSONPath = argv[2];
        }
    }
}

bool CAutoRunner::LoadInputJSONDocument(){
    auto InputFile = std::make_shared< CFileDataSource >(DInputJSONPath);
    CJSONParser Parser;

    if(!Parser.Parse(InputFile)){
        return false;
    }
    DTimerUS = 0;
    DVideoMS = 0;
    DCPUFreq = 0;
    DVideoModel = 1;
    auto RootNode = Parser.Root();
    if(CJSONElement::EType::Object != RootNode->Type()){
        return false;
    }
    DInitNode = RootNode->Member(INIT_STRING);
    DCommandsNode  = RootNode->Member(COMMANDS_STRING);
    if(!DCommandsNode){
        return false;
    }
    return true;
}

bool CAutoRunner::LoadInitParameter(uint32_t &param, const std::string &paramname){
    if(DInitNode){
        uint64_t Temp;
        if(!ParseIntMember(Temp, DInitNode, paramname)){
            return false;
        }
        param = Temp;
    }
    return true;
}

bool CAutoRunner::ParseInitData(std::shared_ptr<CGraphicFactory> graphicfactory){
    if(DInitNode){
        if(!LoadInitParameter(DTimerUS, TIMER_US_STRING)){
            return false;
        }
        if(!LoadInitParameter(DVideoMS, VIDEO_MS_STRING)){
            return false;
        }
        if(!LoadInitParameter(DCPUFreq, CPU_FREQ_STRING)){
            return false;
        }
        if(!LoadInitParameter(DVideoModel, VIDEO_MODEL_STRING)){
            return false;
        }
    }
    auto VideoController = CVideoControllerAllocator::Allocate(DVideoModel,graphicfactory);
    DRISCVConsole = std::make_shared<CRISCVConsole>(DTimerUS, DVideoMS, DCPUFreq, VideoController);
    DRISCVConsole->SetDebugMode(true);
    return true;
}

bool CAutoRunner::ParseIntMember(uint64_t &num, std::shared_ptr< const CJSONElement > node, const std::string &name){
    if(node){
        auto TempNode = node->Member(name);
        if(TempNode){
            size_t Position;
            if(CJSONElement::EType::Number != TempNode->Type()){
                return false;
            }
            try{
                num = std::stoull(TempNode->PrimitiveText(),&Position);
            }
            catch(std::invalid_argument &e){
                printf("std::stoull(%s) for %s\n",TempNode->PrimitiveText().c_str(),name.c_str());
                throw e;
            }
            
            if(Position != TempNode->PrimitiveText().length()){
                return false;
            }
            return true;
        }
    }
    return false;
}

bool CAutoRunner::ParseStringMember(std::string &str, std::shared_ptr< const CJSONElement > node, const std::string &name){
    if(node){
        auto TempNode = node->Member(name);
        if(TempNode){
            if(CJSONElement::EType::String != TempNode->Type()){
                return false;
            }
            str = TempNode->PrimitiveText();
            return true;
        }
    }
    return false;
}

bool CAutoRunner::ParseCommand(SCommand &command, std::shared_ptr< const CJSONElement > node){
    if(!node || (CJSONElement::EType::Object != node->Type())){
        return false;
    }
    if(!ParseIntMember(command.DCycle,node,CYCLE_STRING)){
        return false;
    }
    if(!ParseStringMember(command.DType,node,TYPE_STRING)){
        return false;
    }
    if(!ParseStringMember(command.DData,node,DATA_STRING)){
        return false;
    }
    return true;
}

bool CAutoRunner::ParseCommandData(){
    if(CJSONElement::EType::Array == DCommandsNode->Type()){
        if(!DCommandsNode->ChildCount()){
            return false;
        }
        SCommand CurrentCommand, NextCommand;
        for(size_t Index = 0; Index < DCommandsNode->ChildCount(); Index++){
            auto CommandNode = DCommandsNode->Index(Index);
            if(!ParseCommand(NextCommand,CommandNode)){
                return false;
            }
            if(Index){
                SendCommand(CurrentCommand.DCycle,NextCommand.DCycle,CurrentCommand.DType,CurrentCommand.DData);
            }
            CurrentCommand = NextCommand;
        }
        SendCommand(CurrentCommand.DCycle,NextCommand.DCycle,CurrentCommand.DType,CurrentCommand.DData);
        return true;
    }
    return false;
}

bool CAutoRunner::IsDirectionButton(const std::string &type){
    return (type == DIRECTION_UP_STRING || type == DIRECTION_DOWN_STRING || type == DIRECTION_LEFT_STRING || type == DIRECTION_RIGHT_STRING);
}

bool CAutoRunner::IsDirectionReleaseButton(const std::string &type){
    return (type == DIRECTION_UP_RELEASE_STRING || type == DIRECTION_DOWN_RELEASE_STRING || type == DIRECTION_LEFT_RELEASE_STRING || type == DIRECTION_RIGHT_RELEASE_STRING);
}

bool CAutoRunner::IsNumberButton(const std::string &type){
    return (type == BUTTON_1_STRING || type == BUTTON_2_STRING || type == BUTTON_3_STRING || type == BUTTON_4_STRING);
}

bool CAutoRunner::IsNumberReleaseButton(const std::string &type){
    return (type == BUTTON_1_RELEASE_STRING || type == BUTTON_2_RELEASE_STRING || type == BUTTON_3_RELEASE_STRING || type == BUTTON_4_RELEASE_STRING);
}
    
bool CAutoRunner::OutputJSONFile(){
    auto OutFile = std::make_shared< CFileDataSink >(DOutputJSONPath);
    CJSONWriter JSONWriter;

    return JSONWriter.Write(OutFile, DOutputArray);
}

void CAutoRunner::SendCommand(uint64_t cycle, uint64_t nextCycle, const std::string &type, const std::string &data){
    if(INSERT_FW_STRING == type){
        InsertFW(data);
        DoPowerOn();
    }
    else if(INSERT_CR_STRING == type){
        InsertCR(data);
    }
    else if(REMOVE_CR_STRING == type){
        RemoveCR();
    }
    else if(IsDirectionButton(type)){
        PressDirection(type);
    }
    else if(IsNumberButton(type)){
        PressButton(type);
    }
    else if(IsDirectionReleaseButton(type)){
        ReleaseDirection(type);
    }
    else if(IsNumberReleaseButton(type)){
        ReleaseButton(type);  
    }
    else if(CMD_BUTTON_STRING == type){
        PressCommand();
    }
    else if(OUTPUT_REG_STRING == type){
        QueueOutputMap(cycle, REGS_STRING, OutputRegs());
    }
    else if(OUTPUT_CSR_STRING == type){
        QueueOutputMap(cycle, CSRS_STRING, OutputCSRs());
    }
    else if(OUTPUT_MEM_STRING == type){
        QueueOutputMap(cycle, MEM_STRING, OutputMem(data));
    }
    DoCycleSteps(cycle, nextCycle);
}


bool CAutoRunner::InsertFW(const std::string &data){
    if(!data.empty()){
        std::string FWFileName = data;

        auto InFile = std::make_shared<CFileDataSource>(FWFileName);
        if(DRISCVConsole->ProgramFirmware(InFile)){
            return true;
        }
    }
    return false;
}

bool CAutoRunner::InsertCR(const std::string &data){
    if(!data.empty()){
        std::string CRFileName = data;

        auto InFile = std::make_shared<CFileDataSource>(CRFileName);
        if(DRISCVConsole->InsertCartridge(InFile)){
            return true;
        }
    }
    return false;
}

bool CAutoRunner::RemoveCR(){
    DRISCVConsole->RemoveCartridge();
    return true;
}

bool CAutoRunner::PressDirection(const std::string &type){
    if(type == DIRECTION_UP_STRING){
        DRISCVConsole->PressDirection(CRISCVConsole::EDirection::Up);
        return true;
    } 
    else if(type == DIRECTION_DOWN_STRING){
        DRISCVConsole->PressDirection(CRISCVConsole::EDirection::Down);
        return true;
    } 
    else if(type == DIRECTION_LEFT_STRING){
        DRISCVConsole->PressDirection(CRISCVConsole::EDirection::Left);
        return true;
    } 
    else if(type == DIRECTION_RIGHT_STRING){
        DRISCVConsole->PressDirection(CRISCVConsole::EDirection::Right);
        return true;
    }
    return false;
}

bool CAutoRunner::ReleaseDirection(const std::string &type){
    if(type == DIRECTION_UP_RELEASE_STRING){
        DRISCVConsole->ReleaseDirection(CRISCVConsole::EDirection::Up);
        return true;
    } else if(type == DIRECTION_DOWN_RELEASE_STRING){
        DRISCVConsole->ReleaseDirection(CRISCVConsole::EDirection::Down);
        return true;
    } else if(type == DIRECTION_LEFT_RELEASE_STRING){
        DRISCVConsole->ReleaseDirection(CRISCVConsole::EDirection::Left);
        return true;
    } else if(type == DIRECTION_RIGHT_RELEASE_STRING){
        DRISCVConsole->ReleaseDirection(CRISCVConsole::EDirection::Right);
        return true;
    }
    return false;
}

bool CAutoRunner::PressButton(const std::string &type){
    if(type == BUTTON_1_STRING){
        DRISCVConsole->PressButton(CRISCVConsole::EButtonNumber::Button1);
        return true;
    }
    else if(type == BUTTON_2_STRING){
        DRISCVConsole->PressButton(CRISCVConsole::EButtonNumber::Button2);
        return true;
    }
    else if(type == BUTTON_3_STRING){
        DRISCVConsole->PressButton(CRISCVConsole::EButtonNumber::Button3);
        return true;
    }
    else if(type == BUTTON_4_STRING){
        DRISCVConsole->PressButton(CRISCVConsole::EButtonNumber::Button4);
        return true;
    }
    return false;
}

bool CAutoRunner::ReleaseButton(const std::string &type){
    if(type == BUTTON_1_RELEASE_STRING){
        DRISCVConsole->ReleaseButton(CRISCVConsole::EButtonNumber::Button1);
        return true;
    }
    else if(type == BUTTON_2_RELEASE_STRING){
        DRISCVConsole->ReleaseButton(CRISCVConsole::EButtonNumber::Button2);
        return true;
    }
    else if(type == BUTTON_3_RELEASE_STRING){
        DRISCVConsole->ReleaseButton(CRISCVConsole::EButtonNumber::Button3);
        return true;
    }
    else if(type == BUTTON_4_RELEASE_STRING){
        DRISCVConsole->ReleaseButton(CRISCVConsole::EButtonNumber::Button4);
        return true;
    }
    return false;
}

bool CAutoRunner::PressCommand(){
    DRISCVConsole->PressCommand();
    return true;
}

std::map<std::string, std::string> CAutoRunner::OutputRegs(){
    std::map<std::string, std::string> RegOutputMap;

    for(size_t Index = 0; Index < CRISCVCPU::RegisterCount(); Index++){
        auto RegisterName = CRISCVCPU::CInstruction::RegisterName(Index);
        RegOutputMap[RegisterName] = FormatHex32Bit(DRISCVConsole->CPU()->Register(Index));
    }

    return RegOutputMap;
}

std::map<std::string, std::string> CAutoRunner::OutputCSRs(){
    std::map<std::string, std::string> CSROutputMap;
    size_t LineIndex = 0;
    for(uint32_t CSRAddr : DRISCVConsole->CPU()->ControlStatusRegisterKeys()){
        std::string CSRName = CRISCVSYSTypeInstruction::ControlStatusRegisterName(CSRAddr);
        CSROutputMap[CSRName] = FormatHex32Bit(DRISCVConsole->CPU()->ControlStatusRegister(CSRAddr));
        LineIndex++;
    }
    return CSROutputMap;
}

std::map<std::string, std::string> CAutoRunner::OutputMem(const std::string &data){
    // reference from: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    std::string Delim = "-";

    auto Start = 0U;
    auto End = data.find(Delim);
    std::string StartAdd;
    std::string EndAdd;
    
    while (End != std::string::npos)
    {
        StartAdd = data.substr(Start, End - Start);
        Start = End + Delim.length();
        End = data.find(Delim, Start);
    }

    EndAdd = data.substr(Start, End);

    uint32_t StartAddHex = GetAddressHex(StartAdd);
    uint32_t Bytes = GetAddressHex(EndAdd) - StartAddHex + 1;

    std::stringstream OutputStream;
    const uint8_t *Buffer = DRISCVConsole->Memory()->LoadData(StartAddHex, Bytes);

    for(uint32_t Index = 0; Index < Bytes; Index++){
        OutputStream<<std::setfill('0') << std::setw(2) << std::hex << uint32_t(Buffer[Index]);
    }
    return {{StartAdd, OutputStream.str()}};
}

void CAutoRunner::QueueOutputMap(uint64_t cycle, const std::string &key, const std::map<std::string, std::string> &outmap){
    auto OutNode = std::make_shared< CJSONElement >();
    OutNode->Type(CJSONElement::EType::Object);
    OutNode->AssignMember(CYCLE_STRING,cycle);
    auto TempObject = std::make_shared< CJSONElement >();
    TempObject->Type(CJSONElement::EType::Object);
    for(auto &KeyValue : outmap){
        TempObject->AssignMember(KeyValue.first,KeyValue.second);
    }
    OutNode->AssignMember(key,TempObject);
    DOutputArray->PushBack(OutNode);
}

bool CAutoRunner::DoStep(){
    DRISCVConsole->Step();
    return true;
}

bool CAutoRunner::DoRun(){
    DRISCVConsole->Run();
    return true;
}

bool CAutoRunner::DoStop(){
    DRISCVConsole->Stop();
    return true;
}

bool CAutoRunner::DoPowerOn(){
    DRISCVConsole->PowerOn();
    return true;
}

bool CAutoRunner::DoPowerOff(){
    DRISCVConsole->PowerOff();
    return true;
}

bool CAutoRunner::DoCycleSteps(uint64_t cycle, uint64_t nextCycle){
    for(uint64_t Index = 0; Index < (nextCycle - cycle); Index++){
        DoStep();
    }
    return true;
}   

uint32_t CAutoRunner::GetAddressHex(const std::string &str_addr){
    // reference from: https://stackoverflow.com/questions/1070497/c-convert-hex-string-to-signed-integer
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << str_addr;
    ss >> x;
    // output it as a signed type
    uint32_t addr = static_cast<int>(x);
    return addr;
}

std::string CAutoRunner::FormatHex32Bit(uint32_t val){
    std::stringstream Stream;
    Stream<<std::setfill('0') << std::setw(8) << std::hex << val;
    return Stream.str();
}