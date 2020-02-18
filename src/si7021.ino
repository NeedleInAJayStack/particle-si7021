// See readme for wiring configuration

#include "Adafruit_Si7021.h"

static Adafruit_Si7021 si7021 = Adafruit_Si7021();

// Record last-read and update intervals
long dataReadTime;
int dataInterval = 1000;

// Data variables
double temperature;
double humidity;



void setup() {
  Serial.begin(9600);

  // Declare particle variables
  Particle.variable("temperature", temperature);
  Particle.variable("humidity", humidity);

  // Set up 'power' pins, comment out if not used!
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D2, LOW);
  digitalWrite(D4, HIGH);

  // Setup SI7021
  si7021.begin();
  temperature = si7021.readTemperature();
  humidity = si7021.readHumidity();

  // Set time intervals
  dataReadTime = millis();
}

void loop() {
  // Only read data on correct intervals
  if(millis() - dataReadTime > dataInterval) {

    // Read SI7021
    temperature = si7021.readTemperature();
    humidity = si7021.readHumidity();
    Serial.print("Temperature: "); Serial.print(temperature, 2); Serial.println("C");
    Serial.print("Humidity: "); Serial.print(humidity, 2); Serial.println("%RH");

    Serial.println("---");
    dataReadTime = millis();
  }
}