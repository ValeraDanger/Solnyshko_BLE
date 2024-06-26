#include "TBLEMessanger.h"
#include "MyCallbacks.h"
#include <Arduino.h>


void TBLEMessanger::init() {

    BLEDevice::init(BLE_NAME);
    pServer = BLEDevice::createServer();

    pServer->setCallbacks(new MyServerCallbacks());

    pService = pServer->createService(SERVICE_UUID);


    pRWCharacteristic = pService->createCharacteristic(
                                            RW_CHARACTERISTIC_UUID,
                                            BLECharacteristic::PROPERTY_READ |
                                            BLECharacteristic::PROPERTY_WRITE
                                        );
    pRWCharacteristic->setCallbacks(new MyCharacteristicCallbacks());


    pRWCharacteristic = pService->createCharacteristic(
                                            VERSION_CHARACTERISTIC_UUID,
                                            BLECharacteristic::PROPERTY_READ
                                        );
    pRWCharacteristic->setCallbacks(new VersionCharacteristicCallbacks());


    pNotifyCharacteristic = pService->createCharacteristic(
                                            NF_CHARACTERISTIC_UUID,
                                            BLECharacteristic::PROPERTY_NOTIFY
                                        );
    pNotifyCharacteristic->addDescriptor(new BLE2902());

    pService->start();

    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    oAdvertisementData.setManufacturerData("SNSolnyshko OYFB-04M");         //Where 2 first bytes convert into Reserved manufactorer id;

    pAdvertising = pServer->getAdvertising();
    pAdvertising->setAdvertisementData(oAdvertisementData);
    pAdvertising->start();

    Serial.println("BLE server started!");
}

void TBLEMessanger::notifyStateChanges() {
    this->pNotifyCharacteristic->setValue("CHNG");  // 
    this->pNotifyCharacteristic->notify();
}

DynamicJsonDocument& TBLEMessanger::getRequest() {
    return this->request;
    this->available = false;
}

TBLEMessanger BLEMessanger;


