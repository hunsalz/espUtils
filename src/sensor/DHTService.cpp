#include "DHTService.h"

namespace esp8266util {

  bool DHTService::begin(uint8_t pin, uint8_t type) {

    config_t config;
    config.pin = pin;
    config.type = type;
    begin(config);
  }

  bool DHTService::begin(config_t config) {
    
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

  bool DHTService::begin(JsonObject& json) {
    
    config_t config;
    config.pin = json["pin"];
    config.type = json["type"];
    
    return begin(config);
  }

  DHTService::config_t DHTService::getConfig() {
    return _config;
  }

  JsonObject& DHTService::getConfigAsJson() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["pin"] = _config.pin;
    json["type"] = _config.type; 

    return json;
  }

  DHT_Unified& DHTService::getDHT() {
    return *_dht;
  }

  bool DHTService::update() {

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

  float DHTService::getTemperature() {
    return _temperature;
  }

  float DHTService::getHumidity() {
    return _humidity;
  }

  JsonArray& DHTService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonArray& json = jsonBuffer.createArray();
    if (_dht) {
      sensor_t sensor;
      // map temperature sensor values
      getDHT().temperature().getSensor(&sensor);
      JsonObject& temperature = json.createNestedObject().createNestedObject(F("temperature"));
      temperature[F("name")] = String(sensor.name);
      temperature[F("version")] = sensor.version;
      temperature[F("id")] = sensor.sensor_id;
      temperature[F("min")] = sensor.max_value;
      temperature[F("max")] = sensor.min_value;
      temperature[F("resolution")] = sensor.resolution;
      // map humidity sensor values
      getDHT().humidity().getSensor(&sensor);
      JsonObject& humidity = json.createNestedObject().createNestedObject(F("humidity"));
      humidity[F("name")] = String(sensor.name);
      humidity[F("version")] = sensor.version;
      humidity[F("id")] = sensor.sensor_id;
      humidity[F("min")] = sensor.max_value;
      humidity[F("max")] = sensor.min_value;
      humidity[F("resolution")] = sensor.resolution;
    }

    return json;
  }
}
