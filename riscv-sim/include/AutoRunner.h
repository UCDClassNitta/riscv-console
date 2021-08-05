#ifndef AUTO_RUNNER_H
#define AUTO_RUNNER_H

#define RAPIDJSON_HAS_STDSTRING 1
#define RAPIDJSON_HAS_CXX11_RVALUE_REFS 1
#include "rapidjson/document.h"
#include "RISCVConsole.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <memory>

class CAutoRunner {
    protected:
        std::string DInputJSONPath;
        std::string DOutputJSONPath;

        rapidjson::Document DInputJSONDocument; 
        rapidjson::Document DOutputJSONDocument;
        rapidjson::Value DOutputJSONObjectArray;

        std::shared_ptr<CRISCVConsole> DRISCVConsole;

        bool LoadInputJSONDocument();

        void ParseArguments(int &argc, char *argv[]);
        void ParseInitData();
        void ParseCommandData();
        void SendCommand(uint32_t cycle, uint32_t nextCycle, const std::string &type, const std::string &data);

        void OutputJSONFile();

        bool InsertFW(const std::string &data);
        bool InsertCR(const std::string &data);
        bool RemoveCR();

        bool PressDirection(const std::string &type);
        bool ReleaseDirection(const std::string &type);
        bool PressButton(const std::string &type);
        bool ReleaseButton(const std::string &type);
        bool PressCommand();
        
        bool IsDirectionButton(const std::string &type);
        bool IsDirectionReleaseButton(const std::string &type);
        bool IsNumberButton(const std::string &type);
        bool IsNumberReleaseButton(const std::string &type);

        std::map<std::string, std::string> OutputRegs();
        std::map<std::string, std::string> OutputCSRs();
        std::map<std::string, std::string> OutputMem(const std::string &data);

        bool DoStep();
        bool DoRun();
        bool DoStop();
        bool DoPowerOn();
        bool DoPowerOff();
        bool DoCycleSteps(uint32_t cycle, uint32_t nextCycle);

        uint32_t GetAddressHex(const std::string &str_addr);
        rapidjson::Value FormatOutputMap(std::map<std::string, std::string> map, rapidjson::Document::AllocatorType &allocator);
        std::string FormatHex32Bit(uint32_t val);
        
    public:
        static const std::string INIT_STRING;
        static const std::string TIMER_US_STRING;
        static const std::string VIDEO_MS_STRING;
        static const std::string CPU_FREQ_STRING;

        static const std::string COMMANDS_STRING;
        static const std::string CYCLE_STRING;
        static const std::string TYPE_STRING;
        static const std::string DATA_STRING;

        static const std::string DIRECTION_UP_STRING;
        static const std::string DIRECTION_DOWN_STRING;
        static const std::string DIRECTION_LEFT_STRING;
        static const std::string DIRECTION_RIGHT_STRING;

        static const std::string BUTTON_1_STRING;
        static const std::string BUTTON_2_STRING;
        static const std::string BUTTON_3_STRING;
        static const std::string BUTTON_4_STRING;

        static const std::string DIRECTION_UP_RELEASE_STRING;
        static const std::string DIRECTION_DOWN_RELEASE_STRING;
        static const std::string DIRECTION_LEFT_RELEASE_STRING;
        static const std::string DIRECTION_RIGHT_RELEASE_STRING;

        static const std::string BUTTON_1_RELEASE_STRING;
        static const std::string BUTTON_2_RELEASE_STRING;
        static const std::string BUTTON_3_RELEASE_STRING;
        static const std::string BUTTON_4_RELEASE_STRING;

        static const std::string INSERT_FW_STRING;
        static const std::string INSERT_CR_STRING;
        static const std::string REMOVE_CR_STRING;

        static const std::string CMD_BUTTON_STRING;
        
        static const std::string OUTPUT_REG_STRING;
        static const std::string OUTPUT_CSR_STRING;
        static const std::string OUTPUT_MEM_STRING;
        static const std::string OUTPUTS_STRING;
        
        static const std::string REGS_STRING;
        static const std::string CSRS_STRING;
        static const std::string MEM_STRING;

        explicit CAutoRunner(int argc, char *argv[]);

};


#endif