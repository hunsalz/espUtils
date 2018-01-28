#include "BMP280Sensor.h"

namespace esp8266util {

bool BMP280Sensor::begin() {
  
  _bmp280 = new Adafruit_BMP280();
  _bmp280->begin();
}

Adafruit_BMP280 &BMP280Sensor::getBMP280() { return *_bmp280; }

bool BMP280Sensor::update() {
  
  bool update = false;
  if (_bmp280) {
    _temperature = _bmp280->readTemperature();  // unit is Celsius
    _pressure = _bmp280->readPressure();        // unit is Pascal (Pa) - https://en.wikipedia.org/wiki/Pascal_(unit)
    _altitude = _bmp280->readAltitude(1013.25); // use standard baseline - https://en.wikipedia.org/wiki/Pressure_altitude

    update = true;
  }

  return update;
}

float BMP280Sensor::getTemperature() { return _temperature; }

float BMP280Sensor::getPressure() { return _pressure; }

float BMP280Sensor::getAltitude() { return _altitude; }
} // namespace esp8266util
