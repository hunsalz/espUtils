#include "Driver.h"

namespace esp8266util {

	uint16_t Driver::getPWMRange() {
		return pwmRange;
	}

	// TODO "singleton" 
	void Driver::setPWMRange(uint16_t pwmRange) {

		if (this->pwmRange != pwmRange) {
			this->pwmRange = pwmRange;
			analogWriteRange(pwmRange);
			Log.verbose(F("Set PWM range to %d" CR), this->pwmRange);
		}
	}
}
