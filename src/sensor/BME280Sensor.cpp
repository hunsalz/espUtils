#include "BME280Sensor.h"

namespace esp8266utils {

bool BME280Sensor::begin(uint8_t i2cAddr, const char *device) {
  
  _device = device;
  _bme280 = new Adafruit_BME280();
  return _bme280->begin(i2cAddr);
}

String BME280Sensor::getConfigAsJson() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();

  return esp8266utils::toString(json);
}

Adafruit_BME280 &BME280Sensor::getBME280() {
  return *_bme280;
}

bool BME280Sensor::update(bool mock) {
  
  if (_bme280 && !mock) {
    _temperature = _bme280->readTemperature();  // unit is Celsius, °C
    _humidity = _bme280->readHumidity();  // unit in percent, %
    _pressure = _bme280->readPressure();  // unit is Pascal (Pa) - // https://en.wikipedia.org/wiki/Pascal_(unit)
    _altitude = _bme280->readAltitude(1013.25);  // use standard baseline - // https://en.wikipedia.org/wiki/Pressure_altitude
    return true;
  } else {
    _temperature = random(180, 310) / 10.0;
    _humidity = random(50, 150) / 10.0;
    _pressure = random(10000, 12000);
    _altitude = random(100, 120);
    return true;
  }
}

float BME280Sensor::getTemperature() {
  return _temperature;
}

float BME280Sensor::getHumidity() {
  return _humidity;
}

float BME280Sensor::getPressure() {
  return _pressure;
}

float BME280Sensor::getApproximateAltitude() {
  return _altitude;
}

const char *BME280Sensor::getDevice() {
  return _device;
}

String BME280Sensor::getValuesAsJson() {
  
  const size_t bufferSize = JSON_OBJECT_SIZE(3);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject &json = jsonBuffer.createObject();
  json["temperature"] = getTemperature();
  json["humidity"] = getHumidity();
  json["pressure"] = getPressure();
  json["altitude"] = getApproximateAltitude();
  json["device"] = getDevice();
  
  return esp8266utils::toString(json);
}
}  // namespace esp8266utils
