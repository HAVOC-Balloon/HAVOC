#include <Arduino.h>

#include "havoc.h"

void setup() {
    initPins();
    setSolenoids(SOLENOIDS_OFF);
    logger->init();
    sensors.imu.init();
    sensors.gps.init();
    sensors.barometer.init();
    happyHavocLightDance();
}

void loop() {
    data.packetCount += 1;
    data.missionTime = millis();
    blinkLEDs();
    sensors.imu.collectData(data);
    sensors.gps.collectData(data);
    sensors.barometer.collectData(data);
    updateFlightState();
    stateActions();
    logger->writeTelemetry(data);
    Serial.println(millis());
}