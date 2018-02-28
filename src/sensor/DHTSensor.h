#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <DHT.h>         // https://github.com/adafruit/DHT-sensor-library
#include <DHT_U.h>       // https://github.com/adafruit/DHT-sensor-library

#include "Configurable.h"

namespace esp8266util {

class DHTSensor : public Configurable {

public:
  struct config_t {
    uint8_t pin;
    /**
    Valid types:
    DHT11 = 11
    DHT21 = 21
    DHT22 = 22
    **/
    uint8_t type;
  };

  bool begin(uint8_t pin, uint8_t type);

  bool begin(config_t config);

  bool begin(JsonObject &json);

  config_t getConfig();

  const char* getConfigAsJson();

  DHT_Unified &getDHT();

  bool update(bool mock = false);

  float getTemperature(); // unit in Celsius, °C

  float getHumidity(); // unit in percent, %

  const char* getJsonValue();

private:
  DHT_Unified *_dht = NULL;
  config_t _config;

  float _temperature = NAN;
  float _humidity = NAN;
};
} // namespace esp8266util

#endif // DHT_SENSOR_H
