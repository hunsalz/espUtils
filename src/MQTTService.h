#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <AsyncMqttClient.h> // https://github.com/marvinroger/async-mqtt-client
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

	class MQTTService : public Service {

		public:

	    ~MQTTService();

			bool isSetup();

	    bool isRunning();

	    bool start();

	    bool stop();

			AsyncMqttClient* getMqttClient();

			bool setup(const char* server, uint16_t port);

			const char* getHostName();

			uint16_t getPort();

	    void publish(const char* topic, JsonObject& json);

	  private:

			AsyncMqttClient mqttClient;

			const char* hostName;
			uint16_t port;

			bool setupDone = false;
	};
}
