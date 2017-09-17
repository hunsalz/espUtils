#pragma once

#include "Driver.h"

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

class MotorShieldDriver : public Driver {

	public:

		MotorShieldDriver(bool enabled,
			unsigned int pinPWM_A,
			unsigned int pinDirA,
			unsigned int pinPWM_B,
			unsigned int pinDirB,
			unsigned int pwmRange);

		int getSpeedA();

		int getSpeedB();

		int getDirectionA();

		int getDirectionB();

		void setSpeedA(int speed);

		void setSpeedB(int speed);

    void applySpeedA(int speed);

		void applySpeedB(int speed);

		JsonObject& getConfig();

	private:

		unsigned int pinPWM_A;
		unsigned int pinPWM_B;
		unsigned int pinDirA;
		unsigned int pinDirB;
    unsigned int pwmRange;

    void setSpeed(int speed, unsigned int pinPWM, unsigned int pinDir);
};
