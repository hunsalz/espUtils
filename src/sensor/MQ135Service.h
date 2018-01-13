#ifndef MQ135SERVICE_H
#define MQ135SERVICE_H

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <MQ135.h> // https://github.com/GeorgK/MQ135 - Set up sensor for the first time? https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

#include "Configurable.h"

namespace esp8266util {

// TODO Service
class MQ135Service : public Configurable {

public:
  struct config_t {
    uint8_t pin;
  };

  bool begin(uint8_t pin);

  bool begin(config_t pin);

  bool begin(JsonObject &json);

  config_t getConfig();

  JsonObject &getConfigAsJson();

  MQ135 &getMQ135();

  bool update();

  float getPPM(); // unit in PPM

  float getCO2(); // unit in percent, %

private:
  MQ135 *_mq135 = NULL;
  config_t _config;

  float _ppm = NAN;
};
} // namespace esp8266util

#endif // MQ135SERVICE_H
