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

    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    oAdvertisementData.setManufacturerData("SNSolnyshko OYFB-04M");  //Where 2 first bytes convert into Reserved manufactorer id;

    pAdvertising = pServer->getAdvertising();
    pAdvertising->setAdvertisementData(oAdvertisementData);
    pAdvertising->start();

    Serial.println("BLE server started!");
}

DynamicJsonDocument& TBLEMessanger::getRequest() {
    return this->request;
    this->available = false;
}

TBLEMessanger BLEMessanger;


