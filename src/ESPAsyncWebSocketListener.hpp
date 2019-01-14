#pragma once

#include <AsyncWebSocket.h>  // https://github.com/me-no-dev/ESPAsyncWebServer

#include "Logging.hpp"

namespace esp8266utils {

typedef std::function<void(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len)> WSEventHandler;
typedef std::function<void(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, uint16_t *arg, uint8_t *data, size_t len)> WSErrorHandler;

class ESPAsyncWebSocketListener {
 
 public:
  
  void onConnect(WSEventHandler handler) {
    _connectWSEventHandler = handler;
  }

  void onDisconnect(WSEventHandler handler) {
    _disconnectWSEventHandler = handler;
  }

  void onError(WSErrorHandler handler) {
    _errorWSEventHandler = handler;
  }

  void onPong(WSEventHandler handler) {
    _pongWSEventHandler = handler;
  }

  void onTextMessage(WSEventHandler handler) {
    _textWSEventHandler = handler;
  }

  void onBinaryMessage(WSEventHandler handler) {
    _binaryWSEventHandler = handler;
  }

  void onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    
    switch (type) {
      case WS_EVT_CONNECT:
        handleConnectEvent(ws, client, type, (AwsFrameInfo *)arg, data, len);
        break;
      case WS_EVT_DISCONNECT:
        handleDisconnectEvent(ws, client, type, (AwsFrameInfo *)arg, data, len);
        break;
      case WS_EVT_ERROR:
        handleErrorEvent(ws, client, type, (uint16_t *)arg, data, len);
        break;
      case WS_EVT_PONG:
        handlePongEvent(ws, client, type, (AwsFrameInfo *)arg, data, len);
        break;
      case WS_EVT_DATA: {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        // process data if it's a single frame and all payload is available
        if (info->final && info->index == 0 && info->len == len) {
          if (info->opcode == WS_TEXT) {
            handleTextMessageEvent(ws, client, type, info, data, len);
          } else {
            handleBinaryMessageEvent(ws, client, type, info, data, len);
          }
        } else {
          // TODO implementation for multiple frames or the frame is split into
          // multiple packets
        }
      } break;
      default:
        ERROR_FP(F("ws[%s][%u] - Unexpected type definition"), ws->url(), client->id());
        // TODO send an error response to the client
        break;
    }
  }

 private:
  
  WSEventHandler _connectWSEventHandler;
  WSEventHandler _disconnectWSEventHandler;
  WSErrorHandler _errorWSEventHandler;
  WSEventHandler _pongWSEventHandler;
  WSEventHandler _textWSEventHandler;
  WSEventHandler _binaryWSEventHandler;

  void handleConnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {
    
    if (_connectWSEventHandler) {
      _connectWSEventHandler(ws, client, type, info, data, len);
    } else {
      VERBOSE_FP(F("ws[%s][%u] connected"), ws->url(), client->id());
    }
  }

  void handleDisconnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {
    
    if (_disconnectWSEventHandler) {
      _disconnectWSEventHandler(ws, client, type, info, data, len);
    } else {
      VERBOSE_FP(F("ws[%s][%u] disconnected: %u"), ws->url(), client->id());
    }
  }

  void handleErrorEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, uint16_t *arg, uint8_t *data, size_t len) {
    
    if (_errorWSEventHandler) {
      _errorWSEventHandler(ws, client, type, arg, data, len);
    } else {
      ERROR_FP(F("ws[%s][%u] error(%u): %s"), ws->url(), client->id(), arg, (char *)data);
    }
  }

  void handlePongEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {
    
    if (_pongWSEventHandler) {
      _pongWSEventHandler(ws, client, type, info, data, len);
    } else {
      VERBOSE_FP(F("ws[%s][%u] pong[%u]: %s"), ws->url(), client->id(), len, (len) ? (char *)data : "");
    }
  }

  void handleTextMessageEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {
    
    if (_textWSEventHandler) {
      _textWSEventHandler(ws, client, type, info, data, len);
    } else {
      VERBOSE_FP(F("ws[%s][%u] received : %s"), ws->url(), client->id(), (char *)data);
    }
  }

  void handleBinaryMessageEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {
    
    if (_binaryWSEventHandler) {
      _binaryWSEventHandler(ws, client, type, info, data, len);
    } else {
      VERBOSE_FP(F("ws[%s][%u] received : %d bytes"), ws->url(), client->id(), len);
    }
  }
};
}  // namespace esp8266utils