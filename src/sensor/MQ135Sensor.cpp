#include "MQ135Sensor.h"

namespace esp8266utils {

bool MQ135Sensor::begin(uint8_t pin) {
  
  config_t config;
  config.pin = pin;
  return begin(config);
}

bool MQ135Sensor::begin(config_t config) {
  
  _config = config;
  if (!_config.pin) {
    ERROR_MSG_P(F("Missing pin declaration."));
  }
  _mq135 = new MQ135(config.pin);

  return true;
}

bool MQ135Sensor::begin(JsonObject &object) {
  
  config_t config;
  config.pin = object["pin"];

  return begin(config);
}

MQ135Sensor::config_t MQ135Sensor::getConfig() { 
  return _config; 
}

MQ135 &MQ135Sensor::getMQ135() { 
  return *_mq135; 
}

bool MQ135Sensor::update(bool mock) {
  
  if (_mq135 && !mock) {
    int value = analogRead(_config.pin);
    // VERBOSE_MSG_P(F("Raw analog data = %d"), value);
    // float rzero = _mq135->getRZero(); // the specific resistance at atmospheric CO2 level of your sensor
    // VERBOSE_MSG_P(F("RZero = %D"), rzero);
    _ppm = _mq135->getPPM(); // parts per million - https://en.wikipedia.org/wiki/Carbon_dioxide_in_Earth%27s_atmosphere
    return true;
  } else {
    _ppm = random(5000, 6000) / 10.0;
    return true;
  }
}

float MQ135Sensor::getPPM() { return _ppm; }

String MQ135Sensor::getValuesAsJson() {

  DynamicJsonDocument doc;
  JsonObject object = doc.to<JsonObject>();
  object["ppm"] = getPPM();

  return esp8266utils::toString(object);
}
} // namespace esp8266utils
