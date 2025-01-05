// See readme for wiring configuration

#include "Adafruit_Si7021.h"
#include "MQTT.h"
#include "Secrets.h"

static Adafruit_Si7021 si7021 = Adafruit_Si7021();

// MQTT
const char mqttDomain[] = "192.168.4.100";
const uint16_t mqttPort = 1883;
char mqttUser[] = MQTT_USER;
char mqttPass[] = MQTT_PASS;
MQTT mqttClient(mqttDomain, mqttPort, callback);
// Do nothing when a message is received
void callback(char* topic, byte* payload, unsigned int length) {}
long mqttLastReconnectionAttemptTime;
const int mqttReconnectionInterval = 60; // in seconds

// Record last-read and update intervals
long dataReadTime;
const int dataInterval = 5; // in seconds

// Data variables
double temperature;
double humidity;
bool mqttConnected;

void setup() {
  Serial.begin(9600);

  // Declare particle variables
  Particle.variable("temperature", temperature);
  Particle.variable("humidity", humidity);
  Particle.variable("mqttConnected", mqttConnected);

  // Setup SI7021
  si7021.begin();
  temperature = si7021.readTemperature();
  humidity = si7021.readHumidity();

  // Set time intervals
  dataReadTime = Time.now();

  mqttClient.connect(System.deviceID(), mqttUser, mqttPass);
  mqttLastReconnectionAttemptTime = Time.now();
}

void loop() {
  mqttConnected = mqttClient.isConnected();
  if (mqttConnected) {
    mqttClient.loop();
  } else if (Time.now() - mqttLastReconnectionAttemptTime > mqttReconnectionInterval) {
    // If MQTT is not connected, the system should continue to function, retrying connection in the background
    mqttClient.connect(System.deviceID(), mqttUser, mqttPass);
    mqttLastReconnectionAttemptTime = Time.now();
  }

  // Only read data on correct intervals
  if(Time.now() - dataReadTime > dataInterval) {

    // Read SI7021
    temperature = si7021.readTemperature();
    humidity = si7021.readHumidity();
    dataReadTime = Time.now();

    // Publish to MQTT
    if (mqttClient.isConnected()) {
      String mqttDevicePath = "particle/" + System.deviceID() + "/";
      mqttClient.publish(mqttDevicePath + "temperature", String::format("%f", temperature));
      mqttClient.publish(mqttDevicePath + "humidity", String::format("%f", humidity));
    }
  }
}