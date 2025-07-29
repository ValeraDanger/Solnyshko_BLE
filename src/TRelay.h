#pragma once

#include <Arduino.h>
#include "LampStates.h"

#define RELAY_PIN 18

class TRelay {
	public:
        void init();

		void turnOn();

		void turnOff();

};

extern TRelay Relay;