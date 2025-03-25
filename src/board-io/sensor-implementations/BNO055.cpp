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
    bool foundCalib = false;

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

imu::Vector<3> BNO055::getAcceleration() {
    bno.getEvent(&event);
    return bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
}

imu::Vector<3> BNO055::getGyro() {
    return bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
}

imu::Vector<3> BNO055::getOrientation() {
    return bno.getVector(Adafruit_BNO055::VECTOR_EULER);
}