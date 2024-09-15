/*
  v0.935
  BbkIoT.cpp - Library for connecting to BBK IoT server.
  Created by Ehab Ahmad, September 15, 2024.
*/

#include "BbkIoT.h"

SocketIOclient socketIO;

JsonObjectHandler _jsonObjectHandler;
OnConnected _onConnected;
OnDisconnected _onDisconnected;

StaticJsonDocument<256> messageToSendDoc;
JsonArray messageToSendJsonArray;
StaticJsonDocument<100> dataDoc;
JsonObject dataJsonObject;

String eventName;
String _keyES;


void bbkIoTBegin(OnConnected onConnected, OnDisconnected onDisconnected, JsonObjectHandler jsonObjectHandler, String keyES) {
  _keyES = keyES;
  
  socketIO.beginSSL("bbkiotserver-production.up.railway.app", 443, "/socket.io/?EIO=4");
  
  socketIO.onEvent(bbkIoTSocketIOEvent);
  _jsonObjectHandler = jsonObjectHandler;
  _onConnected = onConnected;
  _onDisconnected = onDisconnected;
}

void bbkIoTSocketIOEvent(socketIOmessageType_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      Serial.printf("[IOc] Disconnected!\n");
      _onDisconnected();
      break;
    case sIOtype_CONNECT:
      Serial.printf("[IOc] Connected to url: %s\n", payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");
      eventName = "joinRequestFromES";
      bbkIoTSendDataToServer("dummy", 0);
      break;
    case sIOtype_EVENT:
      Serial.printf("[IOc] get event: %s\n", payload);
      bbkIoTHandleIncomingMessage(payload);
      break;
    case sIOtype_ACK:
      Serial.printf("[IOc] get ack: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      Serial.printf("[IOc] get error: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      Serial.printf("[IOc] get binary: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      Serial.printf("[IOc] get binary ack: %u\n", length);
      hexdump(payload, length);
      break;
  }
}

void bbkIoTHandleIncomingMessage(uint8_t* payload) {
  StaticJsonDocument<256> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.println(error.f_str());
    return;
  }

  String messageKey = doc[0];
  if (messageKey == "joinedRoom") {
    eventName = "messageFromES";
    _onConnected();
    return;
  } else if (messageKey == "messageToES") {
    String jsonStr = doc[1];
    Serial.println(messageKey);
    Serial.println(jsonStr);

    error = deserializeJson(doc, jsonStr);
    JsonObject jsonObject = doc.as<JsonObject>();
    _jsonObjectHandler(jsonObject);
  }
}

void bbkIoTSendEvent(String output) {
  socketIO.sendEVENT(output);
}

void bbkIoTSendDataToServer(String property, char data[]) {
  dataJsonObject = dataDoc.to<JsonObject>();
  dataJsonObject["keyES"] = _keyES;
  dataJsonObject["property"] = property;
  dataJsonObject["value"] = data;
  bbkIoTSendMessage();
}

void bbkIoTSendDataToServer(String property, bool data) {
  dataJsonObject = dataDoc.to<JsonObject>();
  dataJsonObject["keyES"] = _keyES;
  dataJsonObject["property"] = property;
  dataJsonObject["value"] = data;
  bbkIoTSendMessage();
}

void bbkIoTSendDataToServer(String property, int data) {
  dataJsonObject = dataDoc.to<JsonObject>();
  dataJsonObject["keyES"] = _keyES;
  dataJsonObject["property"] = property;
  dataJsonObject["value"] = data;
  bbkIoTSendMessage();
}

void bbkIoTSendDataToServer(String property, long data) {
  dataJsonObject = dataDoc.to<JsonObject>();
  dataJsonObject["keyES"] = _keyES;
  dataJsonObject["property"] = property;
  dataJsonObject["value"] = data;
  bbkIoTSendMessage();
}

void bbkIoTSendDataToServer(String property, long long data) {
  dataJsonObject = dataDoc.to<JsonObject>();
  dataJsonObject["keyES"] = _keyES;
  dataJsonObject["property"] = property;
  dataJsonObject["value"] = data;
  bbkIoTSendMessage();
}

void bbkIoTSendDataToServer(String property, double data) {
  dataJsonObject = dataDoc.to<JsonObject>();
  dataJsonObject["keyES"] = _keyES;
  dataJsonObject["property"] = property;
  dataJsonObject["value"] = data;
  bbkIoTSendMessage();
}

void bbkIoTSendMessage() {
  messageToSendJsonArray = messageToSendDoc.to<JsonArray>();

  // add evnet name
  // Hint: socket.on('event_name', ....
  messageToSendJsonArray.add(eventName);

  String dataStr;
  serializeJson(dataDoc, dataStr);
  messageToSendJsonArray.add(dataStr);

  String messageStr;
  serializeJson(messageToSendDoc, messageStr);

  // Send event
  bbkIoTSendEvent(messageStr);

  // Print JSON for debugging
  Serial.println(messageStr);
}

void bbkIoTLoop() {
  socketIO.loop();
}