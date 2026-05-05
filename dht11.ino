/*
 * DHT11 Temperature & Humidity Readout
 * -------------------------------------
 * Reads temperature and humidity from a DHT11 sensor by speaking
 * its single-wire protocol directly — no external library required.
 *
 * Target board: Arduino Uno (or any 5V AVR Arduino at 16 MHz)
 * Wiring (3-pin DHT11 module):
 *   DHT11 VCC  -> Arduino 5V
 *   DHT11 GND  -> Arduino GND
 *   DHT11 DATA -> Arduino D2
 *
 * Open Serial Monitor at 9600 baud after upload.
 *
 * (c) 2026 Circuitrocks. Released under the MIT License.
 * https://github.com/circuitrocks
 */

const uint8_t DATA_PIN = 2;

// Reads one full DHT11 frame (40 bits = 5 bytes) into 'humidity' and
// 'temperature'. Returns true on success, false on timeout/checksum error.
bool readDHT11(uint8_t *humidity, uint8_t *temperature) {
  uint8_t bits[5] = {0, 0, 0, 0, 0};

  // 1. Send start signal: pull DATA low for at least 18 ms, then high briefly.
  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(DATA_PIN, LOW);
  delay(20);
  digitalWrite(DATA_PIN, HIGH);
  delayMicroseconds(30);

  // 2. Switch to input and wait for the sensor's response:
  //    line goes LOW (~80us), then HIGH (~80us), then the data starts.
  pinMode(DATA_PIN, INPUT_PULLUP);

  uint32_t t = micros();
  while (digitalRead(DATA_PIN) == HIGH) {
    if (micros() - t > 150) return false;
  }
  t = micros();
  while (digitalRead(DATA_PIN) == LOW) {
    if (micros() - t > 150) return false;
  }
  t = micros();
  while (digitalRead(DATA_PIN) == HIGH) {
    if (micros() - t > 150) return false;
  }

  // 3. Read 40 bits. Each bit is:
  //    LOW for ~50us, then HIGH for ~28us (= 0) or ~70us (= 1).
  for (uint8_t i = 0; i < 40; i++) {
    t = micros();
    while (digitalRead(DATA_PIN) == LOW) {
      if (micros() - t > 150) return false;
    }
    uint32_t highStart = micros();
    while (digitalRead(DATA_PIN) == HIGH) {
      if (micros() - highStart > 150) return false;
    }
    uint32_t highDur = micros() - highStart;
    bits[i / 8] <<= 1;
    if (highDur > 40) bits[i / 8] |= 1;
  }

  // 4. Verify checksum: byte 4 should equal sum of bytes 0..3 (low byte only).
  uint8_t checksum = bits[0] + bits[1] + bits[2] + bits[3];
  if (checksum != bits[4]) return false;

  *humidity    = bits[0];   // integer % RH; bits[1] is decimals (DHT11 always 0)
  *temperature = bits[2];   // integer C;    bits[3] is decimals (DHT11 always 0)
  return true;
}

void setup() {
  Serial.begin(9600);
  pinMode(DATA_PIN, INPUT_PULLUP);
  delay(1000);   // sensor needs ~1s after power-up to settle
  Serial.println(F("DHT11 ready."));
  Serial.println(F("Humidity (%)\tTemp (C)"));
}

void loop() {
  uint8_t humidity, temperature;
  if (readDHT11(&humidity, &temperature)) {
    Serial.print(humidity);
    Serial.print(F("\t\t"));
    Serial.println(temperature);
  } else {
    Serial.println(F("Read failed (timeout or checksum error)."));
  }
  delay(2000);   // DHT11 max sample rate is 1 Hz; 2s is safe
}
