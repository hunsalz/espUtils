#pragma once

#include <Arduino.h>
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

class Service {

	public:

		Service();
		~Service();

		// returns if this service is running
		bool isRunning();

		// starts this service and returns true if service is running
		virtual bool start() = 0;

    // stops this service and returns true if service is stopped
    virtual bool stop() = 0;

	private:

		bool _running = false;
};
