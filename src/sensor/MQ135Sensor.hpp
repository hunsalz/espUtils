#pragma once

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <MQ135.h>  // https://github.com/GeorgK/MQ135 - Set up sensor for the first time? https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

#include "Logging.hpp"
#include "Sensor.hpp"
#include "polyfills/Json2String.h"

namespace esp8266utils {

class MQ135Sensor : public Sensor {
 
 public:
  
  struct config_t {
    uint8_t pin;
  };

  bool begin(uint8_t pin) {
    
    config_t config;
    config.pin = pin;
    return begin(config);
  }

  bool begin(config_t config) {
    
    _config = config;
    if (!_config.pin) {
      ERROR_MSG_P(F("Missing pin declaration."));
    }
    _mq135 = new MQ135(config.pin);

    return true;
  }

  bool begin(JsonObject &object) {
    
    config_t config;
    config.pin = object["pin"];

    return begin(config);
  }

  config_t getConfig() {
    return _config;
  }

  MQ135& getMQ135() {
    return *_mq135;
  }

  bool update(bool mock) {
    
    if (_mq135 && !mock) {
      int value = analogRead(_config.pin);
      // VERBOSE_MSG_P(F("Raw analog data = %d"), value);
      // float rzero = _mq135->getRZero(); // the specific resistance at
      // atmospheric CO2 level of your sensor VERBOSE_MSG_P(F("RZero = %D"),
      // rzero);
      _ppm = _mq135->getPPM();  // parts per million - https://en.wikipedia.org/wiki/Carbon_dioxide_in_Earth%27s_atmosphere
      return true;
    } else {
      _ppm = random(5000, 6000) / 10.0;
      return true;
    }
  }

  float getPPM() {
    return _ppm;
  }

  String getValuesAsJson() {
    
    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    object["ppm"] = getPPM();
    return esp8266utils::toString(object);
  }

 private:
  
  MQ135 *_mq135 = NULL;
  config_t _config;

  float _ppm = NAN;
};
}  // namespace esp8266utils
