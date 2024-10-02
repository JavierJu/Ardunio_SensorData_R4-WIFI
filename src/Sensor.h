#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>  // uint16_t를 사용하기 위해 추가

#include <DHT.h>

// DHT22 센서 설정
#define DHTPIN 7
#define DHTTYPE DHT22
extern DHT dht;

void initSensors();
void readSensors(float &temperature, float &humidity, uint16_t &light, uint16_t &soil_moisture);

#endif