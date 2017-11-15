#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "Driver.h"

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266util {

  class MotorDriver : public Driver {

    public:

      // TODO
      bool isSetup();

      // TODO config_t
      bool setup(uint8_t pinPWM, uint8_t pinDir);

      int getSpeed();

      uint8_t getDirection();

      void setSpeed(int speed);

      void applySpeed(int speed);

      JsonObject& getDetails();

    private:

      uint8_t _pinPWM;
      uint8_t _pinDir;
      int _speed;

      bool _setupDone = false;
  };
}

#endif // MOTORDRIVER_H
