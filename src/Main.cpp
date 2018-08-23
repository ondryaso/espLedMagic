#include "Main.h"
#include "AsyncWebServerManager.h"

const char* ssid = "EvilWiFi";
const char* password = "12345678998765432112345";

LedMan ledManager = LedMan();
AsyncWebServerManager s = AsyncWebServerManager(&ledManager);
WebSocketManager ws = WebSocketManager(&ledManager);

void setup() {    
    Serial.begin(115200);

    Serial.println("Connecting to WiFi and initializing");

    initWifi();
    initOTA();
    initServer();
    initWS();

    Serial.println("Ready");

    analogWriteRange(4095);
    analogWriteFreq(2000);

    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D8, OUTPUT);
}

void loop() {
    ArduinoOTA.handle();

    ledManager.calculateStep(millis());
    
    analogWrite(D1, (int)((*ledManager.r) * 4095));
    analogWrite(D2, (int)((*ledManager.g) * 4095));
    analogWrite(D3, (int)((*ledManager.b) * 4095));
    analogWrite(D8, (int) ((*ledManager.w) * 4095));

    ws.notifyClients();
}

void initWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void initOTA() {
    ArduinoOTA.onStart([]() {
        Serial.println("Start");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();
}

void initServer() {
    s.init();
}

void initWS() {
    ws.init();
}