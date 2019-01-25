#pragma once

#include <Adafruit_BMP280.h>  // https://github.com/adafruit/Adafruit_BMP280_Library
#include <ArduinoJson.h>      // https://github.com/bblanchon/ArduinoJson

#include "Logging.hpp"
#include "Sensor.hpp"

namespace espUtils {

class BMP280Sensor : public Sensor {
 
 public:
  
  bool begin(uint8_t i2cAddr, uint8_t chipId = BMP280_CHIPID) {
    
    _bmp280 = new Adafruit_BMP280();
    _ready = _bmp280->begin(i2cAddr, chipId);
    return _ready;
  }

  Adafruit_BMP280& getBMP280() {
    return *_bmp280;
  }

  bool update(bool mock) {
    
    if (mock) {
      _temperature = random(180, 310) / 10.0;
      _pressure = random(10000, 12000);
      _altitude = random(100, 120);
      return true;
    }

    if (isReady()) {
      _temperature = _bmp280->readTemperature();  // unit is Celsius, °C
      _pressure = _bmp280->readPressure();        // unit is Pascal (Pa) - https://en.wikipedia.org/wiki/Pascal_(unit)
      _altitude = _bmp280->readAltitude(1013.25); // use standard baseline - https://en.wikipedia.org/wiki/Pressure_altitude
      return true;
    } else {
      _temperature = NAN;
      _pressure = NAN;
      _altitude = NAN;
      return false;
    }
  }

  bool isReady() {
    return _ready;
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

    char device[15];
    int size = getDevice(device);

    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    object["temperature"] = getTemperature();
    object["pressure"] = getPressure();
    object["altitude"] = getApproximateAltitude();
    object["device"] = device;
    serializeJson(object, output);
    return measureJson(object);
  }

 private:
  
  Adafruit_BMP280 *_bmp280 = NULL;
  bool _ready = false;

  float _temperature = NAN;
  float _pressure = NAN;
  float _altitude = NAN;
  const char *_device = NULL;
};
}  // namespace espUtils