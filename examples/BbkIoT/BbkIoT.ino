/*
  v1.001
  BbkIoT

  Uses BBK IoT library to connect to IoT server and exchange data with mobile app
  https://play.google.com/store/apps/details?id=com.bbk.iot

  !!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTICE !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    - Library Manager -> install WebSockets by Markus Sattler
    - Library Manager -> install ArduinoJson by Benoit Blanchon
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include "BbkIoT.h"

ESP8266WiFiMulti WiFiMulti;

/*******************************************************************************/
/**** Custom properties - change these properties as needed in your program ****/
/*******************************************************************************/
bool booleanVar;
int number;
double numberDouble;
long long numberLong;
/*******************************************************************************/
/****************************** Custom properties ******************************/
/*******************************************************************************/

long lastTime;

void onConnected() {
  digitalWrite(LED_BUILTIN, true);
}

void onDisconnected() {
  digitalWrite(LED_BUILTIN, false);
}

void jsonObjectHandler(JsonObject jsonObject) {
/*******************************************************************************/
/* Property name - change to match the property name in the mobile application */
/*******************************************************************************/
  String property = "MobileToEs";
/*******************************************************************************/
/******************************** Property name ********************************/
/*******************************************************************************/
  property.toLowerCase();
  property.trim();

  String incomingProperty = jsonObject["property"];
  incomingProperty.toLowerCase();
  incomingProperty.trim();

  if (incomingProperty == property) {
    int value = jsonObject["value"];
    bool ledOff = value % 5 != 0;
    digitalWrite(LED_BUILTIN, ledOff);
  }
}

void sendDataToServer() {
/*******************************************************************************/
/** Property names - change to match the properties in the mobile application **/
/*******************************************************************************/
  bbkIoTSendDataToServer("EsToMobile1", booleanVar);
  bbkIoTSendDataToServer("EsToMobile2", number);
  bbkIoTSendDataToServer("EsToMobile3", numberDouble);
  bbkIoTSendDataToServer("EsToMobile4", numberLong);
  bbkIoTSendDataToServer("EsToMobile5", "Hello");
/*******************************************************************************/
/******************************** Property names *******************************/
/*******************************************************************************/
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  // disable AP
  if (WiFi.getMode() & WIFI_AP) {
    WiFi.softAPdisconnect(true);
  }

  WiFiMulti.addAP("wifi ssid name", "wifi password"); // Replace with your WiFi SSID and password
  
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  String ip = WiFi.localIP().toString();
  Serial.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

  bbkIoTBegin(onConnected, onDisconnected, 
    jsonObjectHandler, "es key goes here"); // Replace with ES Key obtained from the mobile application

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