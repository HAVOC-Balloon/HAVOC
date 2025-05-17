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
  
  pinMode(config.pins.SDN, OUTPUT);
  pinMode(config.pins.NGPOWER, OUTPUT); 

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH); 
}

void loop() {
  //static Timer loopTimer(config.loopCycleTime);
  //data.packetCount += 1;
  //data.missionTime = millis();
  //blinkLEDs();
  digitalWrite(config.pins.NGPOWER, HIGH);
  digitalWrite(config.pins.SDN, HIGH);
  digitalWrite(13, HIGH); 
  //sensors.imu.collectData(data);
  //sensors.gps.collectData(data);
  //sensors.barometer.collectData(data);
  //updateFlightState();
  //stateActions();
  //logger.writeTelemetry(data);
  //while (!loopTimer.isComplete()) {
  //}
  //loopTimer.reset();
}