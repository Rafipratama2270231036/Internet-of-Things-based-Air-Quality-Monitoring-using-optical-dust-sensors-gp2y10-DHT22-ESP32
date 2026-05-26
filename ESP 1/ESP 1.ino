#include <GP2YDustSensor.h>
#include <DHT.h>

const uint8_t SHARP_LED_PIN = 2;
const uint8_t SHARP_VO_PIN = 34;
#define DHTPIN 26
#define DHTTYPE DHT22

GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, SHARP_LED_PIN, SHARP_VO_PIN);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200, SERIAL_8N1, 3, 1);
  
  dustSensor.begin();
  dht.begin();
  
  Serial.println("ESP32 Siap");
  Serial.println("=================================");
}

void loop() {
  float pm25 = dustSensor.getDustDensity();
  
  if (pm25 < 0) pm25 = 0;
  if (pm25 > 500) pm25 = 500;
  
  float suhu = dht.readTemperature();
  float hum = dht.readHumidity();
  if (isnan(suhu) || isnan(hum)) {
    suhu = 0;
    hum = 0;
  }
  
  Serial2.print(pm25, 0);
  Serial2.print(",");
  Serial2.print(suhu, 1);
  Serial2.print(",");
  Serial2.println(hum, 1);
  
  Serial.print("Dust density: ");
  Serial.print(pm25, 0);
  Serial.print(" ug/m3; Running average: ");
  Serial.print(dustSensor.getRunningAverage());
  Serial.println(" ug/m3");
  delay(1000);
}