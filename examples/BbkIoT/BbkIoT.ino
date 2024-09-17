/*
  v0.955
  BbkIoT

  Uses BBK IoT library to connect to IoT server and exchange data with mobile app
  https://play.google.com/store/apps/details?id=com.bbk.iot

  !!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTICE !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    - Download and install WebSocketsClient library from 
      https://github.com/Links2004/arduinoWebSockets (Library Manager: WebSockets by Markus Sattler)
    - Use ArduinoJson library https://arduinojson.org/
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include "BbkIoT.h"

ESP8266WiFiMulti WiFiMulti;

bool booleanVar;
int number;
double numberDouble;
long long numberLong;

long lastTime;

void onConnected() {
  digitalWrite(LED_BUILTIN, true);
}

void onDisconnected() {
  digitalWrite(LED_BUILTIN, false);
}

void jsonObjectHandler(JsonObject jsonObject) {
  if (jsonObject["property"] == "MobileToEs") {
    int value = jsonObject["value"];
    bool ledOff = value % 5 != 0;
    digitalWrite(LED_BUILTIN, ledOff);
  }
}

void sendDataToServer() {
  bbkIoTSendDataToServer("EsToMobile1", booleanVar);
  bbkIoTSendDataToServer("EsToMobile2", number);
  bbkIoTSendDataToServer("EsToMobile3", numberDouble);
  bbkIoTSendDataToServer("EsToMobile4", numberLong);
  bbkIoTSendDataToServer("EsToMobile5", "Hello");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  // disable AP
  if (WiFi.getMode() & WIFI_AP) {
    WiFi.softAPdisconnect(true);
  }

  WiFiMulti.addAP("wifi ssid name", "wifi password");
  
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  String ip = WiFi.localIP().toString();
  Serial.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

  bbkIoTBegin(onConnected, onDisconnected, 
    jsonObjectHandler, "es key goes here");

  booleanVar = false;
  number = 0;
  numberDouble = 7.2;
  numberLong = 12345678912;

  lastTime = millis();
}

void loop() {
  bbkIoTLoop();

  if (millis() - lastTime >= 1000) {
    lastTime = millis();

    booleanVar = !booleanVar;
    number++;
    numberDouble++;
    numberLong++;

    sendDataToServer();
  }
}