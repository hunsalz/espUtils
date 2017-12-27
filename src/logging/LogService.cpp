#include "LogService.h"

namespace esp8266util {

  LogService::LogService() {};

  LogService::~LogService() {
    getFile().close();
  }

  void LogService::begin(const char* path) {
    _path = path;
  }

  String LogService::getPath() {
    return _path;
  }

  uint16_t LogService::getOffset() {

    // if _file is null call init first
    if (_file) {
      init();
    }
    // if limit of max bytes is reached, then rollover and override entries from beginning
    if (_offset > getMaxFileSize()) {
      _offset = DEFAULT_OFFSET;
    }

    return _offset;
  }

  uint16_t LogService::getLineLength() {

    // if _file is null call init first
    if (_file) {
      init();
    }

    return _lineLength;
  }

  uint16_t LogService::getMaxFileSize() {
    return _maxBytes;
  }

  void LogService::setMaxFileSize(uint16_t bytes) {
    _maxBytes = bytes;
  }

  void LogService::write(char* entry, bool verbose) {

    File file = getFile();
    if (file) {
      // if _lineLength is undefined (<= 0) set _lineLength by length of current entry + CR
      if (getLineLength() <= 0) {
        _lineLength = strlen(entry) + 1;
      }
      // create a character buffer with a fixed line length
      char buffer[getLineLength()];
      // copy entry into the fixed buffer, so that ...
      // ... a smaller entry is filled up by padding empty spaces and keep one character for CR
      if (strlen(entry) < getLineLength()) {
        sprintf(buffer, "%-*s", getLineLength() - 1, entry);
      // ... a bigger entry is truncated to fit and keep one character for CR
      } else {
        sprintf(buffer, "%.*s", getLineLength() - 1, entry);
      }
      // write log buffer at next offset position
      uint16_t offset = getOffset() + 1;
      file.seek(offset, SeekSet);
      // write buffer with CR
      file.println(buffer);
      // persist new offset position in file
      writeOffset(offset + getLineLength());
      // write to serial output in verbose mode
      if (verbose) {
        LOG.verbose(F("[%s][length=%d][offset=%d]"), entry, getLineLength(), getOffset());
      }
    }
  }

  void LogService::write(String entry, bool verbose) {
    write(entry.c_str(), verbose);
  }

  StreamString LogService::getLog() {

    StreamString stream;
    String line;
    uint16_t offset = readOffset();
    File file = getFile();
    if (file) {
      // start reading from offset position until EoF reached
      file.seek(offset, SeekSet);
      while (file.available()) {
        line = file.readStringUntil('\n');
        stream.print(line);
      }
      // continue reading from inital offset until EoF or offset reached
      file.seek(DEFAULT_OFFSET, SeekSet);
      int bytes = DEFAULT_OFFSET;
      while (file.available() && bytes < offset) {
        line = file.readStringUntil('\n');
        bytes += line.length() + 1;
        stream.print(line);
      }
      stream.flush();
    }

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
        LOG.error(F("File [%s] doesn't contain a valid offset value."), getPath().c_str());
      }
    }

    return offset;
  }

  void LogService::writeOffset(uint16_t offset) {

    File file = getFile();
    if (file) {
      file.seek(0, SeekSet);
      char digits[10]; // max offset value consists of 10 digits
      sprintf(digits, "%-10d", offset);
      file.println(digits);
      file.flush();
      _offset = offset;
    }
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
    if (!_file) {
      init();
    }

    return _file;
  }

  void LogService::init() {

    if (!_file && _path) {
      // open existing file and set file preferences
      if (SPIFFS.exists(getPath())) {
        _file = SPIFFS.open(getPath(), "r+");
        if (_file) {
          LOG.verbose(F("Open file [%s] successful."), getPath().c_str());
          _offset = readOffset();
          _lineLength = readLineLength();
        } else {
          LOG.error(F("Open file [%s] failed."), getPath().c_str());
        }
      // create a new file and set default file preferences
      } else {
        _file = SPIFFS.open(getPath(), "w+");
        if (_file) {
          LOG.error(F("Creating new file [%s] successful."), getPath().c_str());
          writeOffset(DEFAULT_OFFSET);
          _lineLength = 0; // unset by default
        } else {
          LOG.error(F("Creating new file [%s] failed."), getPath().c_str());
        }
      }
    }
  }
}
