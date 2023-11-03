#include <Arduino.h>
#include <ArduinoJson.h>
#include "LampStates.h"
#include "TBLEMessanger.h"
#include "TRelay.h"
#include "TPreheater.h"
#include "TTimer.h"


DynamicJsonDocument status(200);

void changeLampState(int state) {
  status["state"] = state;
  BLEMessanger.notifyStateChanges();
}

TaskHandle_t timerControllerTaskHandler;
void timerController(void* pvParams) {

  DynamicJsonDocument& command = *(DynamicJsonDocument*)pvParams;  //transform pointer into link
  uint32_t cycle_time = command["timer"]["time"].as<uint32_t>();
  uint32_t cycles_num = command["timer"]["cycles"].as<uint32_t>();

  status["timer"]["cycles"] = cycles_num;
  status["timer"]["cycle_time"] = cycle_time;

  Preheater.start();
  changeLampState(LampState::PREHEATING);
  while (!Preheater.tick()) {       //while timer is not ended
    vTaskDelay(5);
  }
  Preheater.stop();

  uint32_t total_cycles_time_left = cycle_time * cycles_num;
  Timer.setTime(total_cycles_time_left);
  total_cycles_time_left -= cycle_time;
  Timer.start();
  changeLampState(LampState::ACTIVE);

  while (!Timer.tick()) {       //while timer is not ended
    vTaskDelay(5);

    if (Timer.getTime() < total_cycles_time_left) {
      Timer.pause();
      changeLampState(LampState::PAUSED);                       //pause lamp when cycle ends
      total_cycles_time_left -= cycle_time;
    }
  }
  Timer.stop();
  changeLampState(LampState::OFF);

  // status["timer"]["cycles"] = 0;
  // status["timer"]["cycle_time"] = 0;

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

        changeLampState(LampState::ON);
        
      }

      else if (state == LampState::OFF) {

        if (timerControllerTaskHandler != NULL) {
          vTaskDelete(timerControllerTaskHandler);
          timerControllerTaskHandler = NULL;
        }
        Preheater.stop();
        Timer.stop();

        Relay.turnOff();

        changeLampState(LampState::OFF);  
        
      }
    }

    else if ( !strcmp(object.key().c_str(), "timer") ) {

      if ( !strcmp(command["timer"]["action"], "set") ) {
        //uint32_t time = command["timer"]["time"].as<uint32_t>();
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
          &command,               /* Pointer on class object itself */
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

        changeLampState(LampState::OFF);  
      }

      else if ( !strcmp(command["timer"]["action"], "pause") ) { 
        if (status["state"].as<int>() == LampState::ACTIVE) {
          Timer.pause();
          changeLampState(LampState::PAUSED); 
        } 
      }

      else if ( !strcmp(command["timer"]["action"], "resume") ) { 
        if (status["state"].as<int>() == LampState::PAUSED) {
          Timer.resume();
          changeLampState(LampState::ACTIVE);
        }
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  status["state"] = LampState::OFF;
  status["timer"]["time_left"] = 0;
  status["timer"]["cycles"] = 0;
  status["timer"]["cycle_time"] = 0;
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

