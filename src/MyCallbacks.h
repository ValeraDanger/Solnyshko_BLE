#pragma once

#include "TBLEMessanger.h"
#include "TPreheater.h"
#include "TTimer.h"

extern DynamicJsonDocument status;
extern TaskHandle_t proccessCommandTaskHandler;

class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
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

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        BLEMessanger.deviceConnected = true;
        Serial.println("Device connected!");
    }

    void onDisconnect(BLEServer* pServer) {
        BLEMessanger.deviceConnected = false;
        Serial.println("Device disconnected!");
        pServer->getAdvertising()->start();
    }
};