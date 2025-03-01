#include <Arduino.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <data.h>

#include <optional>
#include "time/time.h"
#include "board-io/sensors.h"

// TODO Update each of the getters to only update when it has passed the tick
// time.

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
        delay(1000);
    }

    // Updating settings
    m9n.setI2COutput(COM_TYPE_UBX);
    m9n.setNavigationFrequency(5);
    if (m9n.setDynamicModel(DYN_MODEL_AIRBORNE4g) == false) {
        // Serial1.println(F("*** Warning: setDynamicModel failed ***"));
    }
    m9n.saveConfiguration();

    // Serial.println("GPS Online.");

    // Serial.println("Waiting for GPS lock");
    while (m9n.getSIV() < 3) {
        delay(1000);
    }

}

std::optional<Position> M9N::getPosition() {
    if (tick.isComplete()) {
        Position toReturn;
        toReturn.alt = m9n.getAltitude() / 1000.0;
        toReturn.lon = ((double)m9n.getLongitude()) * pow(10, -7);
        toReturn.lat = ((double)m9n.getLatitude()) * pow(10, -7);
        return toReturn;
    }
    return std::nullopt;
}

std::optional<UTCTime> M9N::getUTCTime() {
    if (tick.isComplete()) {
        UTCTime toReturn;
        toReturn.year = (int)m9n.getYear();
        toReturn.month = (int)m9n.getMonth();
        toReturn.day = (int)m9n.getDay();
        toReturn.hour = (int)m9n.getHour();
        toReturn.minute = (int)m9n.getMinute();
        toReturn.second = (int)m9n.getSecond();
        toReturn.hour = (int)m9n.getHour();
        return toReturn;
    }
    return std::nullopt;
}

std::optional<int> M9N::getSIV() {
    if (tick.isComplete()) {
        return m9n.getSIV();
    }
    return std::nullopt;
}
