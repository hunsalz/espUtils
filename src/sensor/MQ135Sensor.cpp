#include "MQ135Sensor.h"

namespace esp8266util {

bool MQ135Sensor::begin(uint8_t pin) {
  
  config_t config;
  config.pin = pin;
  begin(config);
}

bool MQ135Sensor::begin(config_t config) {
  
  _config = config;
  if (!_config.pin) {
    LOG.error(F("Missing pin declaration."));
  }
  _mq135 = new MQ135(config.pin);

  return true;
}

bool MQ135Sensor::begin(JsonObject &json) {
  
  config_t config;
  config.pin = json["pin"];

  return begin(config);
}

MQ135Sensor::config_t MQ135Sensor::getConfig() { return _config; }

JsonObject &MQ135Sensor::getConfigAsJson() {
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json["pin"] = _config.pin;

  return json;
}

MQ135 &MQ135Sensor::getMQ135() { return *_mq135; }

bool MQ135Sensor::update(bool mock) {
  
  bool update = false;
  if (_mq135 && !mock) {
    int value = analogRead(_config.pin);
    // LOG.verbose(F("Raw analog data = %d"), value);
    // float rzero = _mq135->getRZero(); // the specific resistance at atmospheric CO2 level of your sensor
    // LOG.verbose(F("RZero = %D"), rzero);
    _ppm = _mq135->getPPM(); // parts per million - https://en.wikipedia.org/wiki/Carbon_dioxide_in_Earth%27s_atmosphere
    update = true;
  } else {
    _ppm = random(5000, 6000) / 10.0;
    update = true;
  }

  return update;
}

float MQ135Sensor::getPPM() { return _ppm; }

float MQ135Sensor::getCO2() { return _ppm / 100; }

JsonObject &MQ135Sensor::getJsonValue() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json["ppm"] = getPPM();
  json["co2"] = getCO2();

  return json;
}
} // namespace esp8266util
