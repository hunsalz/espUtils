#include "NTPService.h"

namespace esp8266util {

  NTPService::~NTPService() {
    end();
  }

  bool NTPService::available() {
    return _available;
  }

  bool NTPService::begin(const char* ntpServerName, int timeZone, bool daylight, int shortInterval, int longInterval) {
    
    config_t config;
    config.ntpServerName = ntpServerName;
    config.timeZone = timeZone;
    config.daylight = daylight;
    config.shortInterval = shortInterval;
    config.longInterval = longInterval;
  
    return begin(config);
  }

  bool NTPService::begin(config_t config) {

    // set NTP sync period
    if (!config.shortInterval) {
      config.shortInterval = 30;
    }
    if (!config.longInterval) {
      config.longInterval = 300;
    }
    NTP.setInterval(config.shortInterval, config.longInterval);
    // set ntp server
    if (!config.ntpServerName) {
      config.ntpServerName = "pool.ntp.org";
    }
    // set a default event handler
    NTP.onNTPSyncEvent([this](NTPSyncEvent_t event) {
      if (event) {
        _available = false;
		    if (event == noResponse) {
          LOG.error(F("NTP server [%s] not reachable."), getConfig().ntpServerName);
        } else if (event == invalidAddress) {
          LOG.error(F("NTP server address [%s] is invalid."), getConfig().ntpServerName);
        }
	    } else {
        _available = true;
        LOG.verbose(F("NTP sync with %s was successful: %s"), getConfig().ntpServerName, NTP.getTimeDateString(NTP.getLastNTPSync()).c_str());
	    }
	  });
    // set time zone & daylight settings
    if (!config.timeZone) {
      config.timeZone = 0;
    }
    if (!config.daylight) {
      config.daylight = false;
    }
    _config = config;
    // finally start NTP service
    NTP.begin(config.ntpServerName, config.timeZone, config.daylight);
  }

  bool NTPService::end() {
    
    _available = false;
    
    return NTP.stop();
  }

  NTPService::config_t NTPService::getConfig() {
    return _config;
  }

  NTPClient& NTPService::getNTPClient() {
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