#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define SOIL_MOISTURE_PIN 34
#define PUMP_PIN 26

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* telegramUsername = "@YOUR_TELEGRAM_USERNAME";
const char* apiKey = "YOUR_CALLMEBOT_API_KEY";

DHT dht(DHTPIN, DHTTYPE);

int dryThreshold = 30;
int wetThreshold = 70;
bool pumpRunning = false;
int totalPumpCycles = 0;

void reconnectWifi() {
  // retry wifi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost! Reconnecting...");
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reconnected!");
    } else {
      Serial.println("Reconnection failed.");
    }
  }
}

void sendAlert(String message) {
  HTTPClient http;
  String url = "https://api.callmebot.com/text.php?user=" + String(telegramUsername) + "&apikey=" + String(apiKey) + "&text=" + message;
  http.begin(url);
  http.GET();
  http.end();
}

String getTime() {
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);
  char buf[30];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
  return String(buf);
}

void pumpOn(int moisture, float temp) {
  digitalWrite(PUMP_PIN, HIGH);
  pumpRunning = true;
  totalPumpCycles++;
  Serial.println("Pump ON");
  sendAlert("Irrigation started at " + getTime() + " | Moisture: " + String(moisture) + "% | Temp: " + String(temp) + "C");
}

void pumpOff(int moisture) {
  digitalWrite(PUMP_PIN, LOW);
  pumpRunning = false;
  Serial.println("Pump OFF");
  sendAlert("Irrigation stopped at " + getTime() + " | Moisture restored: " + String(moisture) + "%");
}

void setup() {
  Serial.begin(115200);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  configTime(19800, 0, "pool.ntp.org");
  delay(2000);

  Serial.println("Smart Irrigation System Ready");
}

void loop() {
  reconnectWifi();
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
  Serial.print("Total pump cycles: ");
  Serial.println(totalPumpCycles);

  if (moisturePercent < dryThreshold && !pumpRunning) {
    Serial.println("Soil too dry! Starting irrigation...");
    pumpOn(moisturePercent, temp);
  } else if (moisturePercent > wetThreshold && pumpRunning) {
    Serial.println("Moisture restored. Stopping pump...");
    pumpOff(moisturePercent);
  } else {
    Serial.println(pumpRunning ? "Irrigating..." : "Moisture OK");
  }
}
