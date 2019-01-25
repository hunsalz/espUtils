#pragma once

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <MQ135.h>  // https://github.com/GeorgK/MQ135 - Set up sensor for the first time? https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

#include "Logging.hpp"
#include "Sensor.hpp"

namespace espUtils {

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
    
    if (_config.pin) {
      _config = config;
      _mq135 = new MQ135(config.pin);
    } else {
      ERROR_FP(F("Missing pin declaration."));
    }

    return true;
  }

  config_t getConfig() {
    return _config;
  }

  MQ135& getMQ135() {
    return *_mq135;
  }

  bool update(bool mock) {

    if (mock) {
      _ppm = random(5000, 6000) / 10.0;
      return true;
    }

    if (isReady()) {
      int value = analogRead(getConfig().pin);
      // VERBOSE_FP(F("Raw analog data = %d"), value);
      // float rzero = _mq135->getRZero(); // the specific resistance at
      // atmospheric CO2 level of your sensor VERBOSE_FP(F("RZero = %D"), rzero);
      _ppm = _mq135->getPPM();  // parts per million - https://en.wikipedia.org/wiki/Carbon_dioxide_in_Earth%27s_atmosphere
      return true;
    } else {
      _ppm = NAN;
      return false;
    }
  }

  bool isReady() {
    return _ready;
  }

  float getPPM() {
    return _ppm;
  }

  size_t serialize(String& output) {

    char device[15];
    int size = getDevice(device);

    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    object["ppm"] = getPPM();
    object["device"] = device;
    serializeJson(object, output);
    return measureJson(object);
  }

 private:
  
  MQ135 *_mq135 = NULL;
  config_t _config;
  bool _ready = false;

  float _ppm = NAN;
};
}  // namespace espUtils
