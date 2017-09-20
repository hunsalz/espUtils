#pragma once

#include "Driver.h"

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

class MotorShieldDriver : public Driver {

	public:

		MotorShieldDriver(bool enabled,
			uint8_t pinPWM_A,
			uint8_t pinDirA,
			uint8_t pinPWM_B,
			uint8_t pinDirB,
			uint16_t pwmRange = 1023);

		int getSpeedA();

		int getSpeedB();

		void setSpeedA(int speed);

		void setSpeedB(int speed);

    void applySpeedA(int speed);

		void applySpeedB(int speed);

		uint8_t getDirectionA();

		uint8_t getDirectionB();

		JsonObject& getDetails();

	private:

		uint8_t pinPWM_A;
		uint8_t pinPWM_B;
		uint8_t pinDirA;
		uint8_t pinDirB;
    uint16_t pwmRange;
		int speedA;
		int speedB;

    void setSpeed(int speed, uint8_t pinPWM, uint8_t pinDir);
};
