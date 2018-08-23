#ifndef ESPTEST_WEBSERVERHANDLER_H
#define ESPTEST_WEBSERVERHANDLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "Main.h"
#include "LedMan.h"

class ClassicWebServerHandler : public RequestHandler {
public:
    explicit ClassicWebServerHandler(LedMan * ledManager) {
        this->lm = ledManager;
        SPIFFS.begin();
    }

    virtual bool canHandle(HTTPMethod method, String uri);

    virtual bool canUpload(String uri);

    virtual bool handle(ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri);

    virtual void upload(ESP8266WebServer &server, String requestUri, HTTPUpload &upload);
private:
    LedMan * lm;
};


#endif //ESPTEST_WEBSERVERHANDLER_H
