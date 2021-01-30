
#ifndef PERIODICTIMEOUT_H
#define PERIODICTIMEOUT_H

#include <sys/time.h>
#include <chrono>

class CPeriodicTimeout{
    protected:
        struct timeval DNextExpectedTimeout;
        std::chrono::high_resolution_clock::time_point DNextTimeoutPoint;
        int DTimeoutInterval;
        
    public:
        CPeriodicTimeout(int periodms);
  
        int MiliSecondPeriod() const{
            return DTimeoutInterval; 
        };
        
        int Frequency() const{
            return 1000 / DTimeoutInterval;  
        };
        
        int MiliSecondsUntilDeadline();

        void AcknowledgeDeadline();
};

#endif
