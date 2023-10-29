#pragma once 

#include <TimerMs.h>
#include <Arduino.h>

class TAbstractTimer {
protected:
    TimerMs tmr = TimerMs(0, 0, 1);         /*(period, ms), (0 not started / 1 started), (mode: 0 period / 1 timer)*/
public: 
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool tick() = 0;

    virtual void setTime(uint32_t t) = 0;
    virtual uint32_t getTime() = 0;
};