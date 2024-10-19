#include "Sensor.h"
#include <Arduino.h>

// 센서 핀 설정
#define LIGHT_SENSOR_PIN A0
#define SOIL_MOISTURE_SENSOR_PIN A1

DHT dht(DHTPIN, DHTTYPE);

void initSensors() {
  dht.begin();  // DHT 센서 초기화
}

void readSensors(float &temperature, float &humidity, uint16_t &light, uint16_t &soil_moisture) {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  int lightLevel = analogRead(LIGHT_SENSOR_PIN);
  int soilMoistureLevel = analogRead(SOIL_MOISTURE_SENSOR_PIN);

    // 빛 센서 데이터 변환
  light = map(lightLevel, 0, 1023, 100, 0);

  // 토양 수분 센서 데이터 변환 (유효성 검사 추가)
  if (soilMoistureLevel > 535 || soilMoistureLevel < 230) {
    Serial.println("Invalid soil moisture sensor reading, ignoring...");
    soil_moisture = 0; // 비정상 값은 0%로 설정
  } else {
    soil_moisture = map(soilMoistureLevel, 535, 230, 0, 100);
  }


}