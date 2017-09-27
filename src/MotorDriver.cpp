#include "MotorDriver.h"

MotorDriver::MotorDriver(bool _enabled,
	uint8_t _pinPWM,
	uint8_t _pinDir,
	int _pwmRange) : Driver(_enabled) {

	pinPWM = _pinPWM;
	pinDir = _pinDir;
	pwmRange = _pwmRange;

	// TODO make an init method for pin assignments ?

	pinMode(pinPWM, OUTPUT);
	pinMode(pinDir, OUTPUT);
	// set PWM only once
	analogWriteRange(pwmRange);
	analogWriteFreq(1000);

  Log.verbose(F("Setup motor done : PWM pin = %d and direction pin = %d" CR), pinPWM, pinDir);
}

int MotorDriver::getSpeed() {
	return speed;
}

uint8_t MotorDriver::getDirection() {
	return getSpeed() > 0 ? 1 : 0;
}

int MotorDriver::getPWMRange() {
	return pwmRange;
}

void MotorDriver::setSpeed(int _speed) {

	speed = _speed;

	// limit speed to max. PWM range +/-
  if (speed > getPWMRange()) {
    speed = getPWMRange();
  } else if (speed < -getPWMRange()) {
		speed = -getPWMRange();
	}
	Log.verbose(F("Write speed = %d" CR), speed);

  // write speed to PWM
  analogWrite(pinPWM, abs(speed));
  // change direction accordingly to original signed speed to HIGH or LOW
  digitalWrite(pinDir, getDirection());
}

void MotorDriver::applySpeed(int speed) {
  setSpeed(getSpeed() + speed);
}

JsonObject& MotorDriver::getDetails() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("pinPWM")] = pinPWM;
  json[F("pinDir")] = pinDir;
  json[F("pwmRange")] = getPWMRange();
  json[F("speed")] = getSpeed();

  return json;
}
