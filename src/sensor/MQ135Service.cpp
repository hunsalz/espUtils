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
      Log.error(F("Missing pin declaration." CR));
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
      float rzero = _mq135->getRZero(); // the specific resistance at atmospheric CO2 level of your sensor
      //float resistance = _mq135.getResistance();
      _ppm = _mq135->getPPM(); // parts per million - https://en.wikipedia.org/wiki/Carbon_dioxide_in_Earth%27s_atmosphere
      String co2 = "0.0" + String(_ppm * 100, 0); // calculating CO2 in % assuming that the sensor detects mostly CO2 in normal atmosphere
      
      //Log.verbose(F("Raw analog data = %d" CR), value);
      //Log.verbose(F("RZero = %D" CR), rzero);
      Log.verbose(F("PPM = %D" CR), _ppm);
      Log.verbose(F("CO2 = %s" CR), co2.c_str());

      update = true;
    }

    return update;
  }

  float MQ135Service::getPPM() {
    return _ppm;
  }

  float MQ135Service::getCo2() {
    return _ppm;
  }

  JsonArray& MQ135Service::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonArray& json = jsonBuffer.createArray();
    if (_mq135) {

      // TODO

    }

    return json;
  }
}
