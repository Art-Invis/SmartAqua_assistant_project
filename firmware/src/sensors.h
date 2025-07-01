#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define WATER_PIN A0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float temperature = 0.0;
int waterLevel = 0;

void initSensors() {
  sensors.begin();
}

void readSensors() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  int rawWater = analogRead(WATER_PIN);
  waterLevel = constrain(map(rawWater, 1023, 0, 0, 100), 0, 100);

  time_t now = time(nullptr);
  struct tm* timeInfo = localtime(&now);
  Serial.printf("📅 %02d:%02d | 🌡 %.1f °C | 💧 %d %%\n", timeInfo->tm_hour, timeInfo->tm_min, temperature, waterLevel);
}

void sendSensorData() {
  time_t now = time(nullptr);
  String timestamp = String(now);
  firebaseStatus = Firebase.ready();

  if (firebaseStatus) {
    Firebase.setFloat(fbdo, "/SensorHistory/" + timestamp, temperature);
    Firebase.setInt(fbdo, "/SensorWaterHistory/" + timestamp, waterLevel);

    if (temperature < 15 || temperature > 32)
      Firebase.setString(fbdo, "/Feeder/Alert", "Temperature out of range!");
    else if (waterLevel < 30)
      Firebase.setString(fbdo, "/Feeder/Alert", "Low water level!");
    else
      Firebase.setString(fbdo, "/Feeder/Alert", "OK");
  }
}
