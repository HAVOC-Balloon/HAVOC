#include <Arduino.h>
#include <data.h>
#include <havoc.h>
#include <board-io/telemetry.h>
#include <SPI.h>
#include <SD.h>

void OpenLog::init() {
    Serial1.begin(config.bauds.serial1);
}

void OpenLog::writeTelemetry(Data &data) {
    // This implementation looks sort of messy or maybe slow,
    // but I can assure you after a deep dive into the implementation
    // of Serial1.print that it's no more inefficient than any other
    // implementation --- from what I can tell, the limiting factor
    // is always going to be on Arduino's side of things.
    Serial1.print(data.packetCount);
    Serial1.print(",");
    Serial1.print(data.missionTime);
    Serial1.print(",");
    Serial1.print(data.state);
    Serial1.print(",");
    Serial1.print(data.acceleration.x);
    Serial1.print(",");
    Serial1.print(data.acceleration.y);
    Serial1.print(",");
    Serial1.print(data.acceleration.z);
    Serial1.print(",");
    Serial1.print(data.gyro.x);
    Serial1.print(",");
    Serial1.print(data.gyro.y);
    Serial1.print(",");
    Serial1.print(data.gyro.z);
    Serial1.print(",");
    Serial1.print(data.orientation.x);
    Serial1.print(",");
    Serial1.print(data.orientation.y);
    Serial1.print(",");
    Serial1.print(data.orientation.z);
    Serial1.print(",");
    Serial1.print(data.gps.pos.alt);
    Serial1.print(",");
    Serial1.print(data.gps.pos.lat);
    Serial1.print(",");
    Serial1.print(data.gps.pos.lon);
    Serial1.print(",");
    Serial1.print(data.gps.time.year);
    Serial1.print(",");
    Serial1.print(data.gps.time.month);
    Serial1.print(",");
    Serial1.print(data.gps.time.day);
    Serial1.print(",");
    Serial1.print(data.gps.time.hour);
    Serial1.print(",");
    Serial1.print(data.gps.time.minute);
    Serial1.print(",");
    Serial1.print(data.gps.time.second);
    Serial1.print(",");
    Serial1.print(data.gps.SIV);
    Serial1.print(",");
    Serial1.print(data.atmo.alt);
    Serial1.print(",");
    Serial1.print(data.atmo.pressure);
    Serial1.print(",");
    Serial1.print(data.atmo.temperature);
    Serial1.print(",");
    Serial1.print(data.target.mode);
    Serial1.print(",");
    Serial1.print(data.target.target);
    Serial1.print(",");
    Serial1.print(data.solenoids);
    Serial1.println();
}

void SPILogger::init() {
    // TODO: INIT CODE FOR SPI LOGGER
}

void SPILogger::writeTelemetry(Data &data) {
    // TODO: TELEMETRY CODE FOR SPI LOGGER
}
