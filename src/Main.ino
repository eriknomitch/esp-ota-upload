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

// -----------------------------------------------
// CONSTANTS -------------------------------------
// -----------------------------------------------
#define ESP_WITTY_LED_RED   15
#define ESP_WITTY_LED_GREEN 12
#define ESP_WITTY_LED_BLUE  13

// -----------------------------------------------
// GLOBALS ---------------------------------------
// -----------------------------------------------

// Lower-case version of hostname (this is set later)
String hostnameLower = "";

ESP8266WebServer server(80);

// -----------------------------------------------
// WEB-SERVER->PATH-HANDLERS ---------------------
// -----------------------------------------------
void handleRoot() {

  String output = "hostname: ";

  output = output + "<a href='http://";
  output = output + hostnameLower;
  output = output + "'>";
  output = output + hostnameLower;
  output = output + "</a>";
  
  output = output + "</br>";
  output = output + "</br>";
  
  output = output + "<a href='/on'>ON</a>";
  
  output = output + "</br>";
  output = output + "</br>";
  
  output = output + "<a href='/off'>OFF</a>";

  server.send(200, "text/html", output);
}

void sendPushNotification(String title, String message) {

  HTTPClient http;
  http.begin("http://api.pushover.net/1/messages.json");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String postBody = "";

  postBody = postBody + "token=" + PUSHOVER_TOKEN;
  postBody = postBody + "&user="   + PUSHOVER_USER;
  postBody = postBody + "&device=Laptop";
  postBody = postBody + "&title="   + title;
  postBody = postBody + "&message=" + message;
  postBody = postBody + "&url=http://" + hostnameLower;
  postBody = postBody + "&url_title=" + hostnameLower;
  postBody = postBody + "&sound=none";

  http.POST(postBody);
  http.writeToStream(&Serial);
  http.end();
}

// -----------------------------------------------
// SETUP -----------------------------------------
// -----------------------------------------------
void setup() {

  pinMode(ESP_WITTY_LED_RED,   OUTPUT);
  pinMode(ESP_WITTY_LED_GREEN, OUTPUT);
  pinMode(ESP_WITTY_LED_BLUE,  OUTPUT);

  pinMode(BUILTIN_LED, OUTPUT);

  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_ESSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

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
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/on", [](){
    digitalWrite(ESP_WITTY_LED_GREEN, HIGH);

    server.send(200, "text/html", "Okay.");
  });
  
  server.on("/off", [](){
    digitalWrite(ESP_WITTY_LED_GREEN, LOW);

    server.send(200, "text/html", "Okay.");
  });

  server.begin();

  hostnameLower = WiFi.hostname();
  hostnameLower.toLowerCase();

  sendPushNotification(hostnameLower, WiFi.localIP().toString());
  
  digitalWrite(BUILTIN_LED, LOW);
}

// -----------------------------------------------
// LOOP ------------------------------------------
// -----------------------------------------------
void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
