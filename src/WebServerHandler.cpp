#include "WebServerHandler.h"

const char MAINPAGE[] PROGMEM = "<!DOCTYPE html>"
        "<html lang=\"en\">"
        "<head>"
        "    <meta charset=\"UTF-8\">"
        "    <title>ESP PLOX</title>"
        "    <script type=\"text/javascript\">"
        "        var socket;"
        ""
        "        function openSocket() {"
        "            if(socket == undefined || socket.readyState == 0 || socket.readyState == 3) {"
        "                socket = new WebSocket(\"ws://\" + window.location.hostname + \":8101\");"
        ""
        "                socket.onmessage = function (event) {"
        "                    if(event.data != \"S:K\") {"
        "                        document.getElementById(\"status\").innerHTML += event.data + \"<br>\";"
        "                    }"
        "                };"
        ""
        "                socket.onclose = function (event) {"
        "                    document.getElementById(\"status\").innerHTML += \"Closed\";"
        "                    document.getElementById(\"openBtn\").innerHTML = \"Open\";"
        "                };"
        ""
        "                socket.onopen = function (event) {"
        "                    document.getElementById(\"status\").innerHTML = \"Opened<br>\";"
        "                    document.getElementById(\"openBtn\").innerHTML = \"Close\";"
        "                };"
        "            } else if(socket.readyState == 1) {"
        "                socket.close();"
        "            }"
        "        }"
        ""
        "        function sendRgb() {"
        "            if(socket.readyState == 1) {"
        "                socket.send(\"S\" + (document.getElementById(\"rr\").value / 100.0) + \" \""
        "                    + (document.getElementById(\"gr\").value / 100.0) + \" \""
        "                    + (document.getElementById(\"br\").value / 100.0));"
        "            }"
        "        }"
        "    </script>"
        "</head>"
        "<body>"
        "    <input type=\"range\" value=\"0\" step=\"0.01\" style=\"width: 500px\" id=\"rr\" oninput=\"sendRgb()\"/><br>"
        "    <input type=\"range\" value=\"0\" step=\"0.01\" style=\"width: 500px\" id=\"gr\" oninput=\"sendRgb()\"/><br>"
        "    <input type=\"range\" value=\"0\" step=\"0.01\" style=\"width: 500px\" id=\"br\" oninput=\"sendRgb()\"/><br>"
        "    <button onclick=\"openSocket()\" id=\"openBtn\">Open</button><br>"
        "    <span id=\"status\"></span>"
        "</body>"
        "</html>";

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
        server.send(200, "text/html", FPSTR(MAINPAGE));

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



