#include "TRelay.h"
//#include "TimerClass.h"
#include <Arduino.h>
#include <ArduinoJson.h>

extern DynamicJsonDocument status;

void TRelay::init() {
    pinMode(RELAY_PIN, OUTPUT);
}	

void TRelay::turnOn() {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("\tOn");
}

void TRelay::turnOff() {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("\tOff");
}

TRelay Relay;