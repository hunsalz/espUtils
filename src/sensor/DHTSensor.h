#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <DHT.h>         // https://github.com/adafruit/DHT-sensor-library
#include <DHT_U.h>       // https://github.com/adafruit/DHT-sensor-library

#include "Logging.hpp"
#include "polyfills/Json2String.h"
#include "Sensor.hpp"

namespace esp8266utils {

class DHTSensor : public Sensor {

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

  DHT_Unified &getDHT();

  bool update(bool mock = false);

  float getTemperature(); // unit is Celsius, °C

  float getHumidity(); // unit in percent, %

  String getValuesAsJson();

private:
  DHT_Unified *_dht = NULL;
  config_t _config;

  float _temperature = NAN;
  float _humidity = NAN;
};
} // namespace esp8266utils

#endif // DHT_SENSOR_H
