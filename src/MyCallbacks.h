#pragma once

#include "TBLEMessanger.h"
#include "TPreheater.h"
#include "TTimer.h"

extern DynamicJsonDocument status;
extern TaskHandle_t proccessCommandTaskHandler;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        deserializeJson(BLEMessanger.getRequest(), value);
        BLEMessanger.available = true;
        vTaskResume(proccessCommandTaskHandler);
    }

    void onRead(BLECharacteristic *pCharacteristic) {
        std::string value;

        status["preheat"]["time_left"] = Preheater.getTime();
        status["timer"]["time_left"] = Timer.getTime();

        serializeJson(status, value);
        pCharacteristic->setValue(value);
    }
};