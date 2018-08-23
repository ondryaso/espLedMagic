#ifndef ESPMAGICIMPROVED_ASYNCWEBSERVERMANAGER_H
#define ESPMAGICIMPROVED_ASYNCWEBSERVERMANAGER_H

#include <Arduino.h>
#include <Hash.h>
#include <functional>
#include <ESPAsyncWebServer.h>
#include <FS.h>

#include "LedMan.h"

class AsyncWebServerManager {
public:
    AsyncWebServerManager(LedMan * ledManager);
    void init();
private:
    AsyncWebServer * server;
    LedMan * ledMan;

    void onNotFound(AsyncWebServerRequest *request);
};


#endif //ESPMAGICIMPROVED_ASYNCWEBSERVERMANAGER_H
