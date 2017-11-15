#include "Driver.h"

namespace esp8266util {

  static uint16_t PWM_RANGE = 1023;

  uint16_t Driver::getPWMRange() {
    return PWM_RANGE;
  }

  void Driver::setPWMRange(uint16_t pwmRange) {

    PWM_RANGE = pwmRange;
    analogWriteRange(PWM_RANGE);
    Log.verbose(F("Set PWM range to %d" CR), PWM_RANGE);
  }
}