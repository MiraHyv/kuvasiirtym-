#include "kello.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

//Extern-muuttujat
bool KELLO_VERBOSE = false;


kello::kello() {
    reset();
}


kello::kello(float fps) {
    reset();
    setPeriod_fps(fps);
}


void kello::setPeriod_ms(int period_ms) {
    typedef std::chrono::system_clock::duration sysduration;
    typedef std::chrono::milliseconds milliseconds;
    
    period = std::chrono::duration_cast<sysduration> (milliseconds(period_ms));
}


bool kello::setPeriod_fps(float fps) {
    if(fps <= 0)
        return false;
    float ms = 1000/fps;
    setPeriod_ms(ms);
    return true;
}


void printDuration(std::chrono::system_clock::duration d) {
    typedef std::chrono::milliseconds milliseconds;

    milliseconds duration_ms = std::chrono::duration_cast<milliseconds>(d);
    std::cout << duration_ms.count() << " ms\n";
}


//Sleep until the end of period is reached
//  Return false if period's end passed already and there was lag,
//  true otherwise.
//  The thread that calls this function sleeps.
bool kello::sleep() const {
    typedef std::chrono::system_clock::time_point timePoint;
    typedef std::chrono::system_clock::duration duration;
    
    static timePoint lastCall;
    timePoint now = std::chrono::system_clock::now();
    timePoint periodEnd = lastCall + period;
    
    //it's already past period's end! Don't wait
    if(periodEnd < now) {
        lastCall = now;
        if(KELLO_VERBOSE) {
            std::cout << "Lag! ";
            printDuration(now - periodEnd);
        }
        return false;
    }
    
    else {
        duration sleepTime = periodEnd - now;
        std::this_thread::sleep_until(periodEnd);
        lastCall = periodEnd;
        return true;
    }
}


float kello::getTime() const {
    //precision is milliseconds. TODO: better precision!
    //TODO: something wrong with the duration cast
    typedef std::chrono::system_clock::time_point timePoint;
    typedef std::chrono::system_clock::duration duration;
    typedef std::chrono::milliseconds milliseconds;
    timePoint now = std::chrono::system_clock::now();

    duration elapsedTime = now - startTime;
    duration elapsedTime_ms = std::chrono::duration_cast<milliseconds>(elapsedTime);
    
    return ((float)(elapsedTime_ms.count() ) / 1000000000.0f);
}


void kello::reset() {
    startTime = std::chrono::system_clock::now();
}
