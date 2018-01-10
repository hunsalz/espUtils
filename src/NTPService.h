#ifndef NTPSERVICE_H
#define NTPSERVICE_H

#include <Log4Esp.h> // https://github.com/hunsalz/log4Esp
#include <NtpClientLib.h> // https://github.com/gmag11/NtpClient
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

using log4Esp::LOG;

namespace esp8266util {

  class NTPService : public Service {

    public:

      struct config_t {
        const char* ntpServerName;
        int timeZone;
        bool daylight;
        int shortInterval;
        int longInterval;
      };

      ~NTPService();

      bool available();

      bool begin(const char* ntpServerName = DEFAULT_NTP_SERVER, int timeZone = DEFAULT_NTP_TIMEZONE, bool daylight = false, int shortInterval = 30, int longInterval = 300);

      bool begin(config_t config);

      bool end();

      config_t getConfig();

      NTPClient& getNTPClient();

      JsonObject& getDetails();

    private:

      config_t _config;
      bool _available = false;
  };

  extern NTPService NTP_SERVICE;
}

#endif // NTPSERVICE_H