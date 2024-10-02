#include "DataTransmission.h"
#include <WiFiS3.h>
#include <Arduino.h>

WiFiClient client;

void sendDataToServer(float temperature, float humidity, uint16_t light, uint16_t soil_moisture, const char* serverIP, int serverPort) {
  if (client.connect(serverIP, serverPort)) {
    String httpRequestData = "temperature=" + String(temperature)
                           + "&humidity=" + String(humidity)
                           + "&light=" + String(light)
                           + "&soil_moisture=" + String(soil_moisture);
    
    client.println("POST /SmartFarm_test/server/process_R4.php HTTP/1.1");
    client.print("Host: "); client.println(serverIP);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: "); client.println(httpRequestData.length());
    client.println();
    client.println(httpRequestData);

    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    
    client.stop();
  } else {
    Serial.println("Connection to server failed");
  }
}