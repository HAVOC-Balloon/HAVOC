#include <Arduino.h>
#include <data.h>
#include <havoc.h>
#include <board-io/telemetry.h>
#include <SPI.h>
#include <SdFat.h>

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
    while (!SD.begin(config.pins.sdCSPin)) {
        // Error message cannot be written to SD card before it is initialized.
        errorLED.timedColor(colorPresets.blue, 250);
        errorLED.timedColor(colorPresets.green, 250);
    }
    // Start with number 1
    unsigned short int fileNumber = 1;
    // Reserve space for filenames up to 31 chars
    char fileName[32];
    // Reserve space for converting number to string
    char fileNumberStr[6];
    do {
        // Overwrite fileName with prefix
        strcpy(fileName, config.telemetryFilePrefix);
        // Convert fileNumber to five-char string (e.g. 1 -> 00001)
        sprintf(fileNumberStr, "%0*d", 5, fileNumber);
        // Append fileNumber to prefix
        strcat(fileName, fileNumberStr);
        // Append ".csv" to file name
        strcat(fileName, ".csv");
        // Increment the filenumber for next loop
        fileNumber++;
        Serial.println(fileName);
    } while (SD.exists(fileName)); // Do that again if the file exists already
    Serial.print("Found ");
    Serial.println(fileName);
    // Once we find a file name that doesn't exist, use it!
    while (!(telemetryFile = SD.open(fileName, FILE_WRITE))) {
        // Error message cannot be written to SD card before it is initialized.
        errorLED.timedColor(colorPresets.magenta, 250);
        errorLED.timedColor(colorPresets.blue, 250);
    }
    strcpy(fileName, config.secondaryTelemetryFilePrefix);
    sprintf(fileNumberStr, "%0*d", 5, fileNumber);
    strcat(fileName, fileNumberStr);
    strcat(fileName, ".txt");
    while (!(secondaryTelemetryFile = SD.open(fileName, FILE_WRITE))) {
        // Error message cannot be written to SD card before it is initialized.
        errorLED.timedColor(colorPresets.magenta, 125);
        errorLED.timedColor(colorPresets.blue, 125);
    }
    strcpy(fileName, config.errorMessageFilePrefix);
    sprintf(fileNumberStr, "%0*d", 5, fileNumber);
    strcat(fileName, fileNumberStr);
    strcat(fileName, ".txt");
    while (!(errorMessageFile = SD.open(fileName, FILE_WRITE))) {
        // Error message cannot be written to SD card before it is initialized.
        errorLED.timedColor(colorPresets.magenta, 63);
        errorLED.timedColor(colorPresets.blue, 63);
    }
}

void SPILogger::writeTelemetry(Data &data) {
    static Timer flushTimer(2000);

    telemetryFile.print(data.packetCount);
    telemetryFile.print(",");
    telemetryFile.print(data.missionTime);
    telemetryFile.print(",");
    telemetryFile.print(data.state);
    telemetryFile.print(",");
    telemetryFile.print(data.acceleration.x);
    telemetryFile.print(",");
    telemetryFile.print(data.acceleration.y);
    telemetryFile.print(",");
    telemetryFile.print(data.acceleration.z);
    telemetryFile.print(",");
    telemetryFile.print(data.gyro.x);
    telemetryFile.print(",");
    telemetryFile.print(data.gyro.y);
    telemetryFile.print(",");
    telemetryFile.print(data.gyro.z);
    telemetryFile.print(",");
    telemetryFile.print(data.orientation.x);
    telemetryFile.print(",");
    telemetryFile.print(data.orientation.y);
    telemetryFile.print(",");
    telemetryFile.print(data.orientation.z);
    telemetryFile.print(",");
    telemetryFile.print(data.gps.pos.alt);
    telemetryFile.print(",");
    telemetryFile.print(data.gps.pos.lat, 7);
    telemetryFile.print(",");
    telemetryFile.print(data.gps.pos.lon, 7);
    telemetryFile.print(",");
    telemetryFile.print(data.gps.time.year);
    telemetryFile.print(",");
    telemetryFile.print(data.gps.time.month);
    telemetryFile.print(",");
    telemetryFile.print(data.gps.time.day);
    telemetryFile.print(",");
    telemetryFile.print(data.gps.time.hour);
    telemetryFile.print(",");
    telemetryFile.print(data.gps.time.minute);
    telemetryFile.print(",");
    telemetryFile.print(data.gps.time.second);
    telemetryFile.print(",");
    telemetryFile.print(data.gps.SIV);
    telemetryFile.print(",");
    telemetryFile.print(data.atmo.alt);
    telemetryFile.print(",");
    telemetryFile.print(data.atmo.pressure);
    telemetryFile.print(",");
    telemetryFile.print(data.atmo.temperature);
    telemetryFile.print(",");
    telemetryFile.print(data.atmo.humidity);
    telemetryFile.print(",");
    telemetryFile.print(data.target.mode);
    telemetryFile.print(",");
    telemetryFile.print(data.target.target);
    telemetryFile.print(",");
    telemetryFile.print(data.solenoids);
    telemetryFile.println();
    // Flush written data to SD card to ensure it's written and not just
    // waiting in the buffer
    // This is important because microcontrollers can't guarantee
    // that file I/O that waits too long won't be corrupted by power loss
    if (flushTimer.isComplete()) {
        telemetryFile.flush();
        flushTimer.reset();
    }
}

void SPILogger::writeSecondaryTelemetry(const char * telemetry, bool newline, bool alwaysFlush) {
    static Timer flushTimer(2000);
    secondaryTelemetryFile.print(telemetry);
    if (newline) {secondaryTelemetryFile.println();}
    if (flushTimer.isComplete() || alwaysFlush) {
        telemetryFile.flush();
        flushTimer.reset();
    }
}

void SPILogger::writeErrorMessage(const char * error) {
    errorMessageFile.print(millis());
    errorMessageFile.print(" ms: ");
    errorMessageFile.println(error);
    errorMessageFile.flush();
}