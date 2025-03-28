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
        errorLED.timedColor(colorPresets.magenta, 250);
        errorLED.timedColor(colorPresets.red, 250);
    }

    bno.setExtCrystalUse(true);

    // Below this is code for Calibration
    int eeAddress = 0;
    long bnoID;
    // bool foundCalib = false; // Unused variable warning.

    EEPROM.get(eeAddress, bnoID);

    adafruit_bno055_offsets_t calibrationData;
    sensor_t sensor;

    bno.getSensor(&sensor);
    if (bnoID != sensor.sensor_id)
    {
        // No calibration found
        errorLED.colorSweep(colorPresets.red, 250);
        errorLED.colorSweep(colorPresets.off, 250);
        errorLED.colorSweep(colorPresets.red, 250);
        errorLED.colorSweep(colorPresets.off, 250);
    }
    else
    {
        eeAddress += sizeof(long);
        EEPROM.get(eeAddress, calibrationData);

            //displaySensorOffsets(calibrationData);

        bno.setSensorOffsets(calibrationData);

        //foundCalib = true;
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