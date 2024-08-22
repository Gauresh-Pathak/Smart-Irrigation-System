#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define SOIL_MOISTURE_PIN 34
#define PUMP_PIN 26

DHT dht(DHTPIN, DHTTYPE);

// thresholds
int dryThreshold = 30;
int wetThreshold = 70;
bool pumpRunning = false;

void pumpOn(int moisture, float temp) {
  digitalWrite(PUMP_PIN, HIGH);
  pumpRunning = true;
  Serial.println("Pump ON | Moisture: " + String(moisture) + "% | Temp: " + String(temp) + "C");
}

void pumpOff(int moisture) {
  digitalWrite(PUMP_PIN, LOW);
  pumpRunning = false;
  Serial.println("Pump OFF | Moisture restored: " + String(moisture) + "%");
}

void setup() {
  Serial.begin(115200);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  dht.begin();

  Serial.println("Smart Irrigation System Started");
}

void loop() {
  delay(2000);

  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int moisturePercent = map(soilMoisture, 4095, 0, 0, 100);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  Serial.println("---");
  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("C | Humidity: ");
  Serial.print(hum);
  Serial.println("%");

  // auto pump control
  if (moisturePercent < dryThreshold && !pumpRunning) {
    Serial.println("Soil too dry!");
    pumpOn(moisturePercent, temp);
  } else if (moisturePercent > wetThreshold && pumpRunning) {
    Serial.println("Moisture restored.");
    pumpOff(moisturePercent);
  } else {
    Serial.println(pumpRunning ? "Irrigating..." : "Moisture OK");
  }
}
