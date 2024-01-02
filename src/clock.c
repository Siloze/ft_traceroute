#include "includes/traceroute.h"

size_t getInterval(){
	struct timeval tv;

	gettimeofday(&tv, NULL);
    size_t milliseconds = tv.tv_sec*1000LL + tv.tv_usec/1000;
	return milliseconds;
}

size_t * getRawClock(){
    static size_t clock = 0;

    return &clock;
}

size_t getClock(){
    size_t clock = *getRawClock();

    if (clock > 0) //is clock is running
        return getInterval() - clock;
    return (0);
}

void resetClock(){
    size_t *clock = getRawClock();

    *clock = 0;
}

void startClock(){
    size_t *clock = getRawClock();
    *clock = getInterval();
}

size_t stopClock(){    
    size_t finalTimer = getClock();
    resetClock();
    return finalTimer;
}
