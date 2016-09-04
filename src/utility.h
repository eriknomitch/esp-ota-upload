// ===============================================
// UTILITY =======================================
// ===============================================
#define arrayLength(x) (sizeof(x) / sizeof(*x))

// -----------------------------------------------
// PINS ------------------------------------------
// -----------------------------------------------
void pinHigh(int pin) {
  digitalWrite(pin, HIGH);
}

void pinLow(int pin) {
  digitalWrite(pin, LOW);
}

void pinToggle(int pin) {
  digitalWrite(pin, !digitalRead(pin));
}

bool pinSet(int pin, bool state) {
  if (state) {
        pinHigh(pin);
        return true;
      }
  pinLow(pin);
  return false;
}

void pinsOutput(int pins[]) {
  for (int i = 0; i < arrayLength(pins); i++) {
      pinMode(pins[i], OUTPUT);
    }
}

// -----------------------------------------------
// BLINKING --------------------------------------
// -----------------------------------------------
void blink(int pin, int blinkMs, int blinkTimes) {
  for (int i = 0; i < blinkTimes; i++) {
        pinHigh(pin);
        delay(blinkMs);
        pinLow(pin);
        delay(blinkMs);
      }
}

void blinkWithStartState(int pin, int blinkMs, int blinkTimes, bool startState) {
  for (int i = 0; i < blinkTimes; i++) {
      pinSet(pin, !startState);
      delay(blinkMs);
      pinSet(pin, startState);
      delay(blinkMs);
    }
}

// -----------------------------------------------
// SERIAL ----------------------------------------
// -----------------------------------------------

// WARNING: I believe this will only work in screen.
void clearSerial() {
  Serial.write(27); // Print 'esc'
  Serial.print("[2J");
}

// -----------------------------------------------
// WEB-SERVER ------------------------------------
// -----------------------------------------------
//void ESP8266WebServer_redirect(ESP8266WebServer webServer, String path) {

  //// Send the new location in the header
  //webServer.sendHeader("Location", String("http://") + hostnameLower, true);

  //// Empty content inhibits Content-length header so we have to close the socket ourselves.
  //webServer.send ( 302, "text/plain", ""); 

  //// Stop is needed because we sent no content length
  ////webServer.client().stop(); 
//}

/*ESP8266WebServer::redirect(String path) {*/

/*void ESP8266WebServer::redirect(String path) {*/

    /*this.sendHeader("Location", String("http://") + hostnameLower + path, true);*/
    
    /*// Empty content inhibits Content-length header so we have to close the socket ourselves.*/
    /*this.send ( 302, "text/plain", ""); */

    /*// Stop is needed because we sent no content length*/
    /*this.client().stop(); */
/*}*/

// -----------------------------------------------
// MDNS ------------------------------------------
// -----------------------------------------------
void setupMDNS() {
  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
    /*
  if (!MDNS.begin(hostnameLower)) {
    Serial.println("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  } else {
    Serial.println("mDNS responder started");
  }
    */
}


// -----------------------------------------------
// OTA -------------------------------------------
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
// SERIAL ----------------------------------------
// -----------------------------------------------
void setupSerial(int baudRate) {
  Serial.begin(baudRate);
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

  //hostnameLower = WiFi.hostname();
  //hostnameLower.toLowerCase();
}

// ===============================================
// ESP ===========================================
// ===============================================

// -----------------------------------------------
// LED -------------------------------------------
// -----------------------------------------------
void setBuiltInLed(bool state) {
  if (state) {
    digitalWrite(BUILTIN_LED, LOW);
    return;
  }
  digitalWrite(BUILTIN_LED, HIGH);
}
