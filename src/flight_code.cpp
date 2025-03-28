#include <Arduino.h>

#include "havoc.h"

void setup() {
    initPins();
    logger->init();
    sensors.barometer.init();
    sensors.imu.init();
    sensors.gps.init();
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
}