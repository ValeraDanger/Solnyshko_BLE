#include "TBLEMessanger.h"
#include "MyCallbacks.h"

void TBLEMessanger::init() {

    BLEDevice::init(BLE_NAME);
    pServer = BLEDevice::createServer();

    pServer->setCallbacks(new MyServerCallbacks());

    pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
                                            CHARACTERISTIC_UUID,
                                            BLECharacteristic::PROPERTY_READ |
                                            BLECharacteristic::PROPERTY_WRITE
                                        );

    pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

    pCharacteristic->setValue("Hello, world!");
    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();

    Serial.println("BLE server started!");
}

DynamicJsonDocument& TBLEMessanger::getRequest() {
    return this->request;
    this->available = false;
}

TBLEMessanger BLEMessanger;


