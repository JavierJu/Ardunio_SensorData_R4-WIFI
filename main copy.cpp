#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiS3.h>

// Replace with your network credentials
const char* ssid = "TP-Link_C6A8_JU";
const char* password = "Javierju12";

// Replace with your server IP and PHP file
const char* serverIP = "35.213.49.212"; // IP 주소
const int serverPort = 80; // 기본 HTTP 포트

// DHT22 Sensor configuration
#define DHTPIN 7        // 온습도 센서 핀
#define DHTTYPE DHT22   // DHT22(AM2302) 센서 타입
DHT dht(DHTPIN, DHTTYPE);

// Sensor pin configuration
#define LIGHT_SENSOR_PIN A0         // 빛 센서 핀
#define SOIL_MOISTURE_SENSOR_PIN A1 // 토양 수분 센서 핀

// OLED display width and height, in pixels
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Reset pin (or -1 if sharing Arduino reset pin)
#define OLED_RESET -1

// Create an SSD1306 display object connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi client object for HTTP requests
WiFiClient client;
unsigned long lastSendTime = 0; // 타이머를 위한 변수
const unsigned long sendInterval = 60000; // 1시간 (3,600,000 밀리초)// 1분 (60,000 밀리초)

void setup() {
  Serial.begin(9600);

  // DHT 센서 초기화
  dht.begin();

  // OLED 디스플레이 초기화
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // 무한 루프 (OLED 초기화 실패시)
  }
  
  display.display();
  delay(2000); // 2초 대기 후 디스플레이 초기화 완료
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Wi-Fi 연결
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  // 센서 데이터 읽기
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  int lightLevel = analogRead(LIGHT_SENSOR_PIN);
  int soilMoistureLevel = analogRead(SOIL_MOISTURE_SENSOR_PIN);

  // 센서 데이터 변환 (예: 0-1023 범위를 0-100% 범위로 변환)
  uint16_t light = map(lightLevel, 0, 1023, 100, 0);
  uint16_t soil_moisture = map(soilMoistureLevel, 524, 233, 0, 100);

  // DHT 센서 오류 체크
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // OLED에 센서 데이터 표시
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

  // 현재 시간 가져오기
  unsigned long currentMillis = millis();

  // 1시간마다 데이터 전송
  if (currentMillis - lastSendTime >= sendInterval) {
    // WiFi 연결 상태 확인 후 서버로 데이터 전송
    if (WiFi.status() == WL_CONNECTED) {
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
        lastSendTime = currentMillis; // 마지막 전송 시간 갱신
      } else {
        Serial.println("Connection to server failed");
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
  
  delay(2000); // 데이터를 2초마다 갱신
}