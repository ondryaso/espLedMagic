#include "WebSocketManager.h"

WebSocketManager *WebSocketManager::instance = nullptr;

bool strtouint8(uint8_t *out, char *s, char **end, int base) {
    if (s[0] == '\0' || isspace(s[0]))
        return false;

    errno = 0;
    long l = strtol(s, end, base);


    if (l > UINT8_MAX || (errno == ERANGE && l == LONG_MAX))
        return false;
    if (l < 0 || (errno == ERANGE && l == LONG_MIN))
        return false;

    *out = (uint8_t) l;

    return true;
}

void WebSocketManager::init() {
    this->server->begin();
    WebSocketManager::instance = this;
    this->server->onEvent(WebSocketManager::webSocketEvent);
}

void WebSocketManager::notifyClients() {
    for (uint8_t i = 0; i < 16; i++) {
        if (clientsToNotify[i] != 0 && (millis() % clientsToNotify[i]) == 0) {
            char *s = getColorString('X');
            server->sendTXT(i, s);
            delete[] s;
        }
    }
}

void WebSocketManager::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_ERROR:
            Serial.printf("Error\n");
            break;
        case WStype_DISCONNECTED:
            Serial.printf("Client %u disconnected\n", num);
            if (num < 16) {
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
            handleCommand(num, payload, length);

            break;
        }
        case WStype_BIN:
            Serial.printf("BIN\n");
            instance->server->sendTXT(num, "E_PROTO");
            break;
        default:
            Serial.printf("Other\n");
            instance->server->sendTXT(num, "E_PROTO");
            break;
    }
}

void WebSocketManager::handleCommand(uint8_t client, const uint8_t *payload, size_t length) {
    switch (payload[0]) {
        case 'M':
            instance->ledManager->setMode(static_cast<LedMode>(strtol((const char *) (payload + 1), nullptr, 10)));
            // TODO
            instance->server->sendTXT(client, "M:K");
            break;
        case 'S': {
            if (instance->ledManager->getMode() == MANUAL) {
                char *last;

                instance->ledManager->setR(strtof((const char *) (payload + 1), &last));
                instance->ledManager->setG(strtof(last, &last));
                instance->ledManager->setB(strtof(last, &last));
                instance->ledManager->setW(strtof(last, nullptr));

                instance->server->sendTXT(client, "S:K");
            } else {
                ModeSettings *set = LedMan::createModeSettings(instance->ledManager->getMode(),
                                                               (const char *) (payload + 1));

                if (set != nullptr) {
                    instance->ledManager->setModeSettings(set);
                    instance->server->sendTXT(client, "S:K");
                } else {
                    instance->server->sendTXT(client, "S:E_FORMAT");
                }
            }

            break;
        }
        case 'C': {
            if (length < 4) {
                instance->server->sendTXT(client, "C:E_LEN");
            } else {
                if (instance->ledManager->getMode() == MANUAL) {
                    uint8_t out = 0;
                    char *last;

                    if (strtouint8(&out, (char *) (payload + 1), &last, 10)) {
                        if (out < 4) {
                            float *chansPtr = instance->ledManager->getRGBW();
                            float val = strtof(last, nullptr);

                            *(chansPtr + out) = val;

                            instance->server->sendTXT(client, "C:K");
                        } else {
                            instance->server->sendTXT(client, "C:E_ARG");
                        }
                    } else {
                        instance->server->sendTXT(client, "C:E_FORMAT");
                    }
                } else {
                    instance->server->sendTXT(client, "C:E_MODE");
                }
            }

            break;
        }
        case 'G': {
            if (length == 1) {
                instance->server->sendTXT(client, instance->getColorString('G'));
            } else {
                uint8_t out = 0;

                if (strtouint8(&out, (char *) (payload + 1), nullptr, 10)) {
                    if (out < 4) {
                        char ret[9] = "G:";
                        dtostrf(*(instance->ledManager->getRGBW() + out), 1, 4, (ret + 2));
                        instance->server->sendTXT(client, ret);
                    } else {
                        instance->server->sendTXT(client, "G:E_ARG");
                    }
                } else {
                    instance->server->sendTXT(client, "G:E_FORMAT");
                }
            }

            break;
        }
        case 'N': {
            if (client < 16) {
                int time = (int) strtol((const char *) (payload + 1), nullptr, 10);
                if (time == 0 || time >= 100) {
                    instance->clientsToNotify[client] = time;
                    instance->server->sendTXT(client, "N:K");
                } else {
                    instance->server->sendTXT(client, "N:E_ARG");
                }
            }

            break;
        }
        default:
            instance->server->sendTXT(client, "E_COMMAND");
            break;
    }
}

char *WebSocketManager::getColorString(char prefix) {
    char *ret = new char[30];
    ret[0] = prefix;
    ret[1] = ':';

    dtostrf(*ledManager->r, 1, 4, (ret + 2));
    ret[8] = ' ';
    dtostrf(*ledManager->g, 1, 4, (ret + 9));
    ret[15] = ' ';
    dtostrf(*ledManager->b, 1, 4, (ret + 16));
    ret[22] = ' ';
    dtostrf(*ledManager->w, 1, 4, (ret + 23));

    return ret;
}

