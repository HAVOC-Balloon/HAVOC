#include "board-io/sensors.h"

#include "data.h"

void IMU::collectData(Data &data) {
    if (dataReady.complete() && prefetchData()) {
        dataReady.reset();
    }
    data.acceleration = getAcceleration().value_or(data.acceleration);
    data.gyro = getGyro().value_or(data.gyro);
    data.orientation = getOrientation().value_or(data.orientation);
}

void GPSReceiver::collectData(Data &data) {
    if (dataReady.complete() && prefetchData()) {
        dataReady.reset();
    }
    data.gps.pos = getPosition().value_or(data.gps.pos);
    data.gps.time = getUTCTime().value_or(data.gps.time);
    data.gps.SIV = getSIV().value_or(data.gps.SIV);
}

void Barometer::collectData(Data &data) {
    if (dataReady.complete() && prefetchData()) {
        dataReady.reset();
    }
    data.atmo.pressure = getPressure().value_or(data.atmo.pressure);
    data.atmo.temperature = getTemperature().value_or(data.atmo.temperature);
    data.atmo.alt = getAltitude().value_or(data.atmo.alt);
    data.atmo.humidity = getHumidity().value_or(data.atmo.humidity);
}