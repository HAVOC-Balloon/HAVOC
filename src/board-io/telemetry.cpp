#include <Arduino.h>
#include <data.h>
#include <board-io/telemetry.h>

void OpenLog::init() {
    Serial1.begin(115200);
}

void OpenLog::writeTelemetry(Data data) {
    Serial1.print(data.missionTime);
    Serial1.print(",");
    // etc.
    // TODO: Finish writeTelemetry function
}

void SPILogger::init() {
    // TODO: INIT CODE FOR SPI LOGGER
}

void SPILogger::writeTelemetry(Data data) {
    // TODO: TELEMETRY CODE FOR SPI LOGGER
}
