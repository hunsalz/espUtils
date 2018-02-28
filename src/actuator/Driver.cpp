#include "Driver.h"

namespace esp8266utils {

static uint16_t PWM_RANGE = 1023;

uint16_t Driver::getPWMRange() { return PWM_RANGE; }

void Driver::setPWMRange(uint16_t pwmRange) {
  
  PWM_RANGE = pwmRange;
  analogWriteRange(PWM_RANGE);
  LOG.verbose(F("Set PWM range to %d"), PWM_RANGE);
}
} // namespace esp8266utils