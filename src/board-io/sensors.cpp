#include "data.h"
#include "board-io/sensors.h"

Sensor::Sensor() {
    init();
}

void IMU::collectData(Data data) {
    data.acceleration = getAcceleration();
    data.gyro = getGyro();
    data.orientation = getOrientation();
}

void GPSReciever::collectData(Data data) {
    data.gps.pos = getPosition();
    data.gps.time = getUTCTime();
    data.gps.SIV = getSIV();
}

void Barometer::collectData(Data data) {
    data.atmo.pressure = getPressure();
    data.atmo.temperature = getTemperature();
    data.atmo.alt = getAltitude();
}