#pragma once

#include <Adafruit_BME280.h>  // https://github.com/adafruit/Adafruit_BME280_Library
#include <ArduinoJson.h>      // https://github.com/bblanchon/ArduinoJson

#include "Logging.hpp"
#include "Sensor.hpp"

namespace espUtils {

class BME280Sensor : public Sensor {
 
 public:

  bool begin(uint8_t i2cAddr) {
    
    _bme280 = new Adafruit_BME280();
    _ready = _bme280->begin(i2cAddr);
    return _ready;
  }

  Adafruit_BME280& getBME280() {
    return *_bme280;
  }

  bool update(bool mock) {
    
    if (mock) {
      _temperature = random(180, 310) / 10.0;
      _humidity = random(50, 150) / 10.0;
      _pressure = random(10000, 12000);
      _altitude = random(100, 120);
      return true;
    }

    if (isReady()) {
      _temperature = _bme280->readTemperature();  // unit is Celsius, °C
      _humidity = _bme280->readHumidity();        // unit in percent, %
      _pressure = _bme280->readPressure();        // unit is Pascal (Pa) - https://en.wikipedia.org/wiki/Pascal_(unit)
      _altitude = _bme280->readAltitude(1013.25); // use standard baseline - https://en.wikipedia.org/wiki/Pressure_altitude
      return true;
    } else {
      _temperature = NAN;
      _humidity = NAN;
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

  float getHumidity() {
    return _humidity;
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
    object["humidity"] = getHumidity();
    object["pressure"] = getPressure();
    object["altitude"] = getApproximateAltitude();
    object["device"] = device;
    serializeJson(object, output);
    return measureJson(object);
  }

 private:
  
  Adafruit_BME280 *_bme280 = NULL;
  bool _ready = false;
  
  float _temperature = NAN;
  float _humidity = NAN;
  float _pressure = NAN;
  float _altitude = NAN;
};
}  // namespace espUtils