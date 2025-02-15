#include "data.cpp"

struct Sensors {
    Sensor* imu;
    Sensor* gps;
    Sensor* barometer;
} sensors;

class Sensor {
public:
    Sensor() {
        init();
    }
    virtual void init() = 0;
    virtual void collectData(Data data) = 0;
};

class IMU: public Sensor {
public:
    void collectData(Data data) {
        data.acceleration = getAcceleration();
        data.gyro = getGyro();
        data.orientation = getOrientation();
    }
    virtual Vector getAcceleration() = 0;
    virtual Vector getGyro() = 0;
    virtual Vector getOrientation() = 0;
};

class GPSReciever: public Sensor {
    void collectData(Data data) {
        data.gps.pos = getPosition();
        data.gps.time = getUTCTime();
        data.gps.SIV = getSIV();
    }
    virtual Position getPosition() = 0;
    virtual UTCTime getUTCTime() = 0;
    virtual int getSIV() = 0;
};

class Barometer: public Sensor {
    void collectData(Data data) {
        data.atmo.pressure = getPressure();
        data.atmo.temperature = getTemperature();
        data.atmo.alt = getAltitude();
    }
    virtual float getPressure() = 0;
    virtual float getTemperature() = 0;
    virtual float getAltitude() = 0;
};

class BNO055: public IMU {
public:
    void init() {
        // TODO
    }
    Vector getAcceleration() {
        // TODO
    }
    Vector getGyro() {
        // TODO
    }
    Vector getOrientation() {
        // TODO
    }
};

class M9N: public GPSReciever {
public:
    void init() {
        // TODO
    }
    Position getPosition() {
        // TODO
    }
    UTCTime getUTCTime() {
        // TODO
    }
    int getSIV() {
        // TODO
    }
};

class BMP388: public Barometer {
public:
    void init() {
        // TODO
    }
    float getPressure() {
        // TODO
    }
    float getTemperature() {
        // TODO
    }
    float getAltitude() {
        // TODO
    }
};