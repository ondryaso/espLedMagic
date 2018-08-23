#ifndef ESPMAGIC_WEBSOCKETMANAGER_H
#define ESPMAGIC_WEBSOCKETMANAGER_H

#include "LedMan.h"
#include <cerrno>
#include <limits.h>
#include <WebSocketsServer.h>

bool strtouint8(uint8_t *out, char *s, char **end, int base);

class WebSocketManager {
public:
    explicit WebSocketManager(LedMan * ledManager) : clientsToNotify() {
        this->ledManager = ledManager;
        this->server = new WebSocketsServer(8101);
    }

    void init();
    void notifyClients();

private:
    int clientsToNotify[16];
    LedMan * ledManager;
    WebSocketsServer * server;

    char *getColorString(char prefix);

    static WebSocketManager * instance;
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

    static void handleCommand(uint8_t client, const uint8_t *payload, size_t length);
};


#endif