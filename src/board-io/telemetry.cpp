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
    while (!(currentFile = SD.open(fileName, FILE_WRITE))) {
        errorLED.timedColor(colorPresets.magenta, 250);
        errorLED.timedColor(colorPresets.blue, 250);
    }
}

void SPILogger::writeTelemetry(Data &data) {
    static Timer flushTimer(2000);
    // Flush written data to SD card to ensure it's written and not just
    // waiting in the buffer
    // This is important because microcontrollers can't guarantee
    // that file I/O that waits too long won't be corrupted by power loss
    currentFile.print(data.packetCount);
    currentFile.print(",");
    currentFile.print(data.missionTime);
    currentFile.print(",");
    currentFile.print(data.state);
    currentFile.print(",");
    currentFile.print(data.acceleration.x);
    currentFile.print(",");
    currentFile.print(data.acceleration.y);
    currentFile.print(",");
    currentFile.print(data.acceleration.z);
    currentFile.print(",");
    currentFile.print(data.gyro.x);
    currentFile.print(",");
    currentFile.print(data.gyro.y);
    currentFile.print(",");
    currentFile.print(data.gyro.z);
    currentFile.print(",");
    currentFile.print(data.orientation.x);
    currentFile.print(",");
    currentFile.print(data.orientation.y);
    currentFile.print(",");
    currentFile.print(data.orientation.z);
    currentFile.print(",");
    currentFile.print(data.gps.pos.alt);
    currentFile.print(",");
    currentFile.print(data.gps.pos.lat);
    currentFile.print(",");
    currentFile.print(data.gps.pos.lon);
    currentFile.print(",");
    currentFile.print(data.gps.time.year);
    currentFile.print(",");
    currentFile.print(data.gps.time.month);
    currentFile.print(",");
    currentFile.print(data.gps.time.day);
    currentFile.print(",");
    currentFile.print(data.gps.time.hour);
    currentFile.print(",");
    currentFile.print(data.gps.time.minute);
    currentFile.print(",");
    currentFile.print(data.gps.time.second);
    currentFile.print(",");
    currentFile.print(data.gps.SIV);
    currentFile.print(",");
    currentFile.print(data.atmo.alt);
    currentFile.print(",");
    currentFile.print(data.atmo.pressure);
    currentFile.print(",");
    currentFile.print(data.atmo.temperature);
    currentFile.print(",");
    currentFile.print(data.atmo.humidity);
    currentFile.print(",");
    currentFile.print(data.target.mode);
    currentFile.print(",");
    currentFile.print(data.target.target);
    currentFile.print(",");
    currentFile.print(data.solenoids);
    currentFile.println();
    if (flushTimer.isComplete()) {
        errorLED.temporaryColor(colorPresets.red);
        currentFile.flush();
        errorLED.clearTemporaryColor();
        flushTimer.reset();
    }
    
}
