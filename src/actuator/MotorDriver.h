#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "Driver.h"

namespace esp8266utils {

class MotorDriver : public Driver {

public:
  struct config_t {
    uint8_t pinPWM;
    uint8_t pinDir;
  };

  bool begin(uint8_t pinPWM, uint8_t pinDir);

  bool begin(config_t config);

  config_t getConfig();

  String getConfigAsJson();

  uint8_t getDirection();

  int getSpeed();

  void setSpeed(int speed);

  void applySpeed(int speed);

  String getDetails();

private:
  config_t _config;
  int _speed;
};
} // namespace esp8266utils

#endif // MOTOR_DRIVER_H
