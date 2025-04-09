#pragma once
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "data.h"
#include "utilities/time.h"
#include <optional>

class Sensor {    
protected:
    Timer dataReady = Timer(0);
public:
    virtual void init() = 0;
    virtual bool prefetchData() = 0;
    virtual void collectData(Data &data) = 0;
};

class IMU : public Sensor {
public:
    void collectData(Data &data);
    virtual std::optional<Vector> getAcceleration() = 0;
    virtual std::optional<Vector> getGyro() = 0;
    virtual std::optional<Vector> getOrientation() = 0;
};

class GPSReceiver : public Sensor {
public:
    void collectData(Data &data);
    virtual std::optional<Position> getPosition() = 0;
    virtual std::optional<UTCTime> getUTCTime() = 0;
    virtual std::optional<int> getSIV() = 0;
};

class Barometer : public Sensor {
public:
    void collectData(Data &data);
    virtual std::optional<float> getPressure() = 0;
    virtual std::optional<float> getTemperature() = 0;
    virtual std::optional<float> getAltitude() = 0;
    virtual std::optional<float> getHumidity() = 0;
};

class BNO055 : public IMU {
private:
    std::optional<Vector> acceleration = std::nullopt;
    std::optional<Vector> gyro = std::nullopt;
    std::optional<Vector> orientation = std::nullopt;
public:
    void init();
    bool prefetchData();
    std::optional<Vector> getAcceleration();
    std::optional<Vector> getGyro();
    std::optional<Vector> getOrientation();
};

class M9N : public GPSReceiver {
private:
    SFE_UBLOX_GNSS m9n;
    std::optional<Position> pos = std::nullopt;
    std::optional<UTCTime> time = std::nullopt;
    std::optional<int> SIV = std::nullopt;
public:
    void init();
    bool prefetchData();
    std::optional<Position> getPosition();
    std::optional<UTCTime> getUTCTime();
    std::optional<int> getSIV();
};

class BME280: public Barometer {
private:
    std::optional<float> pressure = std::nullopt;
    std::optional<float> temperature = std::nullopt;
    std::optional<float> humidity = std::nullopt;
    std::optional<float> altitude = std::nullopt;
public:
    void init();
    bool prefetchData();
    std::optional<float> getPressure();
    std::optional<float> getTemperature();
    std::optional<float> getAltitude();
    std::optional<float> getHumidity();
};