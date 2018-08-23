#include "WebServerHandler.h"

bool WebServerHandler::canHandle(HTTPMethod method, String uri) {
    return true;
}

bool WebServerHandler::canUpload(String uri) {
    return false;
}

bool WebServerHandler::handle(ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri) {
    float * target;

    if (requestUri.equalsIgnoreCase("/r")) {
        target = this->lm->r;
    } else if (requestUri.equalsIgnoreCase("/g")) {
        target = this->lm->g;
    } else if (requestUri.equalsIgnoreCase("/b")) {
        target = this->lm->b;
    } else if (requestUri.equalsIgnoreCase("/a")) {
        this->handle(server, requestMethod, "/r");
        this->handle(server, requestMethod, "/g");
        this->handle(server, requestMethod, "/b");

        return true;
    } else if (requestUri.equalsIgnoreCase("/m")) {
        if (server.args() > 0) {
            this->lm->setMode(static_cast<LedMode>(server.arg(0).toInt()));
            //this->lm->setMode(LedMode::SIN_RAINBOW);
            server.send(200);
        } else {
            server.send(400);
        }

        return true;
    } else if (requestUri.equals("/")) {
        File file = SPIFFS.open("/www/index.html", "r");
        server.streamFile(file, "text/html");
        file.close();

        return true;
    } else {
        server.send(404, "text/plain", "Not found. Try /, /r, /g or /b");
        return true;
    }

    if (requestMethod == HTTPMethod::HTTP_GET) {
        server.send(200, "text/plain", String(*target));
    } else if (requestMethod == HTTPMethod::HTTP_POST) {
        if (server.args() > 0) {
            *target = server.arg(0).toFloat();

            if (*target > 1) *target = 1;
            if (*target < 0) *target = 0;

            server.send(200, "text/plain", String(*target));
        } else {
            server.send(400);
        }
    } else {
        server.send(405);
    }

    return true;
}

void WebServerHandler::upload(ESP8266WebServer &server, String requestUri, HTTPUpload &upload) {
    RequestHandler::upload(server, requestUri, upload);
}



