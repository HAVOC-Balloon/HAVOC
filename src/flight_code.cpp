#include <Arduino.h>
#include "havoc.h"

void setup() {
    initPins();
    logger.init();
    sensors.barometer.init();
    sensors.imu.init();
    setSolenoids(SOLENOIDS_OFF);
    delay(500);
    //sensors.gps.init();
    setSolenoids(COUNTERCLOCKWISE);
    //digitalWrite(14, HIGH);
    delay(250);
    setSolenoids(SOLENOIDS_OFF);
    delay(500);
    //digitalWrite(14, LOW);
    setSolenoids(CLOCKWISE);
    //digitalWrite(15, HIGH);
    delay(250);
    //digitalWrite(15, LOW);

    setSolenoids(SOLENOIDS_OFF);
    delay(500);
    for(int i = 0; i < 3; i++){
        digitalWrite(14, HIGH);
        delay(config.solenoidCycleTime);
        digitalWrite(14, LOW);
        digitalWrite(15, LOW);
        delay(500);
        digitalWrite(15, HIGH);
        delay(config.solenoidCycleTime);
        digitalWrite(14, LOW);
        digitalWrite(15, LOW);
        delay(500);
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
    updateFlightState();
    stateActions();
    logger.writeTelemetry(data);
    while (!loopTimer.isComplete()) {}
    loopTimer.reset();
}