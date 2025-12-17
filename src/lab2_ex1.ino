#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Configuration
const char* SSID = "Wokwi-GUEST";
const char* Password = "";

// ThingSpeak Read API
const char* readURL =
  "http://api.thingspeak.com/channels/3109942/feeds.json?results=1";

// LED Pin Assignments
#define RED_LED     26
#define GREEN_LED   27
#define BLUE_LED    14
#define YELLOW_LED  12

void setup() {
  Serial.begin(115200);

  Serial.println("Jaloliddin, Lab 2");

  // Set LED pins as outputs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  // Turn all LEDs OFF initially
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, Password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  // If Wi-Fi disconnected, try to reconnect
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting ... ");
    WiFi.begin(SSID, Password);
    delay(1000);
    return;
  }

  HTTPClient http;
  http.begin(readURL);

  int httpCode = http.GET();

  if (httpCode != 200) {
    Serial.print("HTTP error: ");
    Serial.println(httpCode);
    http.end();
    delay(5000);
    return;
  }

  String payload = http.getString();
  Serial.println("---- Raw JSON Response ----");
  Serial.println(payload);

  // Parse JSON
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("JSON Parsing Error: ");
    Serial.println(error.c_str());
    http.end();
    delay(5000);
    return;
  }

  // Extract values
  String field1 = doc["feeds"][0]["field1"];
  String field2 = doc["feeds"][0]["field2"];
  String field3 = doc["feeds"][0]["field3"];
  String field4 = doc["feeds"][0]["field4"];
  String field5 = doc["feeds"][0]["field5"];

  if (field1 == "HIGH")
    digitalWrite(RED_LED, HIGH);
  else
    digitalWrite(RED_LED, LOW);

  if (field2 == "HIGH")
    digitalWrite(GREEN_LED, HIGH);
  else
    digitalWrite(GREEN_LED, LOW);

  if (field3 == "HIGH")
    digitalWrite(BLUE_LED, HIGH);
  else
    digitalWrite(BLUE_LED, LOW);

  if (field4 == "HIGH")
    digitalWrite(YELLOW_LED, HIGH);
  else
    digitalWrite(YELLOW_LED, LOW);

  Serial.println("User : " + field5);

  http.end();        // Close connection
  delay(5000);       // Fetch every 5 seconds
}
