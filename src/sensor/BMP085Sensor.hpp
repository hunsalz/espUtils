#pragma once

#include <Adafruit_BMP085_U.h>  // https://github.com/adafruit/Adafruit_BMP085_Unified
#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson

#include "Logging.hpp"
#include "Sensor.hpp"

namespace esp8266utils {

class BMP085Sensor : public Sensor {
 
 public:
  
  bool begin(float seaLevelPressure) {
    
    _seaLevelPressure = seaLevelPressure;
    _bmp085 = new Adafruit_BMP085_Unified();
    return _bmp085->begin();
  }

  Adafruit_BMP085_Unified& getBMP085() {
    return *_bmp085;
  }

  bool update(bool mock) {
    
    if (_bmp085 && !mock) {
      sensors_event_t event;
      _bmp085->getEvent(&event);
      _pressure = event.pressure;  // unit is Pascal (Pa) - https://en.wikipedia.org/wiki/Pascal_(unit)
      _bmp085->getTemperature(&_temperature);  // unit is Celsius, °C
      _altitude = _bmp085->pressureToAltitude(getSeaLevelPressure(), event.pressure);  // use standard baseline - https://en.wikipedia.org/wiki/Pressure_altitude
      return true;
    } else {
      _temperature = random(180, 310) / 10.0;
      _pressure = random(10000, 12000);
      _altitude = random(100, 120);
      return true;
    }
  }

  float getSeaLevelPressure() {
    return _seaLevelPressure;
  }

  float getTemperature() {
    return _temperature;
  }

  float getPressure() {
    return _pressure;
  }

  float getApproximateAltitude() {
    return _altitude;
  }

  size_t serialize(String& output) {

    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    object["temperature"] = getTemperature();
    object["pressure"] = getPressure();
    object["altitude"] = getApproximateAltitude();
    object["device"] = "ESP-" + String(ESP.getChipId());
    serializeJson(object, output);
    return measureJson(object);
  }

 private:
  
  Adafruit_BMP085_Unified *_bmp085 = NULL;

  float _seaLevelPressure = 1013.25;
  float _temperature = NAN;
  float _pressure = NAN;
  float _altitude = NAN;
};
}  // namespace esp8266utils
