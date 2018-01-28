#ifndef MQ135_SENSOR_H
#define MQ135_SENSOR_H

#include <MQ135.h> // https://github.com/GeorgK/MQ135 - Set up sensor for the first time? https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

#include "Configurable.h"

namespace esp8266util {

class MQ135Sensor : public Configurable {

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

  bool update(bool mock = false);

  float getPPM(); // unit in PPM

  float getCO2(); // unit in percent, %

  JsonObject &getJsonValue();

private:
  MQ135 *_mq135 = NULL;
  config_t _config;

  float _ppm = NAN;
};
} // namespace esp8266util

#endif // MQ135_SENSOR_H
