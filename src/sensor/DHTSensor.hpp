#pragma once

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <DHT.h>          // https://github.com/adafruit/DHT-sensor-library
#include <DHT_U.h>        // https://github.com/adafruit/DHT-sensor-library

#include "Logging.hpp"
#include "Sensor.hpp"

namespace espUtils {

class DHTSensor : public Sensor {
 
 public:
  
  struct config_t {
    uint8_t pin;
    /**
    Valid types:
    DHT11 = 11
    DHT21 = 21
    DHT22 = 22
    **/
    uint8_t type;
  };

  bool begin(uint8_t pin, uint8_t type) {
    
    config_t config;
    config.pin = pin;
    config.type = type;
    return begin(config);
  }

  bool begin(config_t config) {
    
    if (config.pin && config.type) {
      _config = config;
      _dht = new DHT_Unified(config.pin, config.type);
      _ready = true;
    } else {
      if (!config.pin) {
        ERROR_FP(F("Missing pin declaration."));
      }
      if (!config.type) {
        ERROR_FP(F("Missing type declaration."));
      }
    }

    return _ready;
  }

  config_t getConfig() {
    return _config;
  }

  DHT_Unified& getDHT() {
    return *_dht;
  }

  bool update(bool mock) {
    
    if (mock) {
      _temperature = random(180, 310) / 10.0;
      _humidity = random(50, 150) / 10.0;
      return true;
    }

    if (isReady()) {
      sensors_event_t event;
      getDHT().temperature().getEvent(&event);
      if (isnan(event.temperature)) {
        ERROR_FP(F("Error reading temperature"));
        _temperature = NAN;
      } else {
        _temperature = event.temperature;
      }
      getDHT().humidity().getEvent(&event);
      if (isnan(event.relative_humidity)) {
        ERROR_FP(F("Error reading humidity"));
        _humidity = NAN;
      } else {
        _humidity = event.relative_humidity;
      }
      return true;
    } else {
      _temperature = NAN;
      _humidity = NAN;
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

  size_t serialize(String& output) {

    char device[15];
    int size = getDevice(device);

    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    object["temperature"] = getTemperature();
    object["humidity"] = getHumidity();
    object["device"] = device;
    serializeJson(object, output);
    return measureJson(object);
  }

 private:
  
  DHT_Unified *_dht = NULL;
  config_t _config;
  bool _ready = false;

  float _temperature = NAN;
  float _humidity = NAN;
};
}  // namespace espUtils