enum FlightState {
    STANDBY = 0,
    STABILIZATION = 1,
    LANDED = 2
    // There were additional flight states of PAN and
    // STABILIZATION_2 in the previous code. Those are stupid
    // and have been removed.
}

struct Vector {
    // Unitless here. Units defined elsewhere.
    double x;
    double y;
    double z;
}

struct Position {
    double lat; // DD.dddd
    double lon; // DD.dddd
    long alt; // mm MSL
}

struct UTCTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    // Add function for string representation later if we care.
}

struct GPS {
    // Nikki says we should use a timer class instead of the next two.
    // unsigned long lastTimeUpdate = 0;
    // unsigned long lastTime = 0;
    Position pos;
    UTCTime time;
    int SIV;
}

enum Solenoids {
    OFF = 0,
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = 2
}

struct Atmospheric {
    float pressure; // Pa?
    float temperature; // deg C?
}

struct Data {
    // Nikki says we shouldn't use this.
    // Drew says it should be called "previous time" if we do.
    // long lastTime = 0;
    unsigned long packetCount = 0;
    unsigned long missionTime = 0; // ms
    FlightState state = STANDBY;
    Vector acceleration; // m/s
    Vector gyro; // deg/s
    Vector orientation; // deg
    GPS gps;
    Solenoids solenoids;
    Atmospheric atmo;

    // If additional subsets of the code, outside of the primary loop,
    // have telemetry, they should format it themselves and append
    // it to this variable.
    // Appended telemetry should be CSV and should END with a comma.
    String extraTelemetry = "";
}