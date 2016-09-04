// ===============================================
// OTA-UPLOAD ====================================
// ===============================================
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <env.h>
#include <utility.h>

// -----------------------------------------------
// CONSTANTS -------------------------------------
// -----------------------------------------------
#define SERIAL_BAUD 115200
#define SERVER_HTTP_PORT 80

// -----------------------------------------------
// GLOBALS ---------------------------------------
// -----------------------------------------------
ESP8266WebServer server(SERVER_HTTP_PORT);

// -----------------------------------------------
// SETUP->WEB-SERVER->ROUTES ---------------------
// -----------------------------------------------
void setupWebServerRoutes() {

  // Route: /
  // ---------------------------------------------
  server.on("/", [](){
    String output = "Hello, World.";

    server.send(200, "text/html", output);
  });
}

// -----------------------------------------------
// SETUP->WEB-SERVER -----------------------------
// -----------------------------------------------
void setupWebServer() {
  setupWebServerRoutes();
  server.begin();
  //MDNS.addService("http", "tcp", 80);
}

// -----------------------------------------------
// SETUP -----------------------------------------
// -----------------------------------------------
void setup() {

  setBuiltInLed(true);

  setupSerial(SERIAL_BAUD);

  setupWiFi();

  //setupMDNS();

  setupOTA();
  
  setupWebServer();
}

// -----------------------------------------------
// LOOP ------------------------------------------
// -----------------------------------------------
void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
