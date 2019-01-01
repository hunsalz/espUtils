#ifndef BMP085_SENSOR_H
#define BMP085_SENSOR_H

#include <Adafruit_BMP085_U.h>  // https://github.com/adafruit/Adafruit_BMP085_Unified
#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson

#include "Logging.hpp"
#include "polyfills/Json2String.h"
#include "Sensor.hpp"

namespace esp8266utils {

class BMP085Sensor : public Sensor {

public:
  bool begin(float seaLevelPressure = 1013.25, const char *device = "DefaultDevice");

  Adafruit_BMP085_Unified &getBMP085();

  bool update(bool mock = false);

  float getSeaLevelPressure(); // unit is Pascal, Pa

  float getTemperature(); // unit is Celsius, °C

  float getPressure(); // unit is Pascal, Pa

  float getApproximateAltitude(); // unit in Meter, m

  const char *getDevice();

  String getValuesAsJson();

private:
  Adafruit_BMP085_Unified *_bmp085 = NULL;

  float _seaLevelPressure = 1013.25;
  float _temperature = NAN;
  float _pressure = NAN;
  float _altitude = NAN;
  const char *_device = NULL;
};
} // namespace esp8266utils

#endif // BMP085_SENSOR_H
