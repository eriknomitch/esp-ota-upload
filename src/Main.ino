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

// -----------------------------------------------
// GLOBALS ---------------------------------------
// -----------------------------------------------

// Lower-case version of hostname (this is set later)
String hostnameLower = "";

void ESP8266WebServer_redirect(ESP8266WebServer webServer, String path) {

  // Send the new location in the header
  webServer.sendHeader("Location", String("http://") + hostnameLower, true);

  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  webServer.send ( 302, "text/plain", ""); 

  // Stop is needed because we sent no content length
  //webServer.client().stop(); 
}

/*ESP8266WebServer::redirect(String path) {*/

/*void ESP8266WebServer::redirect(String path) {*/

    /*this.sendHeader("Location", String("http://") + hostnameLower + path, true);*/
    
    /*// Empty content inhibits Content-length header so we have to close the socket ourselves.*/
    /*this.send ( 302, "text/plain", ""); */

    /*// Stop is needed because we sent no content length*/
    /*this.client().stop(); */
/*}*/

ESP8266WebServer server(80);

// -----------------------------------------------
// GLOBALS->BLINK --------------------------------
// -----------------------------------------------
int ledState = LOW;
long previousMillis = 0;
long interval = 1000;

// -----------------------------------------------
// PUSHOVER --------------------------------------
// -----------------------------------------------
/*void sendPushNotification(String title, String message) {*/

  /*HTTPClient http;*/
  /*http.begin("http://api.pushover.net/1/messages.json");*/
  /*http.addHeader("Content-Type", "application/x-www-form-urlencoded");*/

  /*String postBody = "";*/

  /*postBody = postBody + "token=" + PUSHOVER_TOKEN;*/
  /*postBody = postBody + "&user="   + PUSHOVER_USER;*/
  /*postBody = postBody + "&device=Laptop";*/
  /*postBody = postBody + "&title="   + title;*/
  /*postBody = postBody + "&message=" + message;*/
  /*postBody = postBody + "&url=http://" + hostnameLower;*/
  /*postBody = postBody + "&url_title=" + hostnameLower;*/
  /*postBody = postBody + "&sound=none";*/

  /*http.POST(postBody);*/
  /*http.writeToStream(&Serial);*/
  /*http.end();*/
/*}*/

// -----------------------------------------------
// STATUS-LED ------------------------------------
// -----------------------------------------------
/*void setStatusLED() {*/
  /*analogWrite(LED_STATUS, 50);*/
/*}*/

// -----------------------------------------------
// SETUP->OTA ------------------------------------
// -----------------------------------------------
void setupOTA() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
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

// -----------------------------------------------
// SETUP->WEB-SERVER -----------------------------
// -----------------------------------------------
void setupWebServer() {
  server.on("/", [](){
    String output = "Hello, World.";

    server.send(200, "text/html", output);
  });

  server.on("/off", [](){
    // The builtin LED is REVERSED... HIGH is on.
    digitalWrite(BUILTIN_LED, HIGH);
    server.send(200, "text/html", "ok");
  });
  
  server.on("/on", [](){
    // The builtin LED is REVERSED... HIGH is on.
    digitalWrite(BUILTIN_LED, LOW);
    server.send(200, "text/html", "ok");
  });

  server.begin();
}

// -----------------------------------------------
// SETUP->SERIAL ---------------------------------
// -----------------------------------------------
void setupSerial() {
  Serial.begin(SERIAL_BAUD);
}

// -----------------------------------------------
// SETUP->WI-FI ----------------------------------
// -----------------------------------------------
void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_ESSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  hostnameLower = WiFi.hostname();
  hostnameLower.toLowerCase();
}

// -----------------------------------------------
// SETUP -----------------------------------------
// -----------------------------------------------
void setup() {

  digitalWrite(BUILTIN_LED, LOW);

  setupSerial();

  setupWiFi();

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
