#pragma once 

#include "TAbstractTimer.h"

class TTimer : public TAbstractTimer{
public:
    void start() override;
    void stop() override;
    bool tick() override;

    void setTime(uint32_t t) override;
    uint32_t getTime() override;

    void pause();
    void resume();
};

extern TTimer Timer;