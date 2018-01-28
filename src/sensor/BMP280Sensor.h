#ifndef BMP280_SENSOR_H
#define BMP280_SENSOR_H

#include <Adafruit_BMP280.h> // https://github.com/adafruit/Adafruit_BMP280_Library

#include "Configurable.h"

namespace esp8266util {

class BMP280Sensor {

public:
  bool begin();

  Adafruit_BMP280 &getBMP280();

  bool update();

  float getTemperature(); // unit is Celsius,°C

  float getPressure(); // unit is Pascal, Pa

  float getAltitude(); // unit in Meter, m

private:
  Adafruit_BMP280 *_bmp280 = NULL;

  float _temperature = NAN;
  float _pressure = NAN;
  float _altitude = NAN;
};
} // namespace esp8266util

#endif // BMP280_SENSOR_H
