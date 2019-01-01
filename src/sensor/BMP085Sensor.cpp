#include "BMP085Sensor.h"

namespace esp8266utils {

bool BMP085Sensor::begin(float seaLevelPressure, const char *device) {
  
  _device = device;
  _seaLevelPressure = seaLevelPressure;
  _bmp085 = new Adafruit_BMP085_Unified();
  return _bmp085->begin();
}

Adafruit_BMP085_Unified &BMP085Sensor::getBMP085() {
  return *_bmp085;
}

bool BMP085Sensor::update(bool mock) {
  
  if (_bmp085 && !mock) {
    sensors_event_t event;
    _bmp085->getEvent(&event);
    _pressure = event.pressure; // unit is Pascal (Pa) - // https://en.wikipedia.org/wiki/Pascal_(unit)
    _bmp085->getTemperature(&_temperature); // unit is Celsius, °C
    _altitude = _bmp085->pressureToAltitude(getSeaLevelPressure(), event.pressure); // use standard baseline - // https://en.wikipedia.org/wiki/Pressure_altitude
    return true;
  } else {
    _temperature = random(180, 310) / 10.0;
    _pressure = random(10000, 12000);
    _altitude = random(100, 120);
    return true;
  }
}

float BMP085Sensor::getSeaLevelPressure() {
  return _seaLevelPressure;
}

float BMP085Sensor::getTemperature() {
  return _temperature;
}

float BMP085Sensor::getPressure() {
  return _pressure;
}

float BMP085Sensor::getApproximateAltitude() {
  return _altitude;
}

const char *BMP085Sensor::getDevice() {
  return _device;
}

String BMP085Sensor::getValuesAsJson() {
  
  DynamicJsonDocument doc;
  JsonObject object = doc.to<JsonObject>();
  object["temperature"] = getTemperature();
  object["pressure"] = getPressure();
  object["altitude"] = getApproximateAltitude();
  object["device"] = getDevice();
  
  return esp8266utils::toString(object);
}
}  // namespace esp8266utils
