#include <Arduino.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <data.h>
#include <optional>
#include "utilities/time.h"
#include "board-io/sensors.h"
// TEMPORARY INCLUDE
#include "config.h"
Config config2;

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
        digitalWrite(config2.pins.sideLed, HIGH);
        delay(800);
        digitalWrite(config2.pins.sideLed, LOW);
        delay(200);
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
        digitalWrite(config2.pins.sideLed, HIGH);
        delay(100);
        digitalWrite(config2.pins.sideLed, LOW);
        delay(100);
    }

    dataReady.setDuration(1000);
}

void M9N::prefetchData() {
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
