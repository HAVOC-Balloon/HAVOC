#include "data.h"
#include "board-io/sensors.h"
#include <EEPROM.h>


Adafruit_BNO055 bno = Adafruit_BNO055(55);

sensors_event_t event;

void BNO055::init() {
    if(!bno.begin())
    {
      Serial.println(F("Oops, no BNO055 detected ... Check your wiring or I2C ADDR!"));
      // TODO - Error Code 
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
        Serial1.println("\nNo Calibration Data for this sensor exists in EEPROM");
        delay(500);
    }
    else
    {
        Serial1.println("\nFound Calibration for this sensor in EEPROM.");
        eeAddress += sizeof(long);
        EEPROM.get(eeAddress, calibrationData);

            //displaySensorOffsets(calibrationData);

        Serial.println("\n\nRestoring Calibration data to the BNO055...");
        bno.setSensorOffsets(calibrationData);

        Serial1.println("\n\nCalibration data loaded into BNO055");
        //foundCalib = true;
  }
}

Vector BNO055::getAcceleration() {
    bno.getEvent(&event);
    imu::Vector<3> acceleration = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    return {acceleration.x(), acceleration.y(), acceleration.z()};
}

Vector BNO055::getGyro() {
    imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    return {gyro.x(), gyro.y(), gyro.z()};
}

Vector BNO055::getOrientation() {
    imu::Vector<3> orientation = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    return {orientation.x(), orientation.y(), orientation.z()};
}