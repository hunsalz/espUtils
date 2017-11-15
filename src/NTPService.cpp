#include "NTPService.h"

namespace esp8266util {

  NTPService::~NTPService() {
    end();
  }

  bool NTPService::isRunning() {
    return _running;
  }

  bool NTPService::begin(String ntpServerName, int timeOffset, bool daylight) {
    return NTP.begin(ntpServerName, timeOffset, daylight);
  }

  bool NTPService::end() {
    
    _running = false;
    
    return NTP.stop();
  }

  NTPClient& NTPService::getNTPClient() {

    _running = true;

    return NTP;
  }

  JsonObject& NTPService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json[F("ntpServerName")] = NTP.getNtpServerName();
    json[F("timeZone")] = NTP.getTimeZone();
    json[F("shortInterval")] = NTP.getShortInterval();
    json[F("longInterval")] = NTP.getLongInterval();
    json[F("dayLight")] = NTP.getDayLight();
    json[F("time")] = NTP.getTimeStr();
    json[F("date")] = NTP.getDateStr();
    json[F("timedate")] = NTP.getTimeDateString();
    json[F("firstSync")] = NTP.getFirstSync();
    json[F("lastSync")] = NTP.getLastNTPSync();
    json[F("uptime")] = NTP.getUptimeString();
    json[F("lastBootTime")] = NTP.getLastBootTime();

    return json;
  }

  NTPService NTP_SERVICE = NTPService();
}