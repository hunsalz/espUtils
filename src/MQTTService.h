#ifndef MQTTSERVICE_H
#define MQTTSERVICE_H

#include <ArduinoJson.h>     // https://github.com/bblanchon/ArduinoJson
#include <AsyncMqttClient.h> // https://github.com/marvinroger/async-mqtt-client
#include <Log4Esp.h>         // https://github.com/hunsalz/log4Esp

#include "Service.h"

using log4Esp::LOG;

namespace esp8266util {

class MQTTService : public Service {

public:
  struct config_t {
    const char *hostName;
    uint16_t port;
  };

  ~MQTTService();

  bool available();

  bool begin(const char *hostName, uint16_t port);

  bool begin(config_t config);

  config_t getConfig();

  bool end();

  AsyncMqttClient &getMqttClient();

  void publish(const char *topic, JsonObject &json);

  void publish(const char *topic, JsonArray &json);

private:
  config_t _config;
  AsyncMqttClient _mqttClient;
};
} // namespace esp8266util

#endif // MQTTSERVICE_H
