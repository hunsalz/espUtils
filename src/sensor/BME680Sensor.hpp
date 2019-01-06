#pragma once

#include <Adafruit_BME680.h>  // https://github.com/adafruit/Adafruit_BME680
#include <ArduinoJson.h>      // https://github.com/bblanchon/ArduinoJson

#include "Logging.hpp"
#include "Sensor.hpp"

namespace esp8266utils {

class BME680Sensor : public Sensor {
 
 public:
  
  bool begin(uint8_t i2cAddr) {
    
    _bme680 = new Adafruit_BME680();
    if (_bme680->begin(i2cAddr)) {
      // set up oversampling and filter initialization
      _bme680->setTemperatureOversampling(BME680_OS_8X);
      _bme680->setHumidityOversampling(BME680_OS_2X);
      _bme680->setPressureOversampling(BME680_OS_4X);
      _bme680->setIIRFilterSize(BME680_FILTER_SIZE_3);
      _bme680->setGasHeater(320, 150);  // 320*C for 150 ms
      return true;
    } else {
      return false;
    }
  }

  Adafruit_BME680& getBME680() {
    return *_bme680;
  }

  bool update(bool mock) {
    
    if (_bme680 && !mock) {
      if (!_bme680->performReading()) {
        ERROR_FP(F("Perform reading BMP680 values failed."));
        return false;
      } else {
        _temperature = _bme680->readTemperature();  // unit is Celsius, °C
        _humidity = _bme680->readHumidity();        // unit in percent, %
        _pressure = _bme680->readPressure();        // unit is Pascal (Pa) - https://en.wikipedia.org/wiki/Pascal_(unit)
        _gas = _bme680->readGas();                  // unit is Ohm, Ω
        _altitude = _bme680->readAltitude(1013.25); // use standard baseline - https://en.wikipedia.org/wiki/Pressure_altitude
        return true;
      }
    } else {
      _temperature = random(180, 310) / 10.0;
      _humidity = random(50, 150) / 10.0;
      _pressure = random(10000, 12000);
      _gas = random(5000, 12000);
      _altitude = random(100, 120);
      return true;
    }
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

  uint32_t getGasResistance() {
    return _gas;
  }

  float getApproximateAltitude() {
    return _altitude;
  }

  size_t serialize(String& output) {

    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    object["temperature"] = getTemperature();
    object["humidity"] = getHumidity();
    object["pressure"] = getPressure();
    object["gas"] = getGasResistance();
    object["altitude"] = getApproximateAltitude();
    object["device"] = getDeviceName();
    serializeJson(object, output);
    return measureJson(object);
  }

 private:
  
  Adafruit_BME680 *_bme680 = NULL;

  float _temperature = NAN;
  float _humidity = NAN;
  float _pressure = NAN;
  uint32_t _gas = 0;
  float _altitude = NAN;
};
}  // namespace esp8266utils