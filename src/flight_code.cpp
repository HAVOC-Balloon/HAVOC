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
  for(int i = 0; i < 2; i++){
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
  sensors.gps.collectData(data);
  sensors.barometer.collectData(data);
  updateFlightState();
  stateActions();

  // ✨ Pretty pretty LED ✨
  /**
   * 
   * Blue                 - Standby (either one)  "Not Red" - Nikki
   * Cyan (Blue & Green)  - Pre-Stabilization 
   * Green                - Stabilization 
   * Red                  - Confirmed Balloon Demise
   * Magenta (Red & Blue) - Landed
   * 
   */
  switch (data.state) {

    case STANDBY: 
      errorLED.setColor({
        0,       // RED
        0,       // GREEN
        255      // BLUE
      });   
      break; 
    case LOW_STABILIZATION: 
      errorLED.setColor({
        0,       // RED
        255,     // GREEN
        255      // BLUE
      });   
      break; 
    case STANDBY_TWO_ELECTRIC_BOOGALOO: 
      errorLED.setColor({
        0,       // RED
        0,       // GREEN
        255      // BLUE
      });   
      break;
    case STABILIZATION: 
      errorLED.setColor({
        0,       // RED
        255,     // GREEN
        0        // BLUE
      });   
      break;
    case CONFIRMED_BALLOON_DEMISE: 
      errorLED.setColor({
        255,       // RED
        0,         // GREEN
        0          // BLUE
      });   
      break; 
    case LANDED: 
      errorLED.setColor({
        255,       // RED
        0,         // GREEN
        255        // BLUE
      });   
      break; 

  }

  logger.writeTelemetry(data);
  while (!loopTimer.isComplete()) {}
  loopTimer.reset();
}
