#include <Arduino.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <data.h>
#include <optional>
#include "utilities/time.h"
#include "board-io/sensors.h"
#include "havoc.h"

 Position mostRecentPos;
 UTCTime mostRecentTime;
 int mostRecentSIV;

 void pvtCallback(UBX_NAV_PVT_data_t *ubxDataStruct) {
    mostRecentPos = {
        ubxDataStruct->height / 1000.0,
        ((double)ubxDataStruct->lat) * pow(10, -7),
        ((double)ubxDataStruct->lon) * pow(10, -7)
    };   
    mostRecentTime = {
        (int)ubxDataStruct->year,
        (int)ubxDataStruct->month,
        (int)ubxDataStruct->day,
        (int)ubxDataStruct->hour,
        (int)ubxDataStruct->min,
        (int)ubxDataStruct->sec,
    };
    mostRecentSIV = ubxDataStruct->numSV;
}

void M9N::init() {
    // Optional debug statements are commented out

    // Initialization
    while (!m9n.begin()) {
        // Serial.println("GPS not found. Retrying momentarily.");
        errorLED.timedColor(colorPresets.red, 250);
        errorLED.timedColor(colorPresets.blue, 250);
    }
    // Updating settings
    m9n.setI2COutput(COM_TYPE_UBX);
    m9n.setNavigationFrequency(5);
    if (!m9n.setDynamicModel(DYN_MODEL_AIRBORNE4g)) {
        // Serial1.println(F("*** Warning: setDynamicModel failed ***"));
        errorLED.timedColor(colorPresets.cyan, 250);
        errorLED.timedColor(colorPresets.red, 250);
    }
    m9n.saveConfiguration();
    m9n.setAutoPVTcallbackPtr(&pvtCallback);
    // Serial.println("GPS Online.");

    // Serial.println("Waiting for GPS lock");
    bool sivCheck = true; //true/false for deactivating GPS wait
    while (sivCheck) {
        m9n.checkUblox();
        m9n.checkCallbacks();
        switch (mostRecentSIV) {
            case 0:
                errorLED.timedColor(colorPresets.red, 250);
                errorLED.timedColor(colorPresets.green, 250);
                break;
            case 1:
                errorLED.timedColor(colorPresets.red, 250);
                errorLED.timedColor(colorPresets.green, 250);
                break;
            case 2:
                errorLED.timedColor(colorPresets.red, 250);
                errorLED.timedColor(colorPresets.green, 250);
                break;
            default:
                sivCheck = false;
                break;
        }
    }

    dataReady.setDuration(0);
}

bool M9N::prefetchData() {
    m9n.checkUblox();
    m9n.checkCallbacks();
    pos = mostRecentPos;
    time = mostRecentTime;
    SIV = mostRecentSIV;
    return true;
}

std::optional<Position> M9N::getPosition() {
    return pos;
}

std::optional<UTCTime> M9N::getUTCTime() {
    return time;
}

std::optional<int> M9N::getSIV() {
    return SIV;
}
