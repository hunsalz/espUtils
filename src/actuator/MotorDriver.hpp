#pragma once

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson

#include "Driver.hpp"

namespace esp8266utils {

class MotorDriver : public Driver {
 
 public:
  
  struct config_t {
    uint8_t pinPWM;
    uint8_t pinDir;
  };

  bool begin(uint8_t pinPWM, uint8_t pinDir) {
    
    config_t config;
    config.pinPWM = pinPWM;
    config.pinDir = pinDir;
    begin(config);
  }

  bool begin(config_t config) {
    
    _config = config;
    pinMode(_config.pinPWM, OUTPUT);
    pinMode(_config.pinDir, OUTPUT);
    VERBOSE_FP(F("Setup motor done : PWM pin = %d and direction pin = %d"), _config.pinPWM, _config.pinDir);
    return true;
  }

  config_t getConfig() {
    return _config;
  }

  uint8_t getDirection() {
    return getSpeed() > 0 ? 1 : 0;
  }

  int getSpeed() {
    return _speed;
  }

  void setSpeed(int speed) {
    
    // limit speed to max. PWM range +/-
    if (speed > getPWMRange()) {
      speed = getPWMRange();
    } else if (speed < -getPWMRange()) {
      speed = -getPWMRange();
    }
    VERBOSE_FP(F("Write speed = %d"), speed);
    // write speed to PWM
    analogWrite(_config.pinPWM, abs(speed));
    // change direction accordingly to original signed speed to HIGH or LOW
    digitalWrite(_config.pinDir, getDirection());
    // save new speed value
    _speed = speed;
    VERBOSE_FP(F("Write speed = %d - DONE"), _speed);
  }

  void applySpeed(int speed) {
    setSpeed(getSpeed() + speed);
  }

  size_t serialize(String& output) {

    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    object[F("pinPWM")] = _config.pinPWM;
    object[F("pinDir")] = _config.pinDir;
    object[F("pwmRange")] = getPWMRange();
    object[F("speed")] = getSpeed();
    serializeJson(object, output);
    return measureJson(object);
  }

 private:
  
  config_t _config;
  int _speed;
};
}  // namespace esp8266utils