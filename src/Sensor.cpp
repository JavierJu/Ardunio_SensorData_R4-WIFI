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

  // 센서 데이터 변환
  light = map(lightLevel, 0, 1023, 100, 0);
  soil_moisture = map(soilMoistureLevel, 535, 230, 0, 100);
}