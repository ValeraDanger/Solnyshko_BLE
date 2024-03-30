#pragma once 

#include "TAbstractTimer.h"

#define INACTIVITY_TIME 15000  //Max inactivity time, ms

class TTimer : public TAbstractTimer{
private:

public:
    TimerMs inactivity_tmr = TimerMs(INACTIVITY_TIME, 0, 1);  /*(period, ms), (0 not started / 1 started), (mode: 0 period / 1 timer)*/

    void start() override;
    void stop() override;
    bool tick() override;

    void setTime(uint32_t t) override;
    uint32_t getTime() override;

    void pause();
    void resume();

    void inactivity_start();
    void inactivity_stop();
};

extern TTimer Timer;