#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "Driver.h"

namespace esp8266util {

class MotorDriver : public Driver {

public:
  struct config_t {
    uint8_t pinPWM;
    uint8_t pinDir;
  };

  bool begin(uint8_t pinPWM, uint8_t pinDir);

  bool begin(config_t config);

  config_t getConfig();

  const char* getConfigAsJson();

  uint8_t getDirection();

  int getSpeed();

  void setSpeed(int speed);

  void applySpeed(int speed);

  const char* getDetails();

private:
  config_t _config;
  int _speed;
};
} // namespace esp8266util

#endif // MOTOR_DRIVER_H
