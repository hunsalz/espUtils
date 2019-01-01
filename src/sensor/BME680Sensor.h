#ifndef BME680_SENSOR_H
#define BME680_SENSOR_H

#include <Adafruit_BME680.h>  // https://github.com/adafruit/Adafruit_BME680
#include <ArduinoJson.h>      // https://github.com/bblanchon/ArduinoJson

#include "Logging.hpp"
#include "polyfills/Json2String.h"
#include "Sensor.hpp"

namespace esp8266utils {

class BME680Sensor : public Sensor {

public:
  bool begin(uint8_t i2cAddr = BME680_DEFAULT_ADDRESS, const char *device = "DefaultDevice");

  Adafruit_BME680 &getBME680();

  bool update(bool mock = false);

  float getTemperature(); // unit is Celsius, °C

  float getHumidity(); // unit in percent, %

  float getPressure(); // unit is Pascal, Pa

  uint32_t getGasResistance(); // unit is Ohm

  float getApproximateAltitude(); // unit in Meter, m

  const char *getDevice();

  String getValuesAsJson();

private:
  Adafruit_BME680 *_bme680 = NULL;

  float _temperature = NAN;
  float _humidity = NAN;
  float _pressure = NAN;
  uint32_t _gas = 0;
  float _altitude = NAN;
  const char *_device = NULL;
};
} // namespace esp8266utils

#endif // BME680_SENSOR_H
