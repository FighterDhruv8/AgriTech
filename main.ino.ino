#include "DHT.h"
#include <WiFi.h>
#include <ThingSpeak.h>

#define DHT11PIN 0       // Pin for DHT11 sensor
#define SOIL_MOISTURE_PIN 4  // Pin for Soil Moisture sensor
#define AO_PIN 34        // Pin for MQ2 Gas sensor (ESP32 GPIO34)

DHT dht(DHT11PIN, DHT11);

// Wi-Fi credentials
const char* ssid = ".";
const char* password = "password";

// ThingSpeak settings
unsigned long myChannelNumber = 2658117;
const char* myWriteAPIKey = "0CIP80FIMMX6A2CK";
WiFiClient client;

int soilMoistureValue = 0;
int gasValue = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize DHT11 sensor
  dht.begin();

  // Initialize Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Set ADC attenuation for MQ2 sensor
  analogSetAttenuation(ADC_11db);
  
  Serial.println("Warming up the MQ2 sensor");
  delay(20000);  // Warm-up delay for MQ2 sensor
}

void loop() {
  // Read DHT11 sensor values
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  // Read Soil Moisture sensor value
  soilMoistureValue = 4095 - analogRead(SOIL_MOISTURE_PIN);

  // Read Gas sensor value
  gasValue = 4095 - analogRead(AO_PIN);

  // Print values to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("ºC ");
  Serial.print("Humidity: ");
  Serial.println(humi);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  Serial.print("MQ2 sensor AO value: ");
  Serial.println(gasValue);

  // Upload data to ThingSpeak
  ThingSpeak.setField(1, temp);              // Field 1: Temperature
  ThingSpeak.setField(2, humi);              // Field 2: Humidity
  ThingSpeak.setField(3, soilMoistureValue); // Field 3: Soil Moisture
  ThingSpeak.setField(4, gasValue);          // Field 4: Gas sensor value

  // Write the data to ThingSpeak
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  if (x == 200) {
    Serial.println("Data successfully sent to ThingSpeak");
  } else {
    Serial.println("Failed to send data to ThingSpeak. HTTP error code: " + String(x));
  }

  // Wait 15 seconds to avoid exceeding ThingSpeak's update rate limit
  delay(15000);
}
