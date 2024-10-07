#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Replace with your network credentials
const char* ssid = "MUFLIH";
const char* password = "12345678";

// Replace with your Telegram token and chat ID
#define BOT_TOKEN "6101599871:AAGfU4Dz6RrbORbnptzVHzbgbWlIvwGv7Wc"
#define CHAT_ID "815866835"

// Replace with your DHT22 pin
#define DHTPIN 15

// Replace with your water pump pin
#define PUMP_PIN 4

// Initialize DHT sensor
DHT dht(DHTPIN, DHT22);

// Initialize Telegram bot
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize DHT sensor
  dht.begin();

  // Initialize water pump
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  // Connect to Telegram
  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Connection to Telegram failed");
  } else {
    Serial.println("Connected to Telegram");
  }
}

void loop() {
   // read humidity
  float humi  = dht.readHumidity();
  // read temperature in Celsius
  float tempC = dht.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht.readTemperature(true);

  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C");
    Serial.print(tempF);
    Serial.println("°F");
  }

  // wait a 2 seconds between readings
  delay(2000);


  // Check if temperature is above 50 degrees Celsius
  if (temperature > 50) {
    // Turn on water pump
    digitalWrite(PUMP_PIN, HIGH);

    // Send message to Telegram
    String message = "Fire detected! Water pump turned on.";
    bot.sendMessage(CHAT_ID, message);

    // Wait for 10 seconds
    delay(10000);

    // Turn off water pump
    digitalWrite(PUMP_PIN, LOW);
  

    // Wait for 1 second
   delay(1000);
  }
}