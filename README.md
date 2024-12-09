# Air Quality Sensor

This is a [particle.io](https://www.particle.io/) project that uses the [SI7021 Sensor](https://www.adafruit.com/product/3251) to read temp and humidity.

## Wiring

* Vin <-> D4
* GND <-> D2
* SCL <-> D1
* SDA <-> D0

## Development

When developing on a new computer, add a `Secrets.h` file that looks like:

```cpp
#define MQTT_USER "test"
#define MQTT_PASS "test"
```