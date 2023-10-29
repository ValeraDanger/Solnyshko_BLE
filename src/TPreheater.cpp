#include "TPreheater.h"
#include <ArduinoJson.h>
#include "TTimer.h"
#include "LampStates.h"
#include "TRelay.h"

extern DynamicJsonDocument status;

void TPreheater::start() {
    Timer.stop();
    this->tmr.setTime(PREHEAT_TIME); 
    Relay.turnOn();
    this->tmr.start();
    Serial.println("Preheat started");
}

void TPreheater::stop() {
    this->tmr.stop();
    this->setTime(0);
    Serial.println("Preheat stoped");
}

bool TPreheater::tick() {
    return this->tmr.tick();
}

void TPreheater::setTime(uint32_t t) {
    this->tmr.setTime(t * 1000);            //cast to milliseconds
    status["preheat"]["time_left"] = t;
}

uint32_t TPreheater::getTime() {
    return this->tmr.timeLeft();
}

TPreheater Preheater;