#pragma once

#include "Driver.h"

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266util {

	class MotorDriver : public Driver {

		public:

			MotorDriver(bool _enabled,
				uint8_t _pinPWM,
				uint8_t _pinDir,
				int _pwmRange = 1023);

			int getSpeed();

			uint8_t getDirection();

			int getPWMRange();

			void setSpeed(int speed);

	    void applySpeed(int speed);

			JsonObject& getDetails();

		private:

			uint8_t pinPWM;
			uint8_t pinDir;
	    int pwmRange;
			int speed;
	};
}
