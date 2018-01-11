#include "NTPService.h"

namespace esp8266util
{

NTPService::~NTPService()
{
  end();
}

bool NTPService::available()
{
  return _available;
}

bool NTPService::begin(const char *server, tz_utc_offsets_t utc_offset, unsigned int interval)
{
  config_t config;
  config.server = server;
  config.utc_offset = utc_offset;
  config.interval = interval;

  return begin(config);
}

bool NTPService::begin(config_t config)
{
  // set NTP sync interval
  if (!config.interval)
  {
    config.interval = (30 * 60);
  }
  NTP.setPollingInterval(config.interval);
  // set ntp server
  if (!config.server)
  {
    config.server = DEFAULT_NTP_SERVER;
  }
  // set a default event handler
  NTP.onSyncEvent([this](NTPSyncEvent_t ntpEvent) {
    switch (ntpEvent)
    {
    case NTP_EVENT_INIT:
      _available = false;
      break;
    case NTP_EVENT_STOP:
      _available = false;
      break;
    case NTP_EVENT_NO_RESPONSE:
      _available = false;
      LOG.error(F("NTP server [%s] not reachable."), getConfig().server);
      break;
    case NTP_EVENT_SYNCHRONIZED:
      _available = true;
      LOG.verbose(F("NTP sync with %s was successful: %s"), getConfig().server, NTP.getTimeDate(NTP.getLastSync()));
      break;
    }
  });
  // set utc offset
  if (!config.utc_offset)
  {
    config.utc_offset = UTC;
  }
  _config = config;
  // finally start NTP service
  NTP.init((char *)config.server, config.utc_offset);
}

bool NTPService::end()
{
  _available = false;

  return NTP.stop();
}

NTPService::config_t NTPService::getConfig()
{
  return _config;
}

NTPClient &NTPService::getNTPClient()
{
  return NTP;
}

JsonObject &NTPService::getDetails()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("server")] = NTP.getNTPServer();
  json[F("utcOffset")] = _config.utc_offset;
  json[F("interval")] = NTP.getPollingInterval();
  json[F("lastSync")] = NTP.getLastSync();
  json[F("firstSync")] = NTP.getFirstSync();
  json[F("timedate")] = NTP.getTimeDate(now());

  return json;
}

NTPService NTP_SERVICE = NTPService();
}