#ifndef DRIVER_H
#define DRIVER_H

#include <Arduino.h>
#include <Logger.h> // https://github.com/hunsalz/log4arduino

using log4arduino::LOG;

namespace esp8266util {

  class Driver {

    public:

      static uint16_t getPWMRange();

      static void setPWMRange(uint16_t pwmRange = 1023);

      // TODO add analogWriteFreq analog to pwm range

  };
}

#endif // DRIVER_H