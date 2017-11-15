#ifndef DRIVER_H
#define DRIVER_H

#include <Arduino.h>
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

namespace esp8266util {

  class Driver {

    public:

      // use this method to know if this driver is setup or not
      virtual bool isSetup() = 0;

      static uint16_t getPWMRange();

      static void setPWMRange(uint16_t pwmRange = 1023);

      // TODO add analogWriteFreq analog to pwm range

  };
}

#endif // DRIVER_H