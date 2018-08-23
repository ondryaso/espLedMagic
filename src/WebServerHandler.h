#ifndef ESPTEST_WEBSERVERHANDLER_H
#define ESPTEST_WEBSERVERHANDLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "Main.h"
#include "LedMan.h"

class WebServerHandler : public RequestHandler {
public:
    explicit WebServerHandler(LedMan * ledManager) {
        this->lm = ledManager;
    }

    virtual bool canHandle(HTTPMethod method, String uri);

    virtual bool canUpload(String uri);

    virtual bool handle(ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri);

    virtual void upload(ESP8266WebServer &server, String requestUri, HTTPUpload &upload);
private:
    LedMan * lm;
};


#endif //ESPTEST_WEBSERVERHANDLER_H
