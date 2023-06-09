// Connect Vin to 3-5VDC
// Connect GND to ground
// Connect SCL to I2C clock pin (D1 on Photon/Electron)
// Connect SDA to I2C data pin  (D0 on Photon/Electron)F
// Connect WAKE to ground

#if defined(PARTICLE)
 SYSTEM_THREAD(ENABLED)
#endif

#include "Adafruit_CCS811.h"

Adafruit_CCS811 ccs;

void setup() {
  Serial.begin(9600);
  
  Serial.println("CCS811 test");
  
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  // Wait for sensor to be available
  while(!ccs.available());
  // Calibrate temperature sensor
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);
}

void loop() {
  if(ccs.available()){
    float temp = ccs.calculateTemperature();
    ccs.readData();

    Serial.print("CO2: ");
    Serial.print(ccs.geteCO2());
    Serial.print("ppm, TVOC: ");
    Serial.print(ccs.getTVOC());
    Serial.print("ppb   Temp:");
    Serial.println(temp);
  }
  else{
    Serial.println("Sensor Unavailable!");
  }
  delay(1000); // Usually can't run faster than 1sec or else sensor becomes unavailable.
}