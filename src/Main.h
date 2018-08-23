#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SPI.h>
#include <Wire.h>
#include <Hash.h>
#include <WebSocketsServer.h>
#include <WebServerHandler.h>
#include <WebSocketManager.h>

void initWifi();
void initOTA();
void initServer();
void initWS();

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
char * getColorString(char prefix);

#endif