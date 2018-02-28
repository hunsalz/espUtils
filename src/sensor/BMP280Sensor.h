#ifndef BMP280_SENSOR_H
#define BMP280_SENSOR_H

#include <Adafruit_BMP280.h> // https://github.com/adafruit/Adafruit_BMP280_Library

#include "Configurable.h"

namespace esp8266utils {

class BMP280Sensor : public Configurable {

public:
  bool begin();

  const char* getConfigAsJson();

  Adafruit_BMP280 &getBMP280();

  bool update(bool mock = false);

  float getTemperature(); // unit is Celsius,°C

  float getPressure(); // unit is Pascal, Pa

  float getAltitude(); // unit in Meter, m

  const char* getValuesAsJson();

private:
  Adafruit_BMP280 *_bmp280 = NULL;

  float _temperature = NAN;
  float _pressure = NAN;
  float _altitude = NAN;
};
} // namespace esp8266utils

#endif // BMP280_SENSOR_H
