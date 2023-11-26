#ifndef AUTO_RECORDER_H
#define AUTO_RECORDER_H

#include "RISCVConsole.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

class CAutoRecorder {
    protected:
        struct SRecordedEvent{
            uint64_t DCycle;
            std::string DType;
            std::string DData;
        };

        uint32_t DTimerDelayUS;
        uint32_t DVideoDelayMS;
        uint32_t DDebugCPUFreq;
        uint32_t DVideoModel;

        std::vector< SRecordedEvent > DRecordedEvents;

        std::string DFirmwareFilename;
        std::string DCartridgeFilename;
        std::unordered_set< CRISCVConsole::EDirection > DDepressedDirections;
        std::unordered_set< CRISCVConsole::EButtonNumber > DDepressedButtons;

        static std::unordered_map< CRISCVConsole::EDirection, std::string > DDirectionPressMapping;
        static std::unordered_map< CRISCVConsole::EDirection, std::string > DDirectionReleaseMapping;
        static std::unordered_map< CRISCVConsole::EButtonNumber, std::string > DButtonPressMapping;
        static std::unordered_map< CRISCVConsole::EButtonNumber, std::string > DButtonReleaseMapping;

    public:
        CAutoRecorder(uint32_t timerus, uint32_t videoms, uint32_t cpufreq, uint32_t videomodel);

        bool AddFWEvent(const std::string &data);
        bool AddInsertCREvent(const std::string &data, uint64_t cycle);
        bool AddRemoveCREvent(uint64_t cycle);
        bool AddCommandPressEvent(uint64_t cycle);
        bool AddDirectionPressEvent(CRISCVConsole::EDirection direction, uint64_t cycle);
        bool AddDirectionReleaseEvent(CRISCVConsole::EDirection direction, uint64_t cycle);

        bool AddButtonPressEvent(CRISCVConsole::EButtonNumber button, uint64_t cycle);
        bool AddButtonReleaseEvent(CRISCVConsole::EButtonNumber button, uint64_t cycle);

        void OutputJSONFile(const std::string &path);

        void ResetRecord();
};

#endif