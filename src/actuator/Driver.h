#ifndef DRIVER_H
#define DRIVER_H

#include "Configurable.h"

namespace esp8266utils {

class Driver : public Configurable {

public:
  static uint16_t getPWMRange();

  static void setPWMRange(uint16_t pwmRange = 1023);

  // TODO add analogWriteFreq analog to pwm range
};
} // namespace esp8266utils

#endif // DRIVER_H