#pragma once

#include "Logging.hpp"

namespace esp8266utils {

class Driver {
 
 public:
  
  uint16_t getPWMRange() {
    return _pwmRange;
  }

  void setPWMRange(uint16_t pwmRange) {
    
    _pwmRange = pwmRange;
    analogWriteRange(_pwmRange);
    VERBOSE_MSG_P(F("Set PWM range to %d"), _pwmRange);
  }

  // TODO add analogWriteFreq(...) analog to setPWMRange(...)

 private:
  
  uint16_t _pwmRange  = 1023;

};
}  // namespace esp8266utils