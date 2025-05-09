#include <Arduino.h>

#include "havoc.h"

void setup() {
  initPins();
  logger.init();
  sensors.barometer.init();
  sensors.imu.init();
  sensors.gps.init();

  setSolenoids(SOLENOIDS_OFF);
  delay(500);
  setSolenoids(CLOCKWISE);
  delay(50);
  setSolenoids(COUNTERCLOCKWISE);
  delay(50);
  setSolenoids(SOLENOIDS_OFF);
  
  happyHavocLightDance();
}

void loop() {
  static Timer loopTimer(config.loopCycleTime);
  data.packetCount += 1;
  data.missionTime = millis();
  blinkLEDs();
  sensors.imu.collectData(data);
  sensors.gps.collectData(data);
  sensors.barometer.collectData(data);
  updateFlightState();
  stateActions();
  logger.writeTelemetry(data);
  while (!loopTimer.isComplete()) {
  }
  loopTimer.reset();
}