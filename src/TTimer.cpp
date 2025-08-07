#include "TTimer.h"
#include <ArduinoJson.h>
#include "TPreheater.h"
#include "LampStates.h"
#include "TRelay.h"
#include "TBuzzer.h"

extern DynamicJsonDocument status;
extern TaskHandle_t inactivityTimerControllerTaskHandler;
extern void inactivityTimerController(void* pvParams);

void TTimer::inactivity_start() {
    this->inactivity_tmr.setTime(INACTIVITY_TIME);
    this->inactivity_tmr.start();

    xTaskCreate(
        inactivityTimerController,   /* Task method pointer*/
        "Inactivity timer task",          /* Task name*/
        10000,                       /* Stack deepth*/
        NULL,               /* Pointer on class object itself */
        2,                                  /* Priority*/
        &inactivityTimerControllerTaskHandler                       /* Task handle*/
    ); 

    Serial.println("Inactivity timer started");
}

void TTimer::inactivity_stop() {
    this->inactivity_tmr.stop();
    this->inactivity_tmr.setTime(0);
    Serial.println("Inactivity timer stoped");

    if (inactivityTimerControllerTaskHandler != NULL) {
        vTaskDelete(inactivityTimerControllerTaskHandler);
        Serial.println(inactivityTimerControllerTaskHandler == NULL);
        inactivityTimerControllerTaskHandler = NULL;
        Serial.println(inactivityTimerControllerTaskHandler == NULL);
    }
}

void TTimer::start()
{
    //Preheater.stop();
    Relay.turnOn();
    this->tmr.start();
    //this->inactivity_stop();
    Serial.println("Timer started");
}

void TTimer::stop() {
    Relay.turnOff();
    this->tmr.stop();
    this->setTime(0);
    status["timer"]["cycles"] = 0;
    status["timer"]["cycle_time"] = 0;
    //this->inactivity_stop();
    Serial.println("Timer stoped");
}

bool TTimer::tick() {
    return this->tmr.tick();
}

void TTimer::setTime(uint32_t t) {
    this->tmr.setTime(t);         
    status["timer"]["time_left"] = t;
    //this->inactivity_stop();
}

uint32_t TTimer::getTime() {
    return this->tmr.timeLeft();
}

void TTimer::pause() {
    this->tmr.stop();
    Relay.turnOff();
    status["timer"]["time_left"] = this->tmr.timeLeft();
    Buzzer.pauseBeep();
    //this->inactivity_start();
    Serial.println("Timer paused");
}

void TTimer::resume() {
    this->tmr.resume();
    status["timer"]["time_left"] = this->tmr.timeLeft();
    Relay.turnOn();
    //this->inactivity_stop();
    Serial.println("Timer resumed");
}

TTimer Timer;
