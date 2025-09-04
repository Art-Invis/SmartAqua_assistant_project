#pragma once
#include <time.h>

char currentTime[6];
String scheduledTime = "--:--";
String activeStart = "08:00";
String activeEnd = "20:00";
bool hasFedThisMinute = false;
unsigned long lastUpdate = 0;

extern FirebaseData fbdo;
extern Servo feederServo;

void updateTime() {
  time_t now = time(nullptr);
  struct tm* timeInfo = localtime(&now);
  snprintf(currentTime, sizeof(currentTime), "%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min);
}

void checkFirebaseSettings() {
  if (Firebase.getString(fbdo, "/Feeder/ScheduledTime"))
    scheduledTime = fbdo.stringData().substring(0, 5);

  if (Firebase.getString(fbdo, "/Feeder/ActiveHours/Start"))
    activeStart = fbdo.stringData().substring(0, 5);

  if (Firebase.getString(fbdo, "/Feeder/ActiveHours/End"))
    activeEnd = fbdo.stringData().substring(0, 5);
}

void handleFeeding() {
  String nowStr = String(currentTime);
  bool isActive = nowStr >= activeStart && nowStr <= activeEnd;

  if (nowStr == scheduledTime && !hasFedThisMinute && isActive) {
    feedFish("Auto");
    hasFedThisMinute = true;
  }

  if (nowStr != scheduledTime) hasFedThisMinute = false;

  if (Firebase.getString(fbdo, "/Feeder/Status") && fbdo.stringData() == "Feed") {
    feedFish("Manual");
    Firebase.setString(fbdo, "/Feeder/Status", "Idle");
  }
}

void feedFish(String mode) {
  Serial.println("🐟 Feeding...");
  feederServo.write(180);
  delay(1500);
  feederServo.write(0);
  delay(500);

  time_t now = time(nullptr);
  String timestamp = String(now);
  Firebase.setString(fbdo, "/Feeder/FeedLog/" + timestamp, "Fed");

  if (mode == "Auto") Firebase.setString(fbdo, "/Feeder/LastFedAuto", timestamp);
  if (mode == "Manual") Firebase.setString(fbdo, "/Feeder/LastFedManual", timestamp);
}
