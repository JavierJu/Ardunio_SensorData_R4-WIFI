#ifndef DATA_TRANSMISSION_H
#define DATA_TRANSMISSION_H

#include <stdint.h>  // uint16_t를 사용하기 위해 추가

void sendDataToServer(float temperature, float humidity, uint16_t light, uint16_t soil_moisture, const char* serverIP, int serverPort);

#endif