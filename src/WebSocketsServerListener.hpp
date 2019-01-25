#pragma once

#include <WebSocketsServer.h>  // https://github.com/Links2004/arduinoWebSockets/blob/master/src/WebSocketsServer.h

#include "Logging.hpp"

namespace espUtils {

class WebSocketsServerListener {
 
 public:
  
  void onConnect(WebSocketsServer::WebSocketServerEvent event) {
    _connectWebSocketServerEvent = event;
  }

  void onDisconnect(WebSocketsServer::WebSocketServerEvent event) {
    _disconnectWebSocketServerEvent = event;
  }

  void onError(WebSocketsServer::WebSocketServerEvent event) {
    _errorWebSocketServerEvent = event;
  }

  void onTextMessage(WebSocketsServer::WebSocketServerEvent event) {
    _textWebSocketServerEvent = event;
  }

  void onBinaryMessage(WebSocketsServer::WebSocketServerEvent event) {
    _binaryWebSocketServerEvent = event;
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
        ERROR_P(F("Unexpected type definition - Not implemented yet."));
        break;
    }
  }

 private:
  
  WebSocketsServer::WebSocketServerEvent _connectWebSocketServerEvent;
  WebSocketsServer::WebSocketServerEvent _disconnectWebSocketServerEvent;
  WebSocketsServer::WebSocketServerEvent _errorWebSocketServerEvent;
  WebSocketsServer::WebSocketServerEvent _textWebSocketServerEvent;
  WebSocketsServer::WebSocketServerEvent _binaryWebSocketServerEvent;

  void handleConnectEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    if (_connectWebSocketServerEvent) {
      _connectWebSocketServerEvent(num, type, payload, length);
    } else {
      VERBOSE_FP(F("WebSocket client [%u] connected."), num);
    }
  }

  void handleDisconnectEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    if (_disconnectWebSocketServerEvent) {
      _disconnectWebSocketServerEvent(num, type, payload, length);
    } else {
      VERBOSE_FP(F("WebSocket client [%u] disconnected."), num);
    }
  }

  void handleErrorEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    if (_errorWebSocketServerEvent) {
      _errorWebSocketServerEvent(num, type, payload, length);
    } else {
      ERROR_FP(F("[%u] received error event with length [%u]"), num, length);
    }
  }

  void handleTextMessageEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    if (_textWebSocketServerEvent) {
      _textWebSocketServerEvent(num, type, payload, length);
    } else {
      VERBOSE_FP(F("[%u] received text message: %s"), num, payload);
    }
  }

  void handleBinaryMessageEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    
    if (_binaryWebSocketServerEvent) {
      _binaryWebSocketServerEvent(num, type, payload, length);
    } else {
      VERBOSE_FP(F("[%u] received binary message with length [%u]"), num, length);
    }
  }
};
}  // namespace espUtils