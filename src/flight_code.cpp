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
  delay(5000);
  errorLED.clearTemporaryColor();

    //NG Pins Set
  for (int i = 0; i < 2; i++)
  { 
    digitalWrite(config.pins.SDN, LOW); //Actual HIGH
    digitalWrite(config.pins.NGPOWER, LOW); //Actual HIGH
    delay(10000);

    errorLED.temporaryColor(colorPresets.red);
    delay(500);
    errorLED.clearTemporaryColor();

    digitalWrite(config.pins.SDN, LOW); //Actual HIGH
    digitalWrite(config.pins.NGPOWER, HIGH); //Actual LOW
    delay(10000);

    errorLED.temporaryColor(colorPresets.red);
    delay(500);
    errorLED.clearTemporaryColor();

    digitalWrite(config.pins.SDN, HIGH); // Actual LOW
    digitalWrite(config.pins.NGPOWER, HIGH); //Actual LOW
    delay(10000);
    
    errorLED.temporaryColor(colorPresets.red);
    delay(500);
    errorLED.clearTemporaryColor();
  }

  errorLED.temporaryColor(colorPresets.red);
  delay(2000);
  errorLED.clearTemporaryColor();
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