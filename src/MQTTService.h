#ifndef MQTTSERVICE_H
#define MQTTSERVICE_H

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <AsyncMqttClient.h> // https://github.com/marvinroger/async-mqtt-client
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

  class MQTTService : public Service {

    public:

      ~MQTTService();

      bool isSetup();

      bool available();

      bool begin();

      bool end();

      AsyncMqttClient& getMqttClient();

      bool setup(const char* server, uint16_t port);

      const char* getHostName();

      uint16_t getPort();

      void publish(const char* topic, JsonObject& json);

    private:

      AsyncMqttClient _mqttClient;

      const char* _hostName;
      uint16_t _port;

      bool _setupDone = false;
  };
}

#endif // MQTTSERVICE_H
