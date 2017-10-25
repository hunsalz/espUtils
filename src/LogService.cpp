#include "LogService.h"

namespace esp8266util {

  LogService::LogService(String path) {
    this->path = path;
  }

  LogService::~LogService() {
    getFile().close();
  }

  String LogService::getPath() {
    return path;
  }

  uint16_t LogService::getOffset() {

    // if file is null call init first
    if (file) {
      init();
    }
    // if limit of max bytes is reached, then rollover and override entries from beginning
    if (offset > getMaxFileSize()) {
      offset = DEFAULT_OFFSET;
    }

    return offset;
  }

  uint16_t LogService::getLineLength() {

    // if file is null call init first
    if (file) {
      init();
    }

    return lineLength;
  }

  uint16_t LogService::getMaxFileSize() {
    return maxBytes;
  }

  void LogService::setMaxFileSize(uint16_t bytes) {
    this->maxBytes = bytes;
  }

  void LogService::write(char* buffer, bool verbose) {

    File file = getFile();
    if (file) {
      // ensure that every line has the same length
      lineLength = getLineLength();
      // if lineLength is undefined (<= 0) determine lineLength by length of char buffer + CR
      if (lineLength <= 0) {
        lineLength = strlen(buffer) + 1;
      }
      // create a character entry with a fixed line length
      char entry[lineLength];
      // copy buffer into the fixed entry, so that ...
      // ... a smaller buffer is filled up by padding empty spaces and keep one character for CR
      if (strlen(buffer) < lineLength) {
        sprintf(entry, "%-*s", lineLength - 1, buffer);
      // ... a bigger buffer is truncated to fit and keep one character for CR
      } else {
        sprintf(entry, "%.*s", lineLength - 1, buffer);
      }
      // write log entry at next offset position
      uint16_t offset = getOffset() + 1;
      file.seek(offset, SeekSet);
      file.println(entry);
      // persist new offset position in file
      writeOffset(offset + lineLength);
      // write to serial output in verbose mode
      if (verbose) {
        Log.verbose(F("[%s][length=%d]" CR), entry, lineLength);
      }
    }
  }

  void LogService::write(String str, bool verbose) {
    write(str.c_str(), verbose);
  }

  StreamString LogService::getLog() {

    StreamString stream;
    String line;
    uint16_t offset = readOffset();
    // start reading from offset position until lines reached
    file.seek(offset, SeekSet);
    Log.verbose(F("Read file from offset [%d]." CR), offset);
    while (file.available()) {
      line = file.readStringUntil('\n');
      stream.println(line);
    }
    // continue reading from inital offset until lines reached or offset reached
    file.seek(DEFAULT_OFFSET, SeekSet);
    int bytes = DEFAULT_OFFSET;
    while (file.available() && bytes < offset) {
      line = file.readStringUntil('\n');
      bytes += line.length() + 1;
      stream.println(line);
    }
    stream.flush();

    return stream;
  }

  uint16_t LogService::readOffset() {

    uint16_t offset = DEFAULT_OFFSET; // assume DEFAULT_OFFSET as fallback solution
    File file = getFile();
    if (file) {
      file.seek(0, SeekSet);
      String line = file.readStringUntil('\n');
      offset = line.toInt();
      if (offset == 0) {
        Log.error(F("File [%s] doesn't contain a valid offset value." CR), path.c_str());
      } else {
        Log.verbose(F("Offset from file is [%d]." CR), offset);
      }
    }

    return offset;
  }

  void LogService::writeOffset(uint16_t offset) {

    file.seek(0, SeekSet);
    char digits[10]; // max offset value consists of 10 digits
    sprintf(digits, "%-10d", offset);
    file.println(digits);
    file.flush();
    this->offset = offset;
  }

  uint16_t LogService::readLineLength() {

    uint16_t lineLength = 0;
    File file = getFile();
    if (file) {
      file.seek(DEFAULT_OFFSET, SeekSet);
      String line = file.readStringUntil('\n');
      lineLength = line.length();
    }

    return lineLength;
  }

  File LogService::getFile() {

    // if file is null call init first
    if (!file) {
      init();
    }

    return file;
  }

  void LogService::init() {

    if (!file) {
      // open existing file and set file preferences
      if (SPIFFS.exists(path)) {
        file = SPIFFS.open(path, "r+");
        if (file) {
          Log.verbose(F("Open file [%s] successful." CR), path.c_str());
          offset = readOffset();
          lineLength = readLineLength();
        } else {
          Log.error(F("Open file [%s] failed." CR), path.c_str());
        }
      // create a new file and set default file preferences
      } else {
        file = SPIFFS.open(path, "w+");
        if (file) {
          Log.error(F("Creating new file [%s] successful." CR), path.c_str());
          writeOffset(DEFAULT_OFFSET);
          lineLength = 0; // unset by default
        } else {
          Log.error(F("Creating new file [%s] failed." CR), path.c_str());
        }
      //}
    }
  }
}
