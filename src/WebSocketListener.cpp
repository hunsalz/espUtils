#include "WebSocketListener.h"

void WebSocketListener::onConnect(WSEventHandler handler) {
  connectWSEventHandler = handler;
}

void WebSocketListener::onDisconnect(WSEventHandler handler) {
  disconnectWSEventHandler = handler;
}

void WebSocketListener::onError(WSErrorHandler handler) {
  errorWSEventHandler = handler;
}

void WebSocketListener::onPong(WSEventHandler handler) {
  pongWSEventHandler = handler;
}

void WebSocketListener::onTextMessage(WSEventHandler handler) {
  textWSEventHandler = handler;
}

void WebSocketListener::onBinaryMessage(WSEventHandler handler) {
  binaryWSEventHandler = handler;
}

void WebSocketListener::onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  switch (type) {
    case WS_EVT_CONNECT:
      handleConnectEvent(ws, client, type, (AwsFrameInfo*)arg, data, len);
      break;
    case WS_EVT_DISCONNECT:
      handleDisconnectEvent(ws, client, type, (AwsFrameInfo*)arg, data, len);
      break;
    case WS_EVT_ERROR:
      handleErrorEvent(ws, client, type, (uint16_t*)arg, data, len);
      break;
    case WS_EVT_PONG:
      handlePongEvent(ws, client, type, (AwsFrameInfo*)arg, data, len);
      break;
    case WS_EVT_DATA:
      {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        // process data if it's a single frame and all payload is available
        if (info->final && info->index == 0 && info->len == len) {
          if (info->opcode == WS_TEXT) {
            data[len] = '\0';
            handleTextMessageEvent(ws, client, type, info, data, len);
          } else {
            handleBinaryMessageEvent(ws, client, type, info, data, len);
          }
        } else {
          // TODO implementation for multiple frames or the frame is split into multiple packets
        }
      }
      break;
    default:
      Log.error(F("ws[%s][%u] - Unexpected type definition" CR), ws->url(), client->id());
      // TODO send an error response to the client
      break;
    }
}

void WebSocketListener::handleConnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {

  if (connectWSEventHandler != NULL) {
    connectWSEventHandler(ws, client, type, info, data, len);
  } else {
    Log.verbose(F("ws[%s][%u] connected" CR), ws->url(), client->id());
  }
}

void WebSocketListener::handleDisconnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {

  if (disconnectWSEventHandler != NULL) {
    disconnectWSEventHandler(ws, client, type, info, data, len);
  } else {
    Log.verbose(F("ws[%s][%u] disconnected: %u" CR), ws->url(), client->id());
  }
}

void WebSocketListener::handleErrorEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, uint16_t *arg, uint8_t *data, size_t len) {

  if (errorWSEventHandler != NULL) {
    errorWSEventHandler(ws, client, type, arg, data, len);
  } else {
    Log.error(F("ws[%s][%u] error(%u): %s" CR), ws->url(), client->id(), arg, (char*)data);
  }
}

void WebSocketListener::handlePongEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {

  if (pongWSEventHandler != NULL) {
    pongWSEventHandler(ws, client, type, info, data, len);
  } else {
    Log.verbose(F("ws[%s][%u] pong[%u]: %s" CR), ws->url(), client->id(), len, (len)?(char*)data:"");
  }
}

void WebSocketListener::handleTextMessageEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {

  if (textWSEventHandler != NULL) {
    textWSEventHandler(ws, client, type, info, data, len);
  } else {
    Serial.printf("V: ws[%s][%u] received : %s\n", ws->url(), client->id(), (char*)data);
    // TODO Log.verbose(F("ws[%s][%u] received : %s\n" CR), ws->url(), client->id(), (char*)data); results in fatal error
  }
}

void WebSocketListener::handleBinaryMessageEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {

  if (binaryWSEventHandler != NULL) {
    binaryWSEventHandler(ws, client, type, info, data, len);
  } else {
    Log.verbose(F("ws[%s][%u] received : %d bytes" CR), ws->url(), client->id(), len);
  }
}
