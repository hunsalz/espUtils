#include "WebSocketListener.h"

void WebSocketListener::onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  switch (type) {
    case WS_EVT_CONNECT:
      handleConnectEvent(ws, client, type, arg, data, len);
      break;
    case WS_EVT_DISCONNECT:
      handleDisconnectEvent(ws, client, type, arg, data, len);
      break;
    case WS_EVT_ERROR:
      handleErrorEvent(ws, client, type, arg, data, len);
      break;
    case WS_EVT_PONG:
      handlePongEvent(ws, client, type, arg, data, len);
      break;
    case WS_EVT_DATA:
      // process data if it's a single frame and all payload is available and data contains only text
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

        handleMessageEvent(ws, client, type, arg, data, len);

        // convert data into char array
        char message[info->len + 1];
        strncpy(message, (char*)data, info->len);
        // make certain that the message ends with a \0 terminator
        message[info->len] = '\0';

        // TODO: workaround - replace by Log.verbose(...)
        Serial.printf("V: ws[%s][%u] received : %s\n", ws->url(), client->id(), String(message).c_str());

        // try to interpret message as JSON
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(message);
        // interrupt if message contains no valid JSON
        if (json.success()) {
          process(ws, client, json);

        } else {
          Log.error(F("Parsing message into JSON failed." CR));
        }
      } else {
        // TODO send a response to the client
      }
      break;
    default:
      Log.verbose(F("ws[%s][%u] frame[%u] %s[%llu - %llu]" CR), ws->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      // TODO send a response to the client
      break;
    }
}

void WebSocketListener::onConnect(EventHandler handler) {
  connectEventHandler = handler;
}

void WebSocketListener::onDisconnect(EventHandler handler) {
  disconnectEventHandler = handler;
}

void WebSocketListener::onError(EventHandler handler) {
  errorEventHandler = handler;
}

void WebSocketListener::onPong(EventHandler handler) {
  pongEventHandler = handler;
}

void WebSocketListener::onMessage(EventHandler handler) {
  messageEventHandler = handler;
}

void WebSocketListener::handleConnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  if (connectEventHandler != NULL) {
    connectEventHandler(ws, client, type, arg, data, len);
  } else {
    Log.verbose(F("ws[%s][%u] connected" CR), ws->url(), client->id());
  }
}

void WebSocketListener::handleDisconnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  if (disconnectEventHandler != NULL) {
    disconnectEventHandler(ws, client, type, arg, data, len);
  } else {
    Log.verbose(F("ws[%s][%u] disconnected: %u" CR), ws->url(), client->id());
  }
}

void WebSocketListener::handleErrorEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  if (errorEventHandler != NULL) {
    errorEventHandler(ws, client, type, arg, data, len);
  } else {
    Log.error(F("ws[%s][%u] error(%u): %s" CR), ws->url(), client->id(), *((uint16_t*)arg), (char*)data);
  }
}

void WebSocketListener::handlePongEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  if (pongEventHandler != NULL) {
    pongEventHandler(ws, client, type, arg, data, len);
  } else {
    Log.verbose(F("ws[%s][%u] pong[%u]: %s" CR), ws->url(), client->id(), len, (len)?(char*)data:"");
  }
}

void WebSocketListener::handleMessageEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  if (messageEventHandler != NULL) {
    messageEventHandler(ws, client, type, arg, data, len);
  } else {
    // TODO: workaround - replace by Log.verbose(...)
    //Serial.printf("V: ws[%s][%u] received : %s\n", ws->url(), client->id(), String(message).c_str());
  }
}
