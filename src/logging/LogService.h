/*
  LogService enables a rolling log file that's limited by file size in bytes.

  File constraints:
  - 1st line determines offset value of current log line. If not set DEFAULT_OFFSET is assumed
  - 2nd line determines line length of each log entry. LogService support only one sector, so that every line claims the same amount of characters.
*/

#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h
#include <StreamString.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266

namespace esp8266util {

  class LogService {

    public:

      LogService(String path);
      ~LogService();

      /*
       Return log file path.
       */
      String getPath();

      /*
       Return offset of next log entry.
       */
      uint16_t getOffset();

      /*
        Return fixed line length. Returns 0 if no log entry is available.
       */
      uint16_t getLineLength();

      /*
       Return max file size in bytes.
       */
      uint16_t getMaxFileSize();

      /*
       Set max file size in bytes.
       */
      void setMaxFileSize(uint16_t bytes);

      /*
       Write log entry.
       */
      void write(char* entry, bool verbose = false);

      /*
       Write log entry.
       */
      void write(String entry, bool verbose = false);

      /*
       Return log in chronological order
       */
      StreamString getLog();

    private:

      String _path;
      File _file;
      uint16_t _offset;
      uint16_t _lineLength;
      uint16_t _maxBytes = 512; // default size 4KBytes

      const static int DEFAULT_OFFSET = 11; // 10 digits and CR as default value

      uint16_t readOffset();

      void writeOffset(uint16_t offset);

      uint16_t readLineLength();

      File getFile();

      void init();
  };
}
