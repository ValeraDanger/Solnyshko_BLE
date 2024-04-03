#pragma once 

#include "TAbstractTimer.h"

#define PREHEAT_TIME 60000

class TPreheater : public TAbstractTimer{
public:
    void start() override;
    void stop() override;
    bool tick() override;

    void setTime(uint32_t t) override;
    uint32_t getTime() override;
};

extern TPreheater Preheater;