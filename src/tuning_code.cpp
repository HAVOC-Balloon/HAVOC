#include <Arduino.h>

#include "havoc.h"

void setup() {
  initPins();
  logger.init();
  sensors.barometer.init();
  sensors.imu.init();
  // Disable GPS for tuning
  //sensors.gps.init();
  setSolenoids(SOLENOIDS_OFF);
  delay(500);
  for(int i = 0; i < 4; i++){
    setSolenoids(CLOCKWISE);
    delay(50);
    setSolenoids(SOLENOIDS_OFF);
    delay(200);
    setSolenoids(COUNTERCLOCKWISE);
    delay(50);
    setSolenoids(SOLENOIDS_OFF);
    delay(200);
  }
  
  happyHavocLightDance();
}

void loop() {
  static Timer loopTimer(config.loopCycleTime);
  data.packetCount += 1;
  data.missionTime = millis();
  blinkLEDs();
  sensors.imu.collectData(data);
  //sensors.gps.collectData(data);
  sensors.barometer.collectData(data);
  // Set to stabilization for tuning
  data.gps.pos.alt = 24000;
  data.state = FlightState::STABILIZATION;
  stateActions();
  logger.writeTelemetry(data);
  while (!loopTimer.isComplete()) {}
  loopTimer.reset();
}
