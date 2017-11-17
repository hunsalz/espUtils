#include "DHTService.h"

namespace esp8266util {

  bool DHTService::begin(DHTService::config_t config) {
    
    _config = config;
    if (!_config.pin) {
      Log.error(F("Missing pin declaration." CR));
    }
    if (!_config.type) {
      Log.error(F("Missing type declaration." CR));
    }
    
    _dht = new DHT_Unified(config.pin, config.type);

    return true;
  }

  bool DHTService::begin(JsonObject& json) {
    
    DHTService::config_t config;
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
        Log.error(F("Error reading temperature" CR));
        _temperature = NAN;
      } else {
        _temperature = event.temperature;
      }
      getDHT().humidity().getEvent(&event);
      if (isnan(event.relative_humidity)) {
        Log.error(F("Error reading humidity" CR));
        _humidity = NAN;
      } else {
        _humidity = event.relative_humidity;
      }
      update = true;
    }

    return update;
  }

  float DHTService::getTemperature() {

    // TODO
    char out[10];
    dtostrf(_temperature, 1, 2, out);
    Serial.printf("---------------------------------Temp :: %s\n", out);

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
