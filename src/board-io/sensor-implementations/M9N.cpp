#include <data.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> 
#include "board-io/sensors.h"
#include <Arduino.h>

void M9N::init() {

    // Optional debug statements are commented out

    // Initialization
    while (!m9n.begin()) {
        // Serial.println("GPS not found. Retrying momentarily.");
        delay(1000); 
    }

    // Updating settings
    m9n.setI2COutput(COM_TYPE_UBX);
    m9n.setNavigationFrequency(5);
    if (m9n.setDynamicModel(DYN_MODEL_AIRBORNE4g) == false){
        // Serial1.println(F("*** Warning: setDynamicModel failed ***"));
    }
    m9n.saveConfiguration();

    // Serial.println("GPS Online.");

    // Serial.println("Waiting for GPS lock");
    while(m9n.getSIV() < 3) {
        delay(1000);
    }

}

Position M9N::getPosition() {
    // TODO
    // Make sure to convert altitude from mm to m
}

UTCTime M9N::getUTCTime() {

    // Jadens code lol
    /* Year = (int)myGNSS.getYear();
    Month = (int)myGNSS.getMonth();
    Day = (int)myGNSS.getDay();
    date = String(Month) + "/" + String(Day) + "/" + String(Year);
    //date = " Hello";
    */
    // TODO
}

int M9N::getSIV() {
    // TODO
}