#include "MotorShieldDriver.h"

MotorShieldDriver::MotorShieldDriver(bool enabled,
	unsigned int pinPWM_A,
	unsigned int pinDirA,
	unsigned int pinPWM_B,
	unsigned int pinDirB,
	unsigned int pwmRange) : Driver(enabled) {

	this->pinPWM_A = pinPWM_A;
	this->pinDirA = pinDirA;
	this->pinPWM_B = pinPWM_B;
	this->pinDirB = pinDirB;
  this->pwmRange = pwmRange;

	pinMode(this->pinPWM_A, OUTPUT);
	pinMode(this->pinDirA, OUTPUT);
	pinMode(this->pinPWM_B, OUTPUT);
	pinMode(this->pinDirB, OUTPUT);
	analogWriteRange(this->pwmRange);

  Log.verbose(F("Setup motor shield done : PWM(A) = %d with direction = %d, PWM(B) = %d with direction = %d" CR), pinPWM_A, pinDirA, pinPWM_B, pinDirB);
}

int MotorShieldDriver::getSpeedA() {
	return analogRead(pinPWM_A);
}

int MotorShieldDriver::getSpeedB() {
	return analogRead(pinPWM_B);
}

int MotorShieldDriver::getDirectionA() {
	return digitalRead(pinDirA);
}

int MotorShieldDriver::getDirectionB() {
	return digitalRead(pinDirB);
}

void MotorShieldDriver::setSpeedA(int speed) {

	if (isEnabled()) {
		setSpeed(speed, pinPWM_A, pinDirA);

		// TODO verify log line

		Log.verbose(F("Applied speed of motor(A) to %d with direction to %d" CR), getSpeedA(), getDirectionA());
	}
}

void MotorShieldDriver::setSpeedB(int speed) {

	if (isEnabled()) {
		setSpeed(speed, pinPWM_B, pinDirB);

		// TODO verify log line

		Log.verbose(F("Applied speed of motor(B) to %d with direction to %d" CR), getSpeedB(), getDirectionB());
	}
}

void MotorShieldDriver::applySpeedA(int speed) {
  setSpeedA(getSpeedA() + speed);
}

void MotorShieldDriver::applySpeedB(int speed) {
  setSpeedB(getSpeedB() + speed);
}

void MotorShieldDriver::setSpeed(int speed, unsigned int pinPWM, unsigned int pinDir) {

  // limit speed to max. PWM
  int _speed = abs(speed);
  if (_speed > pwmRange) {
    _speed = pwmRange;
  }
  // write speed to PWM
  analogWrite(pinPWM, _speed);
  // change direction accordingly to original signed speed to HIGH or LOW
  digitalWrite(pinDir, speed > 0 ? HIGH : LOW);
}

JsonObject& MotorShieldDriver::getConfig() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("pinPWM_A")] = pinPWM_A;
  json[F("pinDirA")] = pinDirA;
  json[F("pinPWM_B")] = pinPWM_B;
  json[F("pinDirB")] = pinDirB;
  json[F("pwmRange")] = pwmRange;
  json[F("speedA")] = getSpeedA();
  json[F("dirA")] = getDirectionA();
	json[F("speedB")] = getSpeedB();
  json[F("dirB")] = getDirectionB();

  return json;
}
