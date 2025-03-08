#pragma once
#include <Adafruit_BNO055.h>

class Sensor {
public:
    Sensor();
    virtual void init() {};
    virtual void collectData(Data data) = 0;
};

class IMU: public Sensor {
public:
    void collectData(Data data);
    virtual imu::Vector<3> getAcceleration() = 0;
    virtual imu::Vector<3> getGyro() = 0;
    virtual imu::Vector<3> getOrientation() = 0;
};

class GPSReciever: public Sensor {
    void collectData(Data data);
    virtual Position getPosition() = 0;
    virtual UTCTime getUTCTime() = 0;
    virtual int getSIV() = 0;
};

class Barometer: public Sensor {
    void collectData(Data data);
    virtual float getPressure() = 0;
    virtual float getTemperature() = 0;
    virtual float getAltitude() = 0;
};

class BNO055: public IMU {
public:
    void init();
    imu::Vector<3> getAcceleration();
    imu::Vector<3> getGyro();
    imu::Vector<3> getOrientation();
};

class M9N: public GPSReciever {
public:
    void init();
    Position getPosition();
    UTCTime getUTCTime();
    int getSIV();
};

class BMP388: public Barometer {
public:
    void init();
    float getPressure();
    float getTemperature();
    float getAltitude();
};

struct Sensors {
    Sensor* imu;
    Sensor* gps;
    Sensor* barometer;
};