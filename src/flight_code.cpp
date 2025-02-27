#include <Arduino.h>

#include "havoc.h"

void setup() {
  initPins();
  logger->init();
  sensors.imu = new BNO055();
  sensors.gps = new M9N();
  sensors.barometer = new BMP388();
}

void loop() {
  blinkLEDs();
  sensors.imu->collectData(&data);
  sensors.gps->collectData(&data);
  sensors.barometer->collectData(&data);
  updateFlightState();
  stateActions();
  setSolenoids();
  logger->writeTelemetry(data);
}