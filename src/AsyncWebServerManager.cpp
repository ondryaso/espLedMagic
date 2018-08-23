#include "AsyncWebServerManager.h"

AsyncWebServerManager::AsyncWebServerManager(LedMan * ledManager) {
    this->server = new AsyncWebServer(80);
    this->ledMan = ledManager;
}

void AsyncWebServerManager::init() {
    SPIFFS.begin();

    server->rewrite("/", "/index.html");
    server->serveStatic("/", SPIFFS, "/www/");

    server->onNotFound(std::bind(&AsyncWebServerManager::onNotFound, this, std::placeholders::_1));

    server->begin();
}

void AsyncWebServerManager::onNotFound(AsyncWebServerRequest *request){
    auto resp = request->beginResponse(SPIFFS, "/notFound.html", "text/html");
    resp->setCode(404);
    request->send(resp);
}