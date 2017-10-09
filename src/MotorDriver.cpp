#include "MotorDriver.h"

namespace esp8266util {

	bool MotorDriver::isSetup() {
		return setupDone;
	}

	bool MotorDriver::setup(uint8_t pinPWM, uint8_t pinDir, uint16_t pwmRange) {

		this->pinPWM = pinPWM;
		pinMode(pinPWM, OUTPUT);
		this->pinDir = pinDir;
		pinMode(pinDir, OUTPUT);
		setPWMRange(pwmRange);

		Log.verbose(F("Setup motor done : PWM pin = %d and direction pin = %d" CR), pinPWM, pinDir);

		setupDone = true;

		return isSetup();
	}

	int MotorDriver::getSpeed() {
		return speed;
	}

	uint8_t MotorDriver::getDirection() {
		return getSpeed() > 0 ? 1 : 0;
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

		Log.verbose(F("Write speed = %d - DONE" CR), speed);
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
}
