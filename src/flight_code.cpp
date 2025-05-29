#include <Arduino.h>

#include "havoc.h"

void setup() {
  initPins();
  setSolenoids(SOLENOIDS_OFF);
  delay(500);
  setSolenoids(COUNTERCLOCKWISE);
  delay(100);
  setSolenoids(SOLENOIDS_OFF);
  delay(500);
  setSolenoids(CLOCKWISE);
  delay(100);
  setSolenoids(SOLENOIDS_OFF);
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

  errorLED.temporaryColor(colorPresets.blue);
  delay(10000);
  errorLED.clearTemporaryColor();

    //NG Pins Set
  for (int i = 0; i < 4; i++)
  { 
    digitalWrite(config.pins.SDN, HIGH);
    digitalWrite(config.pins.NGPOWER, HIGH);
    delay(5000);
    digitalWrite(config.pins.SDN, LOW);
    digitalWrite(config.pins.NGPOWER, HIGH);
    delay(5000);
    digitalWrite(config.pins.SDN, LOW);
    digitalWrite(config.pins.NGPOWER, LOW);
    delay(5000);
  }
}

void loop() {
  static Timer loopTimer(config.loopCycleTime);
  data.packetCount += 1;
  data.missionTime = millis();
  //blinkLEDs();
  //digitalWrite(config.pins.SDN, HIGH);
  //digitalWrite(config.pins.NGPOWER, HIGH); 
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