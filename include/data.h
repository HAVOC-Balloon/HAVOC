#pragma once
#include <string>
#include <Adafruit_BNO055.h>

enum FlightState {
    STANDBY = 0,
    PRESTABILIZATION = 1,
    STABILIZATION = 2,
    BALLOON_DEMISE = 3,
    CONFIRMED_BALLOON_DEMISE = 4,
    PRELANDED = 5,
    LANDED = 6
};

struct Vector {
    // Unitless here. Units defined elsewhere.
    double x;
    double y;
    double z;
};

struct Position {
    double lat;  // DD.dddd
    double lon;  // DD.dddd
    double alt;  // m MSL
};

struct UTCTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    // Add function for string representation later if we care.
};

struct GPS {
    Position pos;
    UTCTime time;
    int SIV;
};

struct Atmospheric {
    float pressure;     // Pa?
    float temperature;  // deg C?
    float alt;          // m MSL
    float humidity;     // percent
};

enum TargetingMode { NO_TARGET = 0, ORIENTATION = 1, VELOCITY = 2 };

struct Target {
    TargetingMode mode = TargetingMode::ORIENTATION;
    float target = 0;  // deg if ORIENTATION; deg/s if VELOCITY
};

enum Solenoids { SOLENOIDS_OFF = 0, CLOCKWISE = 1, COUNTERCLOCKWISE = 2 };

struct Data {
    unsigned long packetCount = 0;
    unsigned long missionTime = 0;  // ms
    FlightState state = FlightState::STANDBY;
    Vector acceleration; // m/s
    Vector gyro; // deg/s
    Vector orientation; // deg
    GPS gps;
    Atmospheric atmo;
    Target target;
    Solenoids solenoids = SOLENOIDS_OFF;
};