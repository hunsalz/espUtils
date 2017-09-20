#include "MotorShieldDriver.h"

MotorShieldDriver::MotorShieldDriver(bool enabled,
	uint8_t pinPWM_A,
	uint8_t pinDirA,
	uint8_t pinPWM_B,
	uint8_t pinDirB,
	uint16_t pwmRange) : Driver(enabled) {

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
	analogWriteFreq(1000);

  Log.verbose(F("Setup motor shield done : PWM(A) = %d with direction = %d, PWM(B) = %d with direction = %d" CR), pinPWM_A, pinDirA, pinPWM_B, pinDirB);
}

int MotorShieldDriver::getSpeedA() {
	return speedA;
}

int MotorShieldDriver::getSpeedB() {
	return speedB;
}

void MotorShieldDriver::setSpeedA(int speed) {

	if (isEnabled()) {
		speedA = speed;
		setSpeed(speedA, pinPWM_A, pinDirA);
		Log.verbose(F("Applied speed of motor(A) to %d with direction to %d" CR), getSpeedA(), getDirectionA());
	}
}

void MotorShieldDriver::setSpeedB(int speed) {

	if (isEnabled()) {
		speedB = speed;
		setSpeed(speedB, pinPWM_B, pinDirB);
		Log.verbose(F("Applied speed of motor(B) to %d with direction to %d" CR), getSpeedB(), getDirectionB());
	}
}

void MotorShieldDriver::applySpeedA(int speed) {
  setSpeedA(getSpeedA() + speed);
}

void MotorShieldDriver::applySpeedB(int speed) {
  setSpeedB(getSpeedB() + speed);
}

uint8_t MotorShieldDriver::getDirectionA() {
	return digitalRead(pinDirA);
}

uint8_t MotorShieldDriver::getDirectionB() {
	return digitalRead(pinDirB);
}

JsonObject& MotorShieldDriver::getDetails() {

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

void MotorShieldDriver::setSpeed(int speed, uint8_t pinPWM, uint8_t pinDir) {

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
