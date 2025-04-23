#define LED_PIN 3  // LED an Pin D3
#define LED_BRIGHTNESS 10  // LED-Helligkeit (0-255), hier z.B. 80 für reduzierte Helligkeit

#include "globals.h"
#include "nrf.h"
#include "joysticks.h"

void setup() {
  // LED-Pin als Ausgang definieren
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(19200);
  while (!Serial) {}

  // Warte 2 Sekunden zum Starten
  delay(2000);

  bool initError = false;  // Fehlerflag, falls Initialisierung fehlschlägt

  // Während des Setups: Langsam blinken lassen mit reduzierter Helligkeit
  for (int i = 0; i < 3; i++) {
    analogWrite(LED_PIN, LED_BRIGHTNESS);
    delay(200);
    analogWrite(LED_PIN, 0);
    delay(200);
  }

  Serial.println(F("Init Hardware..."));

  // Versuche, den NRF zu initialisieren
  if (!radio.begin()) {  // Falls die NRF-Hardware nicht reagiert
    Serial.println(F("NRF is not responding!"));
    initError = true;
    // Schnelles Blinken als Fehleranzeige
    for (int i = 0; i < 5; i++) {
      analogWrite(LED_PIN, LED_BRIGHTNESS);
      delay(100);
      analogWrite(LED_PIN, 0);
      delay(100);
    }
  }

  // Weitere NRF-Konfiguration
  init_nrf();
  delay(50);
  Serial.println(F("NRF initialized!"));

  // Joysticks initialisieren
  init_joysticks();

  // Falls ein Fehler bei der Initialisierung festgestellt wurde,
  // schnelles Blinken als visuelles Feedback
  if (initError) {
    for (int i = 0; i < 5; i++) {
      analogWrite(LED_PIN, LED_BRIGHTNESS);
      delay(100);
      analogWrite(LED_PIN, 0);
      delay(100);
    }
  }

  // Initiale Verbindung zum Roboter herstellen
  tm_resp = send_remote_data();
}

void loop() {
  // In der Loop soll die LED dauerhaft mit reduzierter Helligkeit leuchten:
  analogWrite(LED_PIN, LED_BRIGHTNESS);

  // Überprüfe in regelmäßigen Abständen den Status der Steuerelemente
  if (millis() - controlsUpdate > controlsInterval) {
    check_joysticks();
  }

  // Sende in regelmäßigen Abständen die Remote-Daten
  if (millis() - lastNRFUpdate > nrfInterval) {
    tm_resp = send_remote_data();
  }
}
