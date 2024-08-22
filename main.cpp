#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define SOIL_MOISTURE_PIN 34
#define PUMP_PIN 26

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  dht.begin();

  Serial.println("Smart Irrigation System Started");
}

void loop() {
  delay(2000);

  // reading soil moisture
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int moisturePercent = map(soilMoisture, 4095, 0, 0, 100);

  // reading temp and humidity
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("C | Humidity: ");
  Serial.print(hum);
  Serial.println("%");
}
