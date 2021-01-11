
#ifndef PERIODICTIMEOUT_H
#define PERIODICTIMEOUT_H

#include <sys/time.h>

class CPeriodicTimeout{
    protected:
        struct timeval DNextExpectedTimeout;
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
};

#endif
