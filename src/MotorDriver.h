#pragma once

#include "Driver.h"

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266util {

	class MotorDriver : public Driver {

		public:

			bool isSetup();

			bool setup(uint8_t pinPWM, uint8_t pinDir);

			int getSpeed();

			uint8_t getDirection();

			void setSpeed(int speed);

	    void applySpeed(int speed);

			JsonObject& getDetails();

		private:

			uint8_t pinPWM;
			uint8_t pinDir;
			int speed;

			bool setupDone = false;
	};
}
