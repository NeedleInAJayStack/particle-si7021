// See readme for wiring configuration

#include "Adafruit_Si7021.h"

static Adafruit_Si7021 si7021 = Adafruit_Si7021();

// Record last-read and update intervals
long dataReadTime;
const int dataInterval = 1; // in seconds

// Data variables
double temperature;
double humidity;



void setup() {
  Serial.begin(9600);

  // Declare particle variables
  Particle.variable("temperature", temperature);
  Particle.variable("humidity", humidity);

  // Setup SI7021
  si7021.begin();
  temperature = si7021.readTemperature();
  humidity = si7021.readHumidity();

  // Set time intervals
  dataReadTime = Time.now();
}

void loop() {
  // Only read data on correct intervals
  if(Time.now() - dataReadTime > dataInterval) {

    // Read SI7021
    temperature = si7021.readTemperature();
    humidity = si7021.readHumidity();
    
    dataReadTime = Time.now();
  }
}