#pragma once
#include <string>
#include <Adafruit_BNO055.h>

enum FlightState {
    STANDBY = 0,
    STABILIZATION = 1,
    LANDED = 2
    // There were additional flight states of PAN and
    // STABILIZATION_2 in the previous code. Those are stupid
    // and have been removed.
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
    // Nikki says we should use a timer class instead of the next two.
    // unsigned long lastTimeUpdate = 0;
    // unsigned long lastTime = 0;
    Position pos;
    UTCTime time;
    int SIV;
};

struct Atmospheric {
    float pressure;     // Pa?
    float temperature;  // deg C?
    float alt;          // m MSL
};

enum TargetingMode { NO_TARGET = 0, ORIENTATION = 1, VELOCITY = 2 };

struct Target {
    TargetingMode mode = TargetingMode::ORIENTATION;
    float target = 0;  // deg if ORIENTATION; deg/s if VELOCITY
};

enum Solenoids { SOLENOIDS_OFF = 0, CLOCKWISE = 1, COUNTERCLOCKWISE = 2 };

struct Data {
    // Nikki says we shouldn't use this.
    // Drew says it should be called "previous time" if we do.
    // long lastTime = 0;
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
    time_t t_object = 0; 

    // If additional subsets of the code, outside of the primary loop,
    // have telemetry, they should format it themselves and append
    // it to this variable.
    // Appended telemetry should be CSV and should END with a comma.

    // extraTelemetry has been temporarily removed until we determine
    // a proper mechanism for allowing telemetry to be collected
    // from alternate sources
    // std::string extraTelemetry = "";
};