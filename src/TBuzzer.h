#pragma once 

#include <vector>

class TBuzzer {
private:
    int noteDurations=10;
    std::vector<int> melody;

public:
    TBuzzer();
    void pauseBeep();
};

extern TBuzzer Buzzer;