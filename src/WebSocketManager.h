#ifndef ESPMAGIC_WEBSOCKETMANAGER_H
#define ESPMAGIC_WEBSOCKETMANAGER_H

#include "LedMan.h"
#include <WebSocketsServer.h>

class WebSocketManager {
public:
    WebSocketManager(LedMan * ledManager) {
        this->ledManager = ledManager;
        this->server = new WebSocketsServer(8101);
    }

    void init();
    void notifyClients();

private:
    LedMan * ledManager;
    WebSocketsServer * server;
    static WebSocketManager * instance;

    int clientsToNotify[16];

    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
    char * getColorString(char prefix);
};


#endif