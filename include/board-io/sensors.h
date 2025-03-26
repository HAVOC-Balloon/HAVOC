#pragma once

#include <Adafruit_BMP3XX.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

#include "data.h"
#include "utilities/time.h"
#include <optional>

class Sensor {    
public:
    Sensor();
    virtual void init() {};
    virtual void collectData(Data &data) = 0;
};

class IMU : public Sensor {
public:
    void collectData(Data &data);
    virtual Vector getAcceleration() = 0;
    virtual Vector getGyro() = 0;
    virtual Vector getOrientation() = 0;
};

class GPSReceiver : public Sensor {
protected:
    Timer tick = Timer(1000); // Change to config.gpsrate or something later
public:
    void collectData(Data &data);
    virtual std::optional<Position> getPosition() = 0;
    virtual std::optional<UTCTime> getUTCTime() = 0;
    virtual std::optional<int> getSIV() = 0;
};

class Barometer : public Sensor {
    void collectData(Data &data);
    virtual float getPressure() = 0;
    virtual float getTemperature() = 0;
    virtual float getAltitude() = 0;
};

class BNO055 : public IMU {
public:
    void init();
    Vector getAcceleration();
    Vector getGyro();
    Vector getOrientation();
};

class M9N : public GPSReceiver {
private:
    SFE_UBLOX_GNSS m9n;
public:
    void init();
    std::optional<Position> getPosition();
    std::optional<UTCTime> getUTCTime();
    std::optional<int> getSIV();
};

class BMP388: public Barometer {
private:
    Adafruit_BMP3XX bmp;
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