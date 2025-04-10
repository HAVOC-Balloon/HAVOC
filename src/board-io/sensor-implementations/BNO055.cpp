#include <optional>
#include "data.h"
#include "havoc.h"
#include "board-io/sensors.h"
#include <EEPROM.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);
sensors_event_t event;

void BNO055::init() {
    if(!bno.begin())
    {
        logger.writeErrorMessage("BNO055 was not found.");
        errorLED.timedColor(colorPresets.magenta, 250);
        errorLED.timedColor(colorPresets.red, 250);
    }
    // Calibration code
    int eeAddress = 0;
    long bnoID;
    EEPROM.get(eeAddress, bnoID);
    adafruit_bno055_offsets_t calibrationData;
    sensor_t sensor;
    bno.getSensor(&sensor);
    if (bnoID != sensor.sensor_id)
    {
        logger.writeErrorMessage("BNO055 calibration data was not found.");
        errorLED.colorSweep(colorPresets.red, 250);
        errorLED.colorSweep(colorPresets.off, 250);
        errorLED.colorSweep(colorPresets.red, 250);
        errorLED.colorSweep(colorPresets.off, 250);
    }
    else
    {
        eeAddress += sizeof(long);
        EEPROM.get(eeAddress, calibrationData);
        bno.setSensorOffsets(calibrationData);
    }
}

bool BNO055::prefetchData() {
    imu::Vector<3> fetchedAcceleration = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    acceleration = {fetchedAcceleration.x(), fetchedAcceleration.y(), fetchedAcceleration.z()};
    imu::Vector<3> fetchedGyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    gyro = {fetchedGyro.x(), fetchedGyro.y(), fetchedGyro.z()};
    imu::Vector<3> fetchedOrientation = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    orientation = {fetchedOrientation.x(), fetchedOrientation.y(), fetchedOrientation.z()};
    return true;
}

std::optional<Vector> BNO055::getAcceleration() {
    return acceleration;
}

std::optional<Vector> BNO055::getGyro() {
    return gyro;
}

std::optional<Vector> BNO055::getOrientation() {
    return orientation;
}