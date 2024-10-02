#include "WiFiConnection.h"
#include <WiFiS3.h>
#include <Arduino.h>

void connectToWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}