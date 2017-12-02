#include "MQTTService.h"

namespace esp8266util {

  MQTTService::~MQTTService() {
    end();
  }

  bool MQTTService::available() {
    return getMqttClient().connected();
  }

  bool MQTTService::begin(const char* hostName, uint16_t port) {
    
    config_t config;
    config.hostName = hostName;
    config.port = port;
    begin(config);
  }

  bool MQTTService::begin(config_t config) {
    
    // add verbose callback handlers
    _mqttClient.onConnect([this](bool sessionPresent) {
      Log.verbose(F("Connected to MQTT broker [%s:%d]" CR), getConfig().hostName, getConfig().port);
    });
    _mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason) {
      Log.verbose(F("Disconnected from MQTT broker [%s:%d]" CR), getConfig().hostName, getConfig().port);
    });
    _mqttClient.onSubscribe([](uint16_t packetId, uint8_t qos) {
      Log.verbose(F("Subscribe acknowledged with packetId [%d]" CR), packetId);
    });
    _mqttClient.onUnsubscribe([](uint16_t packetId) {
      Log.verbose(F("Unsubscribe acknowledged with packetId [%d]" CR), packetId);
    });
    _mqttClient.onMessage([](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
      Log.verbose(F("Message received for topic [%s]: %s" CR), topic, payload);
    });
    _mqttClient.onPublish([](uint16_t packetId) {
      Log.verbose(F("Publish acknowledged with packetId [%d]" CR), packetId);
    });
    // configure mqtt client
    _mqttClient.setServer(config.hostName, config.port);

    // TODO
    // mqttClient.setCredentials(MQTT_USERNAME, MQTT_PASSWD);
    // mqttClient.setClientId(MQTT_CLIENT);
    // mqttClient.setKeepAlive(15); // in seconds
    // mqttClient.setCleanSession(true);
    // try to connect
    _mqttClient.connect();

    return available();
  }

  MQTTService::config_t MQTTService::getConfig() {
    return _config;
  }

  bool MQTTService::end() {

    if (available()) {
      _mqttClient.disconnect();
    }

    return available();
  }

  AsyncMqttClient& MQTTService::getMqttClient() {
    return _mqttClient;
  }

  // TODO reduce duplicate code
  void MQTTService::publish(const char* topic, JsonObject& json) {

    // convert JSON into char array payload
    int length = json.measureLength() + 1;
    char payload[length];
    json.printTo(payload, length);

    // TODO check publish
    _mqttClient.publish(topic, 0, true, payload, length);
    Log.verbose(F("Message send: %s" CR), payload);
  }

  void MQTTService::publish(const char* topic, JsonArray& json) {

    // convert JSON into char array payload
    int length = json.measureLength() + 1;
    char payload[length];
    json.printTo(payload, length);

    // TODO check publish
    _mqttClient.publish(topic, 0, true, payload, length);
    Log.verbose(F("Message send: %s" CR), payload);
  }
}
