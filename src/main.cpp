#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Sensor.h"
#include "WiFiConnection.h"
#include "DataTransmission.h"

// OLED 설정
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Wi-Fi 정보
const char* ssid = "TP-Link_C6A8_JU";
const char* password = "Javierju12";

// 서버 정보
const char* serverIP = "35.213.49.212";
const int serverPort = 80;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 600000; // 1시간 (3,600,000 밀리초)// 1분 (60,000 밀리초) // 10분 (600,000 밀리초) 

void setup() {
  Serial.begin(9600);

  // 센서 및 디스플레이 초기화
  initSensors();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Wi-Fi 연결
  connectToWiFi(ssid, password);
}

void loop() {
  unsigned long currentMillis = millis();
    
  if (currentMillis - lastSendTime >= sendInterval) {
      float temperature, humidity;
      uint16_t light, soil_moisture;

      // 센서 데이터 읽기
      readSensors(temperature, humidity, light, soil_moisture);

      // OLED 디스플레이에 데이터 표시
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Sensor Data");
      display.setCursor(0, 10);
      display.print("Temp: ");
      display.print(temperature);
      display.println(" C");
      display.setCursor(0, 20);
      display.print("Humi: ");
      display.print(humidity);
      display.println(" %");
      display.setCursor(0, 30);
      display.print("Light: ");
      display.print(light);
      display.println(" %");
      display.setCursor(0, 40);
      display.print("Soil Moist: ");
      display.print(soil_moisture);
      display.println(" %");
      display.display();

      // 데이터 전송
      sendDataToServer(temperature, humidity, light, soil_moisture, serverIP, serverPort);

      // 마지막 전송 시간 갱신
      lastSendTime = currentMillis;
  }

  delay(1000);

  // float temperature, humidity;
  // uint16_t light, soil_moisture;

  // // 센서 데이터 읽기
  // readSensors(temperature, humidity, light, soil_moisture);

  // // OLED 디스플레이에 데이터 표시
  // display.clearDisplay();
  // display.setCursor(0, 0);
  // display.println("Sensor Data");
  // display.setCursor(0, 10);
  // display.print("Temp: ");
  // display.print(temperature);
  // display.println(" C");
  // display.setCursor(0, 20);
  // display.print("Humi: ");
  // display.print(humidity);
  // display.println(" %");
  // display.setCursor(0, 30);
  // display.print("Light: ");
  // display.print(light);
  // display.println(" %");
  // display.setCursor(0, 40);
  // display.print("Soil Moist: ");
  // display.print(soil_moisture);
  // display.println(" %");
  // display.display();
  
  // // 데이터 전송
  // unsigned long currentMillis = millis();
  // if (currentMillis - lastSendTime >= sendInterval) {
  //   sendDataToServer(temperature, humidity, light, soil_moisture, serverIP, serverPort);
  //   lastSendTime = currentMillis;
  // }
  // delay(1000); 
}