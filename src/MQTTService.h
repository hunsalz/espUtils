#ifndef MQTT_SERVICE_H
#define MQTT_SERVICE_H

#include <AsyncMqttClient.h>  // https://github.com/marvinroger/async-mqtt-client
#include <Log4Esp.h>          // https://github.com/hunsalz/log4Esp

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson

namespace esp8266util {

class MQTTService {
 public:
  struct config_t {
    const char *hostName;
    uint16_t port;
  };

  bool begin(const char *hostName, uint16_t port);

  bool begin(config_t config);

  config_t getConfig();

  void end();

  AsyncMqttClient &getMqttClient();

  void publish(const char *topic, JsonObject &json);

  void publish(const char *topic, JsonArray &json);

 private:
  config_t _config;
  AsyncMqttClient _mqttClient;
};
}  // namespace esp8266util

#endif  // MQTT_SERVICE_H
