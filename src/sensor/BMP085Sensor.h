#ifndef BMP085_SENSOR_H
#define BMP085_SENSOR_H

#include <Adafruit_BMP085_U.h> // https://github.com/adafruit/Adafruit_BMP085_Unified

#include "Configurable.h"

namespace esp8266utils {

class BMP085Sensor : public Configurable {

public:
  bool begin(float seaLevelPressure = 1013.25);

  String getConfigAsJson();

  Adafruit_BMP085_Unified &getBMP085();

  bool update(bool mock = false);

  float getSeaLevelPressure(); // unit is Pascal, Pa

  float getTemperature(); // unit is Celsius, °C

  float getPressure(); // unit is Pascal, Pa

  float getApproximateAltitude(); // unit in Meter, m

  String getValuesAsJson();

private:
  Adafruit_BMP085_Unified *_bmp085 = NULL;

  float _seaLevelPressure = 1013.25;
  float _temperature = NAN;
  float _pressure = NAN;
  float _altitude = NAN;
};
} // namespace esp8266utils

#endif // BMP085_SENSOR_H
