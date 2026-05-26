#define BLYNK_HEARTBEAT 60
#define BLYNK_TEMPLATE_ID   "TMPL66bhsPYs"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitoring"
#define BLYNK_AUTH_TOKEN    "jq_DzRonZTV8M5yWA2UzKZkFH5tpjhj4"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ThingSpeak.h>
#include "time.h"

WiFiClient client;

const char* ssid     = "rpi.pratma";
const char* password = "#Spekkonten1";

unsigned long myChannelNumber = 3381106;
const char* myWriteAPIKey = "HB3SYW4V4P6JLXIY";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 0;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float pm25 = 0, suhu = 0, hum = 0;
unsigned long lastDataTime = 0;
bool dataReceived = false;

unsigned long lastThingSpeakTime = 0;
const unsigned long thingSpeakInterval = 15000;

unsigned long lastDisplayUpdate = 0;
const unsigned long displayUpdateInterval = 1000;

BlynkTimer timerBlynk;
const unsigned long blynkSendInterval = 90000; 

bool oledEnabled = true;

String getFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "00:00:00";
  char buffer[9];
  strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
  return String(buffer);
}

void updateDisplay() {
  if (!oledEnabled) return;
  
  String currentTime = getFormattedTime();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("PM2.5: "); display.print(pm25, 0); display.println(" ug/m3");
  display.setCursor(0, 10);
  display.print("Temp: "); display.print(suhu, 1); display.println(" C");
  display.setCursor(0, 20);
  display.print("Hum: "); display.print(hum, 1); display.println(" %");
  display.setCursor(0, 30);
  display.print("Time: "); display.print(currentTime);
  display.display();
}

void kirimKeBlynk() {
  if (dataReceived) {
    Blynk.virtualWrite(V1, pm25);
    Blynk.virtualWrite(V2, suhu);
    Blynk.virtualWrite(V3, hum);
    Serial.println("Data dikirim ke Blynk (periodik 90 detik)");
  } else {
    Serial.println("Belum ada data, Blynk skip");
  }
}

BLYNK_WRITE(V4) {
  oledEnabled = param.asInt();
  if (oledEnabled) {
    updateDisplay();
  } else {
    display.clearDisplay();
    display.display();
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 3, 1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED init gagal!");
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("ESP32 #2 Ready.....");
  display.display();
  delay(1000);

  // WiFi & Blynk
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected, IP: " + WiFi.localIP().toString());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Sinkronisasi waktu dari NTP...");

  ThingSpeak.begin(client);

  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();

  timerBlynk.setInterval(blynkSendInterval, kirimKeBlynk);

  display.clearDisplay();
  display.println("System Ready");
  display.display();
  delay(500);

  lastDataTime = millis();
  lastDisplayUpdate = millis();
}

void loop() {
  Blynk.run();
  timerBlynk.run();

  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    data.trim();
    int c1 = data.indexOf(',');
    int c2 = data.indexOf(',', c1+1);
    if (c1 != -1 && c2 != -1) {
      pm25 = data.substring(0, c1).toFloat();
      suhu = data.substring(c1+1, c2).toFloat();
      hum  = data.substring(c2+1).toFloat();
      dataReceived = true;
      lastDataTime = millis();

      Serial.printf("Data terima: %.0f,%.1f,%.1f\n", pm25, suhu, hum);
    }
  } else if (millis() - lastDataTime > 5000 && !dataReceived) {
    if (oledEnabled) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("No data from");
      display.println("ESP32 #1");
      display.println("Please check ESP32 #1");
      display.display();
    }
  }

  if (dataReceived && (millis() - lastDisplayUpdate >= displayUpdateInterval)) {
    lastDisplayUpdate = millis();
    if (oledEnabled) updateDisplay();
  } else if (!dataReceived && (millis() - lastDisplayUpdate >= 3000)) {
    lastDisplayUpdate = millis();
    if (oledEnabled) {
      String currentTime = getFormattedTime();
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Waiting data...");
      display.setCursor(0, 30);
      display.print("Time: "); display.print(currentTime);
      display.display();
    }
  }

  if (WiFi.status() == WL_CONNECTED && dataReceived) {
    if (millis() - lastThingSpeakTime >= thingSpeakInterval) {
      lastThingSpeakTime = millis();
      ThingSpeak.setField(1, pm25);
      ThingSpeak.setField(2, suhu);
      ThingSpeak.setField(3, hum);
      int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      if (httpCode == 200) Serial.println("Data terkirim ke ThingSpeak");
      else Serial.println("Gagal kirim ke ThingSpeak. HTTP: " + String(httpCode));
    }
  }

  delay(10);
}