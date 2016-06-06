#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Toad2G";
const char* password = "Madmen!!";

#define LED_PIN 5

void sendPushNotification(String title, String message) {
  HTTPClient http;
  http.begin("http://api.pushover.net/1/messages.json");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String postBody = "token=a6WvGcj39uyCnDbL7h4G5zM7HjFF7P&user=ugof9hrmsSEfhkhMmNHwrYAtPvKYuZ&device=Laptop";

  postBody = postBody + "&title="   + title;
  postBody = postBody + "&message=" + message;

  http.POST(postBody);
  http.writeToStream(&Serial);
  http.end();
}

void setup() {

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
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

  sendPushNotification(WiFi.hostname(), WiFi.localIP().toString());

}

void loop() {
  ArduinoOTA.handle();

  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(LED_PIN, LOW);
  delay(500);

}
