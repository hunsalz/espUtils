#pragma once

#ifdef ESP8266
extern "C" {
  #include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

	class EspService : public Service {

		public:

	    EspService();

	    ~EspService();

			bool isRunning();

	    bool start();

	    bool stop();

			JsonObject& getDetails();
	};
}
