#ifndef NTPSERVICE_H
#define NTPSERVICE_H

#include <Log4Esp.h>     // https://github.com/hunsalz/log4Esp
#include <NTPClient.h>   // https://github.com/stelgenhof/NTPClient
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

using log4Esp::LOG;

namespace esp8266util
{

class NTPService : public Service
{

public:
  struct config_t
  {
    const char *server;
    tz_utc_offsets_t utc_offset;
    unsigned int interval;
  };

  ~NTPService();

  bool available();

  bool begin(const char *server = DEFAULT_NTP_SERVER, tz_utc_offsets_t utc_offset = UTC, unsigned int interval = (30 * 60));

  bool begin(config_t config);

  bool end();

  config_t getConfig();

  NTPClient &getNTPClient();

  JsonObject &getDetails();

private:
  config_t _config;
  bool _available = false;
};

extern NTPService NTP_SERVICE;
}

#endif // NTPSERVICE_H