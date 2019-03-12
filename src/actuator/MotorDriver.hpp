#pragma once

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson

#include "../Logging.hpp"

namespace espUtils {

class MotorDriver {
 
 public:
  
  struct config_t {
    uint8_t pinPWM;
    uint8_t pinDir;
    uint8_t pwmRange;
  };

  bool begin(uint8_t pinPWM, uint8_t pinDir, uint8_t pwmRange) {
    
    config_t config;
    config.pinPWM = pinPWM;
    config.pinDir = pinDir;
    config.pwmRange = pwmRange;
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
    if (speed > _config.pwmRange) {
      speed = _config.pwmRange;
    } else if (speed < -_config.pwmRange) {
      speed = -_config.pwmRange;
    }
    TRACE_FP(F("Write speed = %d"), speed);
    // write speed to PWM
    #ifdef ESP32
      // TODO ESP32
      // https://github.com/espressif/arduino-esp32/issues/4
      // https://github.com/ERROPiX/ESP32_AnalogWrite
      WARNING_P(F("analogWrite() not implemented."));
    #else
      analogWrite(_config.pinPWM, abs(speed));
    #endif
    // change direction accordingly to original signed speed to HIGH or LOW
    digitalWrite(_config.pinDir, getDirection());
    // save new speed value
    _speed = speed;
    TRACE_FP(F("Write speed = %d - DONE"), _speed);
  }

  void applySpeed(int speed) {
    setSpeed(getSpeed() + speed);
  }

  size_t serialize(String& output) {

    const size_t CAPACITY = JSON_OBJECT_SIZE(4) + 60;
    StaticJsonDocument<CAPACITY> doc;
    JsonObject object = doc.to<JsonObject>();
    object[F("pinPWM")] = _config.pinPWM;
    object[F("pinDir")] = _config.pinDir;
    object[F("pwmRange")] = _config.pwmRange;
    object[F("speed")] = getSpeed();
    serializeJson(object, output);
    return measureJson(object);
  }

  static void applyPWMRange(uint16_t pwmRange = 255) {
    
    #ifdef ESP32
    // TODO ESP32
    // https://github.com/espressif/arduino-esp32/issues/4
    // https://github.com/ERROPiX/ESP32_AnalogWrite
      WARNING_P(F("applyPWMRange() not implemented."));
    #else
      analogWriteRange(pwmRange);
      VERBOSE_FP(F("Set PWM range to %d"), pwmRange);
    #endif
  }

 private:
  
  config_t _config;
  int _speed;
};
}  // namespace espUtils