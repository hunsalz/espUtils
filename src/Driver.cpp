#include "Driver.h"

namespace esp8266util {

  static uint16_t pwmRange = 1023;

  uint16_t Driver::getPWMRange() {
    return pwmRange;
  }

  void Driver::setPWMRange(uint16_t _pwmRange) {

    pwmRange = _pwmRange;
    analogWriteRange(pwmRange);
    Log.verbose(F("Set PWM range to %d" CR), pwmRange);
  }
}
