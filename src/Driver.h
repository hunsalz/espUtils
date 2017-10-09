#pragma once

#include <Arduino.h>
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

namespace esp8266util {

	class Driver {

		public:

			// use this method to know if this driver is setup or not
			virtual bool isSetup() = 0;

			uint16_t getPWMRange();

			void setPWMRange(uint16_t pwmRange = 1023);

			// TODO add analogWriteFreq analog to pwm range

		private:

			uint16_t pwmRange;
	};
}
