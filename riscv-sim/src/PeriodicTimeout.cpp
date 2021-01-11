
#include "PeriodicTimeout.h"
#include <cstdint>
/*
int CTimeout::SecondsUntilDeadline(struct timeval deadline){
    struct timeval CurrentTime;
    
    gettimeofday(&CurrentTime, nullptr);
    
    return ((deadline.tv_sec * 1000 + deadline.tv_usec / 1000) - (CurrentTime.tv_sec * 1000 + CurrentTime.tv_usec / 1000)) / 1000;
};

int CTimeout::MiliSecondsUntilDeadline(struct timeval deadline){
    struct timeval CurrentTime;
    
    gettimeofday(&CurrentTime, nullptr);
    
    return ((deadline.tv_sec * 1000 + deadline.tv_usec / 1000) - (CurrentTime.tv_sec * 1000 + CurrentTime.tv_usec / 1000));
};
*/

CPeriodicTimeout::CPeriodicTimeout(int periodms){
    gettimeofday(&DNextExpectedTimeout, nullptr);
    if(0 >= periodms){
        DTimeoutInterval = 1000;        
    }
    else{
        DTimeoutInterval = periodms;    
    }
}

int CPeriodicTimeout::MiliSecondsUntilDeadline(){
    struct timeval CurrentTime;
    int64_t TimeDelta;
    
    gettimeofday(&CurrentTime, nullptr);
    TimeDelta = (DNextExpectedTimeout.tv_sec * 1000 + DNextExpectedTimeout.tv_usec / 1000) - (CurrentTime.tv_sec * 1000 + CurrentTime.tv_usec / 1000);
    while(0 >= TimeDelta){
        DNextExpectedTimeout.tv_usec += DTimeoutInterval * 1000;
        if(1000000 <= DNextExpectedTimeout.tv_usec){
            DNextExpectedTimeout.tv_usec %= 1000000;
            DNextExpectedTimeout.tv_sec++;
        }
        TimeDelta = (DNextExpectedTimeout.tv_sec * 1000 + DNextExpectedTimeout.tv_usec / 1000) - (CurrentTime.tv_sec * 1000 + CurrentTime.tv_usec / 1000);
    }
    return TimeDelta;
}
