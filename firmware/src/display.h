#pragma once
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "icons.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int bubbleY = 50;
bool bubbleUp = true;
int currentSlide = 0;

void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.clearDisplay();
}

void showSplashScreen() {
  display.clearDisplay();
  display.drawBitmap(48, 0, epd_bitmap_fishIcon, 32, 32, SSD1306_WHITE);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.println("Aquassistant");
  display.display();
  delay(3000);
  display.clearDisplay();
}

void fadeOut() {
  for (int i = 0; i < 5; i++) {
    display.dim(true);
    display.display();
    delay(30);
  }
  display.clearDisplay();
}

void fadeIn() {
  display.dim(false);
  display.display();
}

void handleDisplaySlides() {
  static unsigned long slideLastChange = 0;
  if (millis() - slideLastChange > 5000) {
    fadeOut();
    currentSlide = (currentSlide + 1) % 5;
    slideLastChange = millis();
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  switch (currentSlide) {
    case 0:
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Temperature");
      display.drawBitmap(0, 16, epd_bitmap_tempIcon, 32, 32, SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(40, 24);
      display.print(temperature, 1);
      display.println(" C");
      break;

    case 1:
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Water Level");
      display.drawBitmap(0, 16, epd_bitmap_waterIcon, 32, 32, SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(40, 24);
      display.print(waterLevel);
      display.println(" %");
      display.fillCircle(110, bubbleY, 2, SSD1306_WHITE);
      if (bubbleUp) {
        bubbleY--;
        if (bubbleY < 40) bubbleUp = false;
      } else {
        bubbleY++;
        if (bubbleY > 52) bubbleUp = true;
      }
      break;

    case 2:
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Current Time");
      display.setTextSize(2);
      display.setCursor(20, 25);
      display.println(currentTime);
      break;

    case 3:
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Next Feeding");
      display.setTextSize(2);
      display.setCursor(20, 25);
      display.println(scheduledTime);
      break;

    case 4:
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Wi-Fi Status");
      display.drawBitmap(0, 16, epd_bitmap_statusIcon, 32, 32, SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(40, 25);
      display.println(firebaseStatus ? "Online" : "Offline");
      break;
  }

  display.display();
}
