#include "DHTSensor.h"

namespace esp8266utils {

bool DHTSensor::begin(uint8_t pin, uint8_t type) {

  config_t config;
  config.pin = pin;
  config.type = type;
  return begin(config);
}

bool DHTSensor::begin(config_t config) {

  _config = config;
  if (!_config.pin) {
    ERROR_MSG_P(F("Missing pin declaration."));
  }
  if (!_config.type) {
    ERROR_MSG_P(F("Missing type declaration."));
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

String DHTSensor::getConfigAsJson() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json["pin"] = _config.pin;
  json["type"] = _config.type;

  return esp8266utils::toString(json);
}

DHT_Unified &DHTSensor::getDHT() { return *_dht; }

bool DHTSensor::update(bool mock) {

  if (_dht && !mock) {
    sensors_event_t event;
    getDHT().temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      ERROR_MSG_P(F("Error reading temperature"));
      _temperature = 999; // FIXME -> NAN
    } else {
      _temperature = event.temperature;
    }
    getDHT().humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      ERROR_MSG_P(F("Error reading humidity"));
      _humidity = 999; // FIXME -> NAN
    } else {
      _humidity = event.relative_humidity;
    }
    return true;
  } else {
    _temperature = random(180, 310) / 10.0;
    _humidity = random(50, 150) / 10.0;
    return true;
  }
}

float DHTSensor::getTemperature() { return _temperature; }

float DHTSensor::getHumidity() { return _humidity; }

String DHTSensor::getValuesAsJson() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json["temperature"] = getTemperature();
  json["humidity"] = getHumidity();

  // TODO remove debugging
  //return String("{\"temperature\": ") + getTemperature() + String(",\"humidity\":") + getHumidity() + String("}");

  return esp8266utils::toString(json);
}
} // namespace esp8266utils
