#ifndef DRIVER_H
#define DRIVER_H

#include "Logging.hpp"

namespace esp8266utils {

class Driver {

public:
  static uint16_t getPWMRange();

  static void setPWMRange(uint16_t pwmRange = 1023);

  // TODO add analogWriteFreq analog to pwm range
};
} // namespace esp8266utils

#endif // DRIVER_H