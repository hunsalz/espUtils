#ifndef BMP280_SENSOR_H
#define BMP280_SENSOR_H

#include <Adafruit_BMP280.h>  // https://github.com/adafruit/Adafruit_BMP280_Library
#include <ArduinoJson.h>      // https://github.com/bblanchon/ArduinoJson

#include "Logging.hpp"
#include "polyfills/Json2String.h"
#include "Sensor.hpp"

namespace esp8266utils {

class BMP280Sensor : public Sensor {

public:
  bool begin(uint8_t i2cAddr = BMP280_ADDRESS, uint8_t chipId = BMP280_CHIPID, const char *device = "DefaultDevice");

  Adafruit_BMP280 &getBMP280();

  bool update(bool mock = false);

  float getTemperature(); // unit is Celsius, °C

  float getPressure(); // unit is Pascal, Pa

  float getApproximateAltitude(); // unit in Meter, m

  const char *getDevice();

  String getValuesAsJson();

private:
  Adafruit_BMP280 *_bmp280 = NULL;

  float _temperature = NAN;
  float _pressure = NAN;
  float _altitude = NAN;
  const char *_device = NULL;
};
} // namespace esp8266utils

#endif // BMP280_SENSOR_H
