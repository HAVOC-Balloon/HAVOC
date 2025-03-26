#include "board-io/sensors.h"

#include "data.h"

void IMU::collectData(Data &data) {
    data.acceleration = getAcceleration();
    data.gyro = getGyro();
    data.orientation = getOrientation();
}

void GPSReceiver::collectData(Data &data) {
    if (!tick.isComplete()) return;
    data.gps.pos = getPosition().value_or(data.gps.pos);
    data.gps.time = getUTCTime().value_or(data.gps.time);
    data.gps.SIV = getSIV().value_or(data.gps.SIV);
    tick.reset();
}

void Barometer::collectData(Data &data) {
    data.atmo.pressure = getPressure();
    data.atmo.temperature = getTemperature();
    data.atmo.alt = getAltitude();
    data.atmo.
}