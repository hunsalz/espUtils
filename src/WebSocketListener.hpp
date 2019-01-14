#pragma once

#include <WebSockets.h>  // https://github.com/Links2004/arduinoWebSockets/blob/master/src/WebSockets.h

#include "Logging.hpp"

namespace esp8266utils {

typedef std::function<void(uint8_t num, WStype_t type, uint8_t *payload, size_t length)> WSEventHandler;

class WebSocketListener {
 
 public:
  
  void onConnect(WSEventHandler handler) {
    _connectWSEventHandler = handler;
  }

  void onDisconnect(WSEventHandler handler) {
    _disconnectWSEventHandler = handler;
  }

  void onError(WSEventHandler handler) {
    _errorWSEventHandler = handler;
  }

  void onTextMessage(WSEventHandler handler) {
    _textWSEventHandler = handler;
  }

  void onBinaryMessage(WSEventHandler handler) {
    _binaryWSEventHandler = handler;
  }

  void onEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    switch (type) {
      case WStype_CONNECTED:
        handleConnectEvent(num, type, payload, length);
        break;
      case WStype_DISCONNECTED:
        handleDisconnectEvent(num, type, payload, length);
        break;
      case WStype_ERROR:
        handleErrorEvent(num, type, payload, length);
        break;
      case WStype_TEXT:
        handleTextMessageEvent(num, type, payload, length);
        break;
      case WStype_BIN:
        handleBinaryMessageEvent(num, type, payload, length);
        break;
      default:
        ERROR_FP(F("[%u] - Got unexpected type definition"), num);
        // TODO send an error response to the client
        break;
    }
  }

 private:
  
  WSEventHandler _connectWSEventHandler;
  WSEventHandler _disconnectWSEventHandler;
  WSEventHandler _errorWSEventHandler;
  WSEventHandler _textWSEventHandler;
  WSEventHandler _binaryWSEventHandler;

  void handleConnectEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    if (_connectWSEventHandler) {
      _connectWSEventHandler(num, type, payload, length);
    } else {
      VERBOSE_FP(F("[%u] connected."), num);
      //IPAddress ip = webSocket.remoteIP(num);
      //VERBOSE_FP(F("[%u] connected from %d.%d.%d.%d : %s"), num, ip[0], ip[1], ip[2], ip[3], payload);
      //VERBOSE_FP(F("ws[%s][%u] connected"), ws->url(), client->id());
    }
  }

  void handleDisconnectEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    if (_disconnectWSEventHandler) {
      _disconnectWSEventHandler(num, type, payload, length);
    } else {
      VERBOSE_FP(F("[%u] disconnected."), num);
      //VERBOSE_FP(F("ws[%s][%u] disconnected: %u"), ws->url(), client->id());
    }
  }

  void handleErrorEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    if (_errorWSEventHandler) {
      _errorWSEventHandler(num, type, payload, length);
    } else {
      ERROR_FP(F("[%u] received : %s"), num, payload);
      //ERROR_FP(F("ws[%s][%u] error(%u): %s"), ws->url(), client->id(), arg, (char *)data);
    }
  }

  void handleTextMessageEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    if (_textWSEventHandler) {
      _textWSEventHandler(num, type, payload, length);
    } else {
      VERBOSE_FP(F("[%u] received : %s"), num, payload);
      //VERBOSE_FP(F("ws[%s][%u] received : %s"), ws->url(), client->id(), (char *)data);
    }
  }

  void handleBinaryMessageEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    if (_binaryWSEventHandler) {
      _binaryWSEventHandler(num, type, payload, length);
    } else {
      VERBOSE_FP(F("[%u] received : %s"), num, payload);
      //VERBOSE_FP(F("ws[%s][%u] received : %d bytes"), ws->url(), client->id(), len);
    }
  }
};
}  // namespace esp8266utils