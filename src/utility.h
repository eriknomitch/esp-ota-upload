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

