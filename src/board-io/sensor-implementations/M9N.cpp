#include <Arduino.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <data.h>
#include <optional>
#include "utilities/time.h"
#include "board-io/sensors.h"
#include "havoc.h"

/**
 * NOTE:
 *      Here, I have it set up to where each getter will only update when
 *      it has passed the tick time, but only the overall `collectData()`
 *      function will update the tickTime. Keep that as a note: the specific
 *      getters for each value are meant to be used by the `collectData()`
 *      function, not the end user directly.
 *
 */

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

    // Serial.println("GPS Online.");

    // Serial.println("Waiting for GPS lock");
    int siv;
    while ((siv = m9n.getSIV()) < 3 && true) {
        if (siv == 0) {
            errorLED.timedColor(colorPresets.red, 250);
            errorLED.timedColor(colorPresets.green, 250);
        } else if (siv == 1) {
            errorLED.timedColor(colorPresets.red, 125);
            errorLED.timedColor(colorPresets.green, 125);
        } else {
            errorLED.timedColor(colorPresets.red, 63);
            errorLED.timedColor(colorPresets.green, 63);
        }
    }
    
    

    dataReady.setDuration(1000);
}

bool M9N::prefetchData() {
    if (m9n.checkUblox()) {
        errorLED.temporaryColor(colorPresets.blue);
        pos = {
            m9n.getAltitude() / 1000.0,
            ((double)m9n.getLongitude()) * pow(10, -7),
            ((double)m9n.getLatitude()) * pow(10, -7)
        };
        time = {
            (int)m9n.getYear(),
            (int)m9n.getMonth(),
            (int)m9n.getDay(),
            (int)m9n.getHour(),
            (int)m9n.getMinute(),
            (int)m9n.getSecond(),
        };
        SIV = m9n.getSIV();
        errorLED.clearTemporaryColor();
        return true;
    }
    return false;    
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
