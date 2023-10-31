#include <Arduino.h>
#include <ArduinoJson.h>
#include "LampStates.h"
#include "TBLEMessanger.h"
#include "TRelay.h"
#include "TPreheater.h"
#include "TTimer.h"


DynamicJsonDocument status(200);



TaskHandle_t timerControllerTaskHandler;
void timerController(void* pvParams) {
    uint32_t time = *((uint32_t*)pvParams);

    Preheater.start();
    status["state"] = LampState::PREHEATING;
    BLEMessanger.notifyStateChanges();
    while (!Preheater.tick()) {       //while timer is not ended
      vTaskDelay(5);
    }
    Preheater.stop();

    Timer.setTime(time);
    Timer.start();
    status["state"] = LampState::ACTIVE;
    BLEMessanger.notifyStateChanges();

    while (!Timer.tick()) {       //while timer is not ended
      vTaskDelay(5);
    }
    Timer.stop();
    status["state"] = LampState::OFF; 
    BLEMessanger.notifyStateChanges();

    timerControllerTaskHandler = NULL;
    vTaskDelete(NULL);
}


TaskHandle_t proccessCommandTaskHandler;
void proccessCommand(void* pvParams) {
  for(;;) {
    vTaskSuspend(NULL);
    DynamicJsonDocument command = BLEMessanger.getRequest();
    serializeJsonPretty (command, Serial);
    Serial.println();

    JsonObject::iterator it = command.as<JsonObject>().begin();    //using for key parsing next
    JsonPair object = *it;                                             //first object in json

    if (!strcmp(object.key().c_str(), "relay")) {

      int state = command["relay"].as<int>();

      if (state == LampState::ON) {

        if (timerControllerTaskHandler != NULL) {
          vTaskDelete(timerControllerTaskHandler);
          timerControllerTaskHandler = NULL;
        }
        Preheater.stop();
        Timer.stop();

        Relay.turnOn();

        status["state"] = LampState::ON;
        
      }

      else if (state == LampState::OFF) {

        if (timerControllerTaskHandler != NULL) {
          vTaskDelete(timerControllerTaskHandler);
          timerControllerTaskHandler = NULL;
        }
        Preheater.stop();
        Timer.stop();

        Relay.turnOff();
        
        status["state"] = LampState::OFF;
      }
    }

    else if ( !strcmp(object.key().c_str(), "timer") ) {

      if ( !strcmp(command["timer"]["action"], "set") ) {
        uint32_t time = command["timer"]["time"].as<uint32_t>();
        if (timerControllerTaskHandler != NULL) {
          vTaskDelete(timerControllerTaskHandler);
          timerControllerTaskHandler = NULL;
        }
        Preheater.stop();
        Timer.stop();

        xTaskCreate(
          timerController,   /* Task method pointer*/
          "Timer pipeline controll task",          /* Task name*/
          10000,                       /* Stack deepth*/
          &time,               /* Pointer on class object itself */
          2,                                  /* Priority*/
          &timerControllerTaskHandler                       /* Task handle*/
        ); 
      }

      else if ( !strcmp(command["timer"]["action"], "stop") ) { 

        if (timerControllerTaskHandler != NULL) {
          vTaskDelete(timerControllerTaskHandler);
          timerControllerTaskHandler = NULL;
        }
        Preheater.stop();
        Timer.stop();
        status["state"] = LampState::OFF;

      }

      else if ( !strcmp(command["timer"]["action"], "pause") ) { 
        if (status["state"].as<int>() == LampState::ACTIVE) {
          // if (timerControllerTaskHandler != NULL) {
          //   vTaskSuspend(timerControllerTaskHandler);
          // }
          Timer.pause();
          status["state"] = LampState::PAUSED;
        } 
      }

      else if ( !strcmp(command["timer"]["action"], "resume") ) { 
        if (status["state"].as<int>() == LampState::PAUSED) {
          // if (timerControllerTaskHandler != NULL) {
          //   vTaskResume(timerControllerTaskHandler);
          // }
          Timer.resume();
          status["state"] = LampState::ACTIVE;
        }
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  status["state"] = LampState::OFF;
  status["timer"]["time_left"] = 0;
  status["preheat"]["time_left"] = 0;

  Relay.init();

  xTaskCreate(
    proccessCommand,   /* Task method pointer*/
    "Command proccess task",          /* Task name*/
    10000,                       /* Stack deepth*/
    NULL,               /* Pointer on class object itself */
    2,                                  /* Priority*/
    &proccessCommandTaskHandler                        /* Task handle*/
  ); 

  BLEMessanger.init();
}

void loop() {
  // put your main code here, to run repeatedly:
}

