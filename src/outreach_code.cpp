#include <Arduino.h>
#include "havoc.h"
#include "procedures/stabilization.h"

void setup() {
    initPins();
    logger.init();
    sensors.imu.init();
    happyHavocLightDance();
}

void loop() {
    blinkLEDs();
    sensors.imu.collectData(data);
    data.target = targetPresets.north->getTarget(data);
    Solenoids requestedSolenoidState = BangBang().getStabilization(data);
    setSolenoids(requestedSolenoidState);
}