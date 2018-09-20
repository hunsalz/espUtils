#include "BMP280Sensor.h"

namespace esp8266utils {

bool BMP280Sensor::begin(uint8_t i2cAddr, uint8_t chipId) {
  _bmp280 = new Adafruit_BMP280();
  return _bmp280->begin(i2cAddr, chipId);
}

String BMP280Sensor::getConfigAsJson() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();

  return esp8266utils::toString(json);
}

Adafruit_BMP280 &BMP280Sensor::getBMP280() {
  return *_bmp280;
}

bool BMP280Sensor::update(bool mock) {
  
  bool update = false;
  if (_bmp280 && !mock) {
    _temperature = _bmp280->readTemperature();  // unit is Celsius, °C
    _pressure = _bmp280->readPressure();  // unit is Pascal (Pa) - // https://en.wikipedia.org/wiki/Pascal_(unit)
    _altitude = _bmp280->readAltitude(1013.25);  // use standard baseline - // https://en.wikipedia.org/wiki/Pressure_altitude
    update = true;
  } else {
    _temperature = random(180, 310) / 10.0;
    _pressure = random(10000, 12000);
    _altitude = random(100, 120);
    update = true;
  }

  return update;
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

String BMP280Sensor::getValuesAsJson() {
  
  const size_t bufferSize = JSON_OBJECT_SIZE(3);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject &json = jsonBuffer.createObject();
  json["temperature"] = getTemperature();
  json["pressure"] = getPressure();
  json["altitude"] = getApproximateAltitude();
  
  return esp8266utils::toString(json);
}
}  // namespace esp8266utils
