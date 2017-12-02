#ifndef MQTTSERVICE_H
#define MQTTSERVICE_H

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <AsyncMqttClient.h> // https://github.com/marvinroger/async-mqtt-client
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

  class MQTTService : public Service {

    public:

      struct config_t {
        const char* hostName;
        uint16_t port;
      };

      ~MQTTService();

      bool available();

      bool begin(const char* hostName, uint16_t port);

      bool begin(config_t config);

      config_t getConfig();

      bool end();

      AsyncMqttClient& getMqttClient();

      void publish(const char* topic, JsonObject& json);

      void publish(const char* topic, JsonArray& json);

    private:

      AsyncMqttClient _mqttClient;
      config_t _config;
  };
}

#endif // MQTTSERVICE_H
