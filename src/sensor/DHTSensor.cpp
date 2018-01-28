#include "DHTSensor.h"

namespace esp8266util {

bool DHTSensor::begin(uint8_t pin, uint8_t type) {

  config_t config;
  config.pin = pin;
  config.type = type;
  begin(config);
}

bool DHTSensor::begin(config_t config) {

  _config = config;
  if (!_config.pin) {
    LOG.error(F("Missing pin declaration."));
  }
  if (!_config.type) {
    LOG.error(F("Missing type declaration."));
  }
  _dht = new DHT_Unified(config.pin, config.type);

  return true;
}

bool DHTSensor::begin(JsonObject &json) {

  config_t config;
  config.pin = json["pin"];
  config.type = json["type"];

  return begin(config);
}

DHTSensor::config_t DHTSensor::getConfig() { return _config; }

JsonObject &DHTSensor::getConfigAsJson() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json["pin"] = _config.pin;
  json["type"] = _config.type;

  return json;
}

DHT_Unified &DHTSensor::getDHT() { return *_dht; }

bool DHTSensor::update() {

  bool update = false;
  if (_dht) {
    sensors_event_t event;
    getDHT().temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      LOG.error(F("Error reading temperature"));
      _temperature = NAN;
    } else {
      _temperature = event.temperature;
    }
    getDHT().humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      LOG.error(F("Error reading humidity"));
      _humidity = NAN;
    } else {
      _humidity = event.relative_humidity;
    }
    update = true;
  }

  return update;
}

float DHTSensor::getTemperature() { return _temperature; }

float DHTSensor::getHumidity() { return _humidity; }

JsonObject &DHTSensor::getJsonValue() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json["temperature"] = getTemperature();
  json["humidity"] = getHumidity();

  return json;
}
} // namespace esp8266util
