#include "wifi_setup.h"
#include "firebase_setup.h"
#include "sensors.h"
#include "display.h"
#include "time_utils.h"
#include "icons.h"
#include <Servo.h>

Servo feederServo;

void setup() {
  Serial.begin(115200);
  initWiFi();
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  initFirebase();
  initServo();
  initSensors();
  initDisplay();
  showSplashScreen();
}

void loop() {
  updateTime();
  checkFirebaseSettings();
  handleFeeding();

  if (millis() - lastUpdate > updateInterval) {
    readSensors();
    sendSensorData();
    lastUpdate = millis();
  }

  handleDisplaySlides();
  delay(500);
}
