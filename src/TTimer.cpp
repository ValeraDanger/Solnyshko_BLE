#include "TTimer.h"
#include <ArduinoJson.h>
#include "TPreheater.h"
#include "LampStates.h"
#include "TRelay.h"

extern DynamicJsonDocument status;

void TTimer::start() {
    //Preheater.stop();
    Relay.turnOn();
    this->tmr.start();
    Serial.println("Timer started");
}

void TTimer::stop() {
    Relay.turnOff();
    this->tmr.stop();
    this->setTime(0);
    status["timer"]["cycles"] = 0;
    status["timer"]["cycle_time"] = 0;
    Serial.println("Timer stoped");
}

bool TTimer::tick() {
    return this->tmr.tick();
}

void TTimer::setTime(uint32_t t) {
    this->tmr.setTime(t);         
    status["timer"]["time_left"] = t;
}

uint32_t TTimer::getTime() {
    return this->tmr.timeLeft();
}

void TTimer::pause() {
    this->tmr.stop();
    status["timer"]["time_left"] = this->tmr.timeLeft();
    Serial.println("Timer paused");
}

void TTimer::resume() {
    this->tmr.resume();
    status["timer"]["time_left"] = this->tmr.timeLeft();
    Serial.println("Timer resumed");
}

TTimer Timer;
