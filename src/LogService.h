/*
  LogService enables a rolling log file that's limited by file size in bytes.

  File constraints:
  - 1st line determines offset value of current log line. If not set DEFAULT_OFFSET is assumed
  - 2nd line determines line length of each log entry. LogService support only one sector, so that every line claims the same amount of characters.
*/

#pragma once

#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <StreamString.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266

namespace esp8266util {

  class LogService {

    public:

      LogService(String path);
      ~LogService();

      String getPath();

      uint16_t getOffset();

      uint16_t getLineLength();

      uint16_t getMaxFileSize();

      void setMaxFileSize(uint16_t bytes);

      void write(char* buffer, bool verbose = false);

      void write(String str, bool verbose = false);

      StreamString getLog();

    private:

      String path;
      File file;
      uint16_t offset;
      uint16_t lineLength;
      uint16_t maxBytes = 512; // default size 4KBytes

      const static int DEFAULT_OFFSET = 11; // 10 digits and CR as default value

      uint16_t readOffset();

      void writeOffset(uint16_t offset);

      uint16_t readLineLength();

      File getFile();

      void init();
  };
}
