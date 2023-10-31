#pragma once 
#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include <ArduinoJson.h>

extern DynamicJsonDocument status;


#define SERVICE_UUID                "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define RW_CHARACTERISTIC_UUID      "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define NF_CHARACTERISTIC_UUID      "1fd32b0a-aa51-4e49-92b2-9a8be97473c9"
#define BLE_NAME                    "Солнышко ОУФБ-04M"

class TBLEMessanger {
private: 
    DynamicJsonDocument request;

    BLEServer* pServer;
    BLEService* pService;
    BLECharacteristic* pRWCharacteristic;
    BLECharacteristic* pNotifyCharacteristic;
    BLEAdvertising* pAdvertising;

public:
    TBLEMessanger() : request(100) {}

    bool deviceConnected = false;
    bool available = false;

    void init();
    void notifyStateChanges();

    DynamicJsonDocument& getRequest();
};

extern TBLEMessanger BLEMessanger;