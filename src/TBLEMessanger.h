#pragma once 
#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <ArduinoJson.h>

extern DynamicJsonDocument status;


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define BLE_NAME            "Солнышко ОУФБ-04M"

class TBLEMessanger {
private: 
    DynamicJsonDocument request;

    BLEServer* pServer;
    BLEService* pService;
    BLECharacteristic* pCharacteristic;
    BLEAdvertising* pAdvertising;

public:
    TBLEMessanger() : request(100) {}

    bool available = false;
    void init();

    DynamicJsonDocument& getRequest();
};

extern TBLEMessanger BLEMessanger;