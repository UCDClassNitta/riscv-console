#ifndef AUTO_RUNNER_H
#define AUTO_RUNNER_H

#include "RISCVConsole.h"
#include "JSONParser.h"

#include <string>


class CAutoRunner {
    protected:
        struct SCommand{
            uint64_t DCycle;
            std::string DType;
            std::string DData;
        };

        std::string DInputJSONPath;
        std::string DOutputJSONPath;

        std::shared_ptr<CRISCVConsole> DRISCVConsole;

        std::shared_ptr< const CJSONElement > DInitNode;
        std::shared_ptr< const CJSONElement > DCommandsNode;
        std::shared_ptr< CJSONElement > DOutputArray;

        uint32_t DTimerUS;
        uint32_t DVideoMS;
        uint32_t DCPUFreq;
        uint32_t DVideoModel;

        int DReturnCode;

        bool LoadInputJSONDocument();
        bool LoadInitParameter(uint32_t &param, const std::string &paramname);

        void ParseArguments(int &argc, char *argv[]);
        bool ParseInitData(std::shared_ptr<CGraphicFactory> graphicfactory);
        bool ParseIntMember(uint64_t &num, std::shared_ptr< const CJSONElement > node, const std::string &name);
        bool ParseStringMember(std::string &str, std::shared_ptr< const CJSONElement > node, const std::string &name);
        bool ParseCommand(SCommand &command, std::shared_ptr< const CJSONElement > node);
        bool ParseCommandData();
        void SendCommand(uint64_t cycle, uint64_t nextCycle, const std::string &type, const std::string &data);

        bool OutputJSONFile();

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
        void QueueOutputMap(uint64_t cycle, const std::string &key, const std::map<std::string, std::string> &outmap);

        bool DoStep();
        bool DoRun();
        bool DoStop();
        bool DoPowerOn();
        bool DoPowerOff();
        bool DoCycleSteps(uint64_t cycle, uint64_t nextCycle);

        uint32_t GetAddressHex(const std::string &str_addr);
        std::string FormatHex32Bit(uint32_t val);
        
    public:
        static const std::string INIT_STRING;
        static const std::string TIMER_US_STRING;
        static const std::string VIDEO_MS_STRING;
        static const std::string CPU_FREQ_STRING;
        static const std::string VIDEO_MODEL_STRING;

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

        explicit CAutoRunner(int argc, char *argv[], std::shared_ptr<CGraphicFactory> graphicfactory);
        int ReturnCode() const{
            return DReturnCode;
        };

};


#endif