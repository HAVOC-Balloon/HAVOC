#pragma once

struct Pins {
    const int clockwise = 15;
    const int counterclockwise = 14;
    const int led1 = 29;
    const int led2 = 33;
    const int sdCSPin = 5; // EXAMPLE. Change when value is known.
};

struct Blink {
    const int cycleTime = 1000; // ms
    const int duration = 50; // ms
};

struct BaudRates {
    const int serial1 = 115200;
};

struct Config {
    const Pins pins;
    const BaudRates bauds;
    const Blink blink;
    // TO BE DISCUSSED: DELETE SAMPLE RATE
    // BECAUSE NOT LOGGING EVERY LOOP IS SILLY IF YOU CAN
    // const int sampleRate = 50; //  ms. This isn't a rate. This is a time. Rate is 20Hz.
    const float seaLevelPressure = 1013.25; // hPa
    const double targetAltitude = 20000; // m
    const double deactivateAltitude = 1000; // m
    const char * telemetryFilePrefix = "telemetry/HAVOC-Data-";
    // No idea what this is but we can bring it back if we ever use it
    // const double GPSTick = 1000; // ms?
};