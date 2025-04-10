#pragma once
#include <Arduino.h>
#include "config.h"
#include "data.h"
#include "board-io/sensors.h"
#include "board-io/telemetry.h"
#include "procedures/targeter.h"
#include "board-io/errorLED.h"

extern Config config;
extern Data data;
extern Sensors sensors;
extern TargetPresets targetPresets;
extern SPILogger logger;
extern ErrorLED errorLED;

void initPins();
void blinkLEDs();
void updateFlightState();
void stateActions();
void setSolenoids(Solenoids solenoidState);
void happyHavocLightDance();