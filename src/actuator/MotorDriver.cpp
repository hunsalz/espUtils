#include "MotorDriver.h"

namespace esp8266util {

  bool MotorDriver::isSetup() {
    return _setupDone;
  }

  bool MotorDriver::setup(uint8_t pinPWM, uint8_t pinDir) {

    _pinPWM = pinPWM;
    _pinDir = pinDir;
    pinMode(_pinPWM, OUTPUT);
    pinMode(_pinDir, OUTPUT);
    Log.verbose(F("Setup motor done : PWM pin = %d and direction pin = %d" CR), _pinPWM, _pinDir);

    _setupDone = true;

    return isSetup();
  }

  int MotorDriver::getSpeed() {
    return _speed;
  }

  uint8_t MotorDriver::getDirection() {
    return getSpeed() > 0 ? 1 : 0;
  }

  void MotorDriver::setSpeed(int speed) {

    // limit speed to max. PWM range +/-
    if (speed > getPWMRange()) {
      speed = getPWMRange();
    } else if (speed < -getPWMRange()) {
      speed = -getPWMRange();
    }
    Log.verbose(F("Write speed = %d" CR), speed);
    // write speed to PWM
    analogWrite(_pinPWM, abs(speed));
    // change direction accordingly to original signed speed to HIGH or LOW
    digitalWrite(_pinDir, getDirection());
    // save new speed value
    _speed = speed;

    Log.verbose(F("Write speed = %d - DONE" CR), _speed);
  }

  void MotorDriver::applySpeed(int speed) {
    setSpeed(getSpeed() + speed);
  }

  JsonObject& MotorDriver::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json[F("pinPWM")] = _pinPWM;
    json[F("pinDir")] = _pinDir;
    json[F("pwmRange")] = getPWMRange();
    json[F("speed")] = getSpeed();

    return json;
  }
}
