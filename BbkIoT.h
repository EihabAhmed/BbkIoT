/*
  v1.11
  BbkIoT.h - Library for connecting to BBK IoT server.
  Created by Ehab Ahmad, September 15, 2024.
*/

#ifndef BBKIOT_H_
#define BBKIOT_H_

#include <Arduino.h>

#include <ArduinoJson.h>  // https://arduinojson.org/

#include <WebSocketsClient.h>  // download and install from https://github.com/Links2004/arduinoWebSockets (Library Manager: WebSockets by Markus Sattler)
#include <SocketIOclient.h>

#include <Hash.h>

#ifdef __AVR__
typedef void (*JsonObjectHandler)(JsonObject object);
typedef void (*OnConnected)();
typedef void (*OnDisconnected)();
#else
typedef std::function<void(JsonObject object)> JsonObjectHandler;
typedef std::function<void()> OnConnected;
typedef std::function<void()> OnDisconnected;
#endif

void bbkIoTBegin(OnConnected onConnected, OnDisconnected onDisconnected, JsonObjectHandler jsonObjectHandler, String keyES);
void bbkIoTSocketIOEvent(socketIOmessageType_t type, uint8_t* payload, size_t length);
void bbkIoTHandleIncomingMessage(uint8_t* payload);
void bbkIoTSendEvent(String output);
void bbkIoTSendDataToServer(String property, char data[]);
void bbkIoTSendDataToServer(String property, bool data);
void bbkIoTSendDataToServer(String property, int data);
void bbkIoTSendDataToServer(String property, long data);
void bbkIoTSendDataToServer(String property, long long data);
void bbkIoTSendDataToServer(String property, double data);
void bbkIoTSendMessage();
void bbkIoTLoop();

#endif /* BBKIOT_H_ */