#ifndef BME280_SENSOR_H
#define BME280_SENSOR_H

#include <Adafruit_BME280.h> // https://github.com/adafruit/Adafruit_BME280_Library

#include "Configurable.h"

namespace esp8266utils {

class BME280Sensor : public Configurable {

public:
  bool begin(uint8_t i2cAddr = BME280_ADDRESS);

  String getConfigAsJson();

  Adafruit_BME280 &getBME280();

  bool update(bool mock = false);

  float getTemperature(); // unit is Celsius, °C

  float getHumidity(); // unit in percent, %

  float getPressure(); // unit is Pascal, Pa

  float getAltitude(); // unit in Meter, m

  String getValuesAsJson();

private:
  Adafruit_BME280 *_bme280 = NULL;

  float _temperature = NAN;
  float _humidity = NAN;
  float _pressure = NAN;
  float _altitude = NAN;
};
} // namespace esp8266utils

#endif // BME280_SENSOR_H
