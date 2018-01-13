#include "MotorDriver.h"

namespace esp8266util {

bool MotorDriver::begin(uint8_t pinPWM, uint8_t pinDir) {
  
  config_t config;
  config.pinPWM = pinPWM;
  config.pinDir = pinDir;
  begin(config);
}

bool MotorDriver::begin(config_t config) {
  
  _config = config;
  pinMode(_config.pinPWM, OUTPUT);
  pinMode(_config.pinDir, OUTPUT);
  LOG.verbose(F("Setup motor done : PWM pin = %d and direction pin = %d"), _config.pinPWM, _config.pinDir);

  return true;
}

MotorDriver::config_t MotorDriver::getConfig() { return _config; }

JsonObject &MotorDriver::getConfigAsJson() {
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json["pinPWM"] = _config.pinPWM;
  json["pinDir"] = _config.pinDir;

  return json;
}

uint8_t MotorDriver::getDirection() { return getSpeed() > 0 ? 1 : 0; }

int MotorDriver::getSpeed() { return _speed; }

void MotorDriver::setSpeed(int speed) {
  
  // limit speed to max. PWM range +/-
  if (speed > getPWMRange()) {
    speed = getPWMRange();
  } else if (speed < -getPWMRange()) {
    speed = -getPWMRange();
  }
  LOG.verbose(F("Write speed = %d"), speed);
  // write speed to PWM
  analogWrite(_config.pinPWM, abs(speed));
  // change direction accordingly to original signed speed to HIGH or LOW
  digitalWrite(_config.pinDir, getDirection());
  // save new speed value
  _speed = speed;

  LOG.verbose(F("Write speed = %d - DONE"), _speed);
}

void MotorDriver::applySpeed(int speed) { setSpeed(getSpeed() + speed); }

JsonObject &MotorDriver::getDetails() {
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("pinPWM")] = _config.pinPWM;
  json[F("pinDir")] = _config.pinDir;
  json[F("pwmRange")] = getPWMRange();
  json[F("speed")] = getSpeed();

  return json;
}
} // namespace esp8266util
