#pragma once

#include <Arduino.h>
#include "LampStates.h"

#define RELAY_PIN 22

class TRelay {
	public:
        void init();

		void turnOn();

		void turnOff();

};

extern TRelay Relay;