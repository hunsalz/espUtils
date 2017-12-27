#include "MQ135Service.h"

namespace esp8266util {

  bool MQ135Service::begin(uint8_t pin) {

    config_t config;
    config.pin = pin;
    begin(config);
  }

  bool MQ135Service::begin(config_t config) {
    
    _config = config;
    if (!_config.pin) {
      LOG.error(F("Missing pin declaration."));
    }
    _mq135 = new MQ135(config.pin);

    return true;
  }

  bool MQ135Service::begin(JsonObject& json) {
    
    config_t config;
    config.pin = json["pin"];
    
    return begin(config);
  }

  MQ135Service::config_t MQ135Service::getConfig() {
    return _config;
  }

  JsonObject& MQ135Service::getConfigAsJson() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["pin"] = _config.pin;

    return json;
  }

  MQ135& MQ135Service::getMQ135() {
    return *_mq135;
  }

  bool MQ135Service::update() {

    bool update = false;
    if (_mq135) {
      int value = analogRead(_config.pin);
      //LOG.verbose(F("Raw analog data = %d"), value);
      //float rzero = _mq135->getRZero(); // the specific resistance at atmospheric CO2 level of your sensor
      //LOG.verbose(F("RZero = %D"), rzero);
      _ppm = _mq135->getPPM(); // parts per million - https://en.wikipedia.org/wiki/Carbon_dioxide_in_Earth%27s_atmosphere
      update = true;
    }

    return update;
  }

  float MQ135Service::getPPM() {
    return _ppm;
  }

  float MQ135Service::getCO2() {
    return _ppm / 100;
  }
}
