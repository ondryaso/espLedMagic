#include "WebSocketManager.h"

WebSocketManager* WebSocketManager::instance = nullptr;

void WebSocketManager::init() {
    this->server->begin();
    WebSocketManager::instance = this;
    this->server->onEvent(WebSocketManager::webSocketEvent);
}

void WebSocketManager::notifyClients() {
    for(uint8_t i = 0; i < 16; i++) {
        if(clientsToNotify[i] != 0 && (millis() % clientsToNotify[i]) == 0) {
            char * s = getColorString('X');
            server->sendTXT(i, s);
            delete[] s;
        }
    }
}

void WebSocketManager::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
        switch(type) {
        case WStype_ERROR:
            Serial.printf("Error");
            break;
        case WStype_DISCONNECTED:
            Serial.printf("Client %u disconnected\n", num);
            if(num < 16) {
                instance->clientsToNotify[num] = 0;
            }
            break;
        case WStype_CONNECTED: {
            IPAddress ip = instance->server->remoteIP(num);
            Serial.printf("Client [%u] connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            instance->server->sendTXT(num, "HI");
        }
            break;
        case WStype_TEXT: {
            Serial.println((const char *) payload);

            switch(payload[0]) {
                case 'M':
                    instance->ledManager->setMode(static_cast<LedMode>(strtol((const char *)(payload + 1), nullptr, 10)));
                    // TODO
                    instance->server->sendTXT(num, "M:K");
                    break;
                case 'S': {
                    if (instance->ledManager->getMode() == LedMode::MANUAL) {
                        char * last;

                        *instance->ledManager->r = (float) strtod((const char *) (payload + 1), &last);
                        *instance->ledManager->g = (float) strtod(last, &last);
                        *instance->ledManager->b = (float) strtod(last, nullptr);

                        instance->server->sendTXT(num, "S:K");
                    } else {
                        ModeSettings * set = LedMan::createModeSettings(instance->ledManager->getMode(),
                                                                        (const char *) (payload + 1));

                        if (set != nullptr) {
                            instance->ledManager->setModeSettings(set);
                            instance->server->sendTXT(num, "S:K");
                        } else {
                            instance->server->sendTXT(num, "S:BAD_SET");
                        }
                    }

                    break;
                }
                case 'C': {
                    instance->server->sendTXT(num, instance->getColorString('C'));
                    break;
                }
                case 'N': {
                    if(num < 16) {
                        instance->clientsToNotify[num] = (int) strtol((const char *) (payload + 1), nullptr, 10);
                    }

                    instance->server->sendTXT(num, "N:K");
                    break;
                }
                default:
                    instance->server->sendTXT(num, "WUT");
                    break;
            }
        }
            break;
        case WStype_BIN:
            instance->server->sendTXT(num, "SRY");
            break;
        default:
            instance->server->sendTXT(num, "SRY");
            break;
    }
}

char * WebSocketManager::getColorString(char prefix) {
    char * ret = new char[23];
    ret[0] = prefix;
    ret[1] = ':';

    dtostrf(*ledManager->r, 1, 4, (ret + 2));
    ret[8] = ' ';
    dtostrf(*ledManager->g, 1, 4, (ret + 9));
    ret[15] = ' ';
    dtostrf(*ledManager->b, 1, 4, (ret + 16));

    return ret;
}