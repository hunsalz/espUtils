#include "WebSocketListener.h"

namespace esp8266util
{

void WebSocketListener::onConnect(WSEventHandler handler)
{
  _connectWSEventHandler = handler;
}

void WebSocketListener::onDisconnect(WSEventHandler handler)
{
  _disconnectWSEventHandler = handler;
}

void WebSocketListener::onError(WSErrorHandler handler)
{
  _errorWSEventHandler = handler;
}

void WebSocketListener::onPong(WSEventHandler handler)
{
  _pongWSEventHandler = handler;
}

void WebSocketListener::onTextMessage(WSEventHandler handler)
{
  _textWSEventHandler = handler;
}

void WebSocketListener::onBinaryMessage(WSEventHandler handler)
{
  _binaryWSEventHandler = handler;
}

void WebSocketListener::onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
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
  case WS_EVT_DATA:
  {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    // process data if it's a single frame and all payload is available
    if (info->final && info->index == 0 && info->len == len)
    {
      if (info->opcode == WS_TEXT)
      {
        data[len] = '\0';
        handleTextMessageEvent(ws, client, type, info, data, len);
      }
      else
      {
        handleBinaryMessageEvent(ws, client, type, info, data, len);
      }
    }
    else
    {
      // TODO implementation for multiple frames or the frame is split into multiple packets
    }
  }
  break;
  default:
    LOG.error(F("ws[%s][%u] - Unexpected type definition"), ws->url(), client->id());
    // TODO send an error response to the client
    break;
  }
}

void WebSocketListener::handleConnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len)
{
  if (_connectWSEventHandler)
  {
    _connectWSEventHandler(ws, client, type, info, data, len);
  }
  else
  {
    LOG.verbose(F("ws[%s][%u] connected"), ws->url(), client->id());
  }
}

void WebSocketListener::handleDisconnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len)
{
  if (_disconnectWSEventHandler)
  {
    _disconnectWSEventHandler(ws, client, type, info, data, len);
  }
  else
  {
    LOG.verbose(F("ws[%s][%u] disconnected: %u"), ws->url(), client->id());
  }
}

void WebSocketListener::handleErrorEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, uint16_t *arg, uint8_t *data, size_t len)
{
  if (_errorWSEventHandler)
  {
    _errorWSEventHandler(ws, client, type, arg, data, len);
  }
  else
  {
    LOG.error(F("ws[%s][%u] error(%u): %s"), ws->url(), client->id(), arg, (char *)data);
  }
}

void WebSocketListener::handlePongEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len)
{
  if (_pongWSEventHandler)
  {
    _pongWSEventHandler(ws, client, type, info, data, len);
  }
  else
  {
    LOG.verbose(F("ws[%s][%u] pong[%u]: %s"), ws->url(), client->id(), len, (len) ? (char *)data : "");
  }
}

void WebSocketListener::handleTextMessageEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len)
{
  if (_textWSEventHandler)
  {
    data[len] = 0;
    _textWSEventHandler(ws, client, type, info, data, len);
  }
  else
  {
    // TODO test verbose message
    LOG.verbose(F("ws[%s][%u] received : %s"), ws->url(), client->id(), (char *)data);
  }
}

void WebSocketListener::handleBinaryMessageEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len)
{
  if (_binaryWSEventHandler)
  {
    _binaryWSEventHandler(ws, client, type, info, data, len);
  }
  else
  {
    LOG.verbose(F("ws[%s][%u] received : %d bytes"), ws->url(), client->id(), len);
  }
}
}
