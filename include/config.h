#pragma once
#include <board-io/sensors.h>

struct Pins {
    const int clockwise = 15;
    const int counterclockwise = 14;
    const int sideLed = 16;
    const int redLed = 23;
    const int greenLed = 3;
    const int blueLed = 22;
    const int sdCSPin = 10;
};

struct Blink {
    const int cycleTime = 1000; // ms
    const int duration = 50; // ms
};

struct BaudRates {
    const int serial1 = 115200;
};

struct WaitTimes {
    const unsigned long stabilization = 5000;
    const unsigned long landed = 10000;
};

struct Config {
    const Pins pins;
    const BaudRates bauds;
    const Blink blink;
    const WaitTimes waitTimes;
    // TO BE DISCUSSED: DELETE SAMPLE RATE
    // BECAUSE NOT LOGGING EVERY LOOP IS SILLY IF YOU CAN
    // const int sampleRate = 50; //  ms. This isn't a rate. This is a time. Rate is 20Hz.
    const float seaLevelPressure = 1013.25; // hPa
    const double targetAltitude = 20000; // m
    const double deactivateAltitude = 2000; // m
    const char * telemetryFilePrefix = "telemetry/HAVOC-Data-";
    const char * secondaryTelemetryFilePrefix = "extra/HAVOC-Extra-";
    const char * errorMessageFilePrefix = "errors/HAVOC-Errors-";
    const unsigned long loopCycleTime = 50; // ms
    const unsigned long solenoidCycleTime = 50; // ms
    const bool waitForGPSLock = true;
};

struct Sensors {
    BNO055 imu = BNO055();
    M9N gps = M9N();
    BME280 barometer = BME280();     
};