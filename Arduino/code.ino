#include <WiFiS3.h>
#include "secrets.h"
#include <DHT.h>
#include "ThingSpeak.h"

// Pin configuration and timing intervals
#define DHT11_PIN 2                // Arduino pin connected to DHT11 sensor
#define READ_INTERVAL 30000         // Interval (in ms) between sensor readings
#define THINGSPEAK_INTERVAL 30000  // Interval (in ms) between ThingSpeak updates
#define LED_PIN 13

// WiFi and ThingSpeak client setup
WiFiClient client;
DHT dht11(DHT11_PIN, DHT11);  // Initialize DHT sensor on specified pin

// WiFi and ThingSpeak credentials (stored securely in "secrets.h")
char ssid[] = SECRET_SSID;                        // WiFi network SSID
char pass[] = SECRET_PASS;                        // WiFi network password
unsigned long myChannelNumber = SECRET_CH_ID;     // ThingSpeak channel ID
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;  // ThingSpeak API key
const char* myReadAPIKey = SECRET_READ_APIKEY;
int status = WL_IDLE_STATUS;                      // WiFi connection status

// Variables to keep track of the last operation times
unsigned long lastReadTime = 0;
unsigned long lastUploadTime = 0;

// Function to print WiFi network information (SSID, IP address, etc.)
void printNetworkInfo() {
  Serial.print("WiFi Status: ");
  Serial.println(WiFi.status());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Function to establish connection to WiFi network
void connectToWiFi() {
  // Check if WiFi module is available
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true)
      ;  // Halt if WiFi module is unavailable
  }

  // Check WiFi firmware version
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Warning: Firmware is outdated.");
  }

  // Attempt to connect to the specified WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Connecting to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);  // Start WiFi connection
    delay(10000);                     // Wait 10 seconds before retrying
  }

  Serial.println("Connected to WiFi");
  printNetworkInfo();  // Print network details after connection
}

// Function to reconnect WiFi if disconnected
void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    connectToWiFi();  // Re-establish connection if lost
  }
}

// Function to write data to ThingSpeak (temperature/humidity values)
void updateThingSpeak(float humidity, float temperature) {

  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);
  int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  // Check if the update was successful
  if (response == 0) {
    Serial.println("Channel updated successfully.");
  } else {
    Serial.println("Problem updating channel. HTTP error code: " + String(response));
    delay(2000);                                                        // Small delay before retry
    response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  // Retry update
  }
}

// Function to read humidity and temperature from DHT11 sensor and send to ThingSpeak
void readSensorData() {
  // Read values from DHT11 sensor
  float humidity = dht11.readHumidity();
  float temperature = dht11.readTemperature();  // Temperature in Celcius

  //humidity = random(1,101);
  //temperature = random(-20,71);

  // Check if reading failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT11 sensor!");
  } else {
    // Display values on Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("% | Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");

    // Send readings to ThingSpeak (humidity in field 1, temperature in field 2)
    updateThingSpeak(humidity, temperature);
    delay(1000);
  }
}

void readLED() {
  int ledStatus =  ThingSpeak.readIntField(myChannelNumber,3,myReadAPIKey);  // Read field 3 value
  Serial.println(ledStatus);
  // Update LED state based on the read value
  if (ledStatus == 0) {
    digitalWrite(LED_PIN, HIGH);  // Turn LED on
    Serial.println("LED turned ON");
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn LED off
    Serial.println("LED turned OFF");
}
}

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  connectToWiFi();     // Connect to WiFi network
  pinMode(LED_BUILTIN, OUTPUT);
  delay(20000);
  ThingSpeak.begin(client);  // Initialize ThingSpeak client
  dht11.begin();             // Initialize DHT11 sensor
}

void loop() {
  reconnectWiFi();  // Ensure WiFi connection is maintained

  unsigned long currentTime = millis();  // Current time in milliseconds

  // Check if it's time to read sensor data
  if (currentTime - lastReadTime >= READ_INTERVAL) {
    readSensorData();  // Read and display sensor data
    readLED();
    lastReadTime = currentTime;  // Update last read time
  }

  // Check if it's time to update ThingSpeak
  // if (currentTime - lastUploadTime >= THINGSPEAK_INTERVAL) {
  //   lastUploadTime = currentTime;  // Update last upload time
  // }
}
