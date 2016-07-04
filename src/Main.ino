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
#define ESP_WITTY_LED_RED   15
#define ESP_WITTY_LED_GREEN 12
#define ESP_WITTY_LED_BLUE  13

#define LED_STATUS ESP_WITTY_LED_GREEN

#define LUMINANCE_MAX 730.0
#define LUMINANCE_MIN 30.0

#define ANALOG_MAX 1024.0

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
// WEB-SERVER->PATH-HANDLERS ---------------------
// -----------------------------------------------
void handleRoot() {

  String output = "<!DOCTYPE> <html> <head> <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.0.0/jquery.min.js\"></script> <script> $.ajax({ url: \"http://server/esp8266/main.html\", method: \"GET\", success: function(data) { var newDoc = document.open(\"text/html\", \"replace\"); newDoc.write(data); newDoc.close(); } }); </script> </head> </html>";

  server.sendHeader("Access-Control-Allow-Origin", "server", true);

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

  pinMode(A0, INPUT);

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

    //ESP8266WebServer_redirect(server, "");
    server.send(200, "text/html", "Okay.");

  });
  
  server.on("/luminance", [](){

    String luminance = String(analogRead(A0));

    server.send(200, "text/plain", luminance);
  });

  server.on("/rainbow", [](){
    for (int i = 0; i < 20; i++) {
      pinHigh(ESP_WITTY_LED_GREEN);
      delay(50);
      pinLow(ESP_WITTY_LED_GREEN);
      pinHigh(ESP_WITTY_LED_BLUE);
      delay(50);
      pinLow(ESP_WITTY_LED_BLUE);
      pinHigh(ESP_WITTY_LED_RED);
      delay(50);
      pinLow(ESP_WITTY_LED_RED);
    }

    server.send(200, "text/html", "Okay.");
  });

  server.begin();

  hostnameLower = WiFi.hostname();
  hostnameLower.toLowerCase();

  sendPushNotification(hostnameLower, WiFi.localIP().toString());
  
  digitalWrite(BUILTIN_LED, LOW);
}

// -----------------------------------------------
// -----------------------------------------------
// -----------------------------------------------
void handleBlink() {
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    ledState = ledState == LOW ? HIGH : LOW;

    analogWrite(LED_STATUS, ledState == LOW ? 0 : 50);
  }
}

// -----------------------------------------------
// LOOP ------------------------------------------
// -----------------------------------------------
void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  handleBlink();
}
