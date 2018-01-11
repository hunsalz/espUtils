#include "BMP280Service.h"

namespace esp8266util
{

bool BMP280Service::begin()
{
  _bmp280 = new Adafruit_BMP280();
  _bmp280->begin();
}

Adafruit_BMP280 &BMP280Service::getBMP280()
{
  return *_bmp280;
}

bool BMP280Service::update()
{
  bool update = false;
  if (_bmp280)
  {
    _temperature = _bmp280->readTemperature();  // unit is Celsius
    _pressure = _bmp280->readPressure();        // unit is Pascal (Pa) - https://en.wikipedia.org/wiki/Pascal_(unit)
    _altitude = _bmp280->readAltitude(1013.25); // use standard baseline - https://en.wikipedia.org/wiki/Pressure_altitude

    update = true;
  }

  return update;
}

float BMP280Service::getTemperature()
{
  return _temperature;
}

float BMP280Service::getPressure()
{
  return _pressure;
}

float BMP280Service::getAltitude()
{
  return _altitude;
}
}
