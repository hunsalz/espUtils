#include "BMP280Sensor.h"

namespace esp8266utils {

bool BMP280Sensor::begin(uint8_t i2cAddr, uint8_t chipId, const char *device) {
  
  _device = device;
  _bmp280 = new Adafruit_BMP280();
  return _bmp280->begin(i2cAddr, chipId);
}

Adafruit_BMP280 &BMP280Sensor::getBMP280() {
  return *_bmp280;
}

bool BMP280Sensor::update(bool mock) {
  
  if (_bmp280 && !mock) {
    _temperature = _bmp280->readTemperature();  // unit is Celsius, °C
    _pressure = _bmp280->readPressure();  // unit is Pascal (Pa) - // https://en.wikipedia.org/wiki/Pascal_(unit)
    _altitude = _bmp280->readAltitude(1013.25);  // use standard baseline - // https://en.wikipedia.org/wiki/Pressure_altitude
    return true;
  } else {
    _temperature = random(180, 310) / 10.0;
    _pressure = random(10000, 12000);
    _altitude = random(100, 120);
    return true;
  }
}

float BMP280Sensor::getTemperature() {
  return _temperature;
}

float BMP280Sensor::getPressure() {
  return _pressure;
}

float BMP280Sensor::getApproximateAltitude() {
  return _altitude;
}

const char *BMP280Sensor::getDevice() {
  return _device;
}

String BMP280Sensor::getValuesAsJson() {
  
  DynamicJsonDocument doc;
  JsonObject object = doc.to<JsonObject>();
  object["temperature"] = getTemperature();
  object["pressure"] = getPressure();
  object["altitude"] = getApproximateAltitude();
  object["device"] = getDevice();
  
  return esp8266utils::toString(object);
}
}  // namespace esp8266utils
