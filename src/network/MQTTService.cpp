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
    
    _config = config;
    // add verbose callback handlers
    _mqttClient.onConnect([this](bool sessionPresent) {
      LOG.verbose(F("Connected to MQTT broker [%s:%d]"), getConfig().hostName, getConfig().port);
    });
    _mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason) {
      LOG.verbose(F("Disconnected from MQTT broker [%s:%d]"), getConfig().hostName, getConfig().port);
    });
    _mqttClient.onSubscribe([](uint16_t packetId, uint8_t qos) {
      LOG.verbose(F("Subscribe acknowledged with packetId [%d]"), packetId);
    });
    _mqttClient.onUnsubscribe([](uint16_t packetId) {
      LOG.verbose(F("Unsubscribe acknowledged with packetId [%d]"), packetId);
    });
    _mqttClient.onMessage([](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
      LOG.verbose(F("Message received for topic [%s]: %s"), topic, payload);
    });
    _mqttClient.onPublish([](uint16_t packetId) {
      LOG.verbose(F("Publish acknowledged with packetId [%d]"), packetId);
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
    LOG.verbose(F("Message send: %s"), payload);
  }

  void MQTTService::publish(const char* topic, JsonArray& json) {

    // convert JSON into char array payload
    int length = json.measureLength() + 1;
    char payload[length];
    json.printTo(payload, length);

    // TODO check publish
    _mqttClient.publish(topic, 0, true, payload, length);
    LOG.verbose(F("Message send: %s"), payload);
  }
}
