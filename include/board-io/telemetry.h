#pragma once
#include <data.h>
#include <SD.h>

class Logger {
public:
    virtual void init() = 0;
    virtual void writeTelemetry(Data &data) = 0;
};

class OpenLog: public Logger {
public:
    void init();
    void writeTelemetry(Data &data);
};

class SPILogger: public Logger {
private:
    File telemetryFile;
    File secondaryTelemetryFile;
    File errorMessageFile;
public:
    void init();
    void writeTelemetry(Data &data);
    void writeSecondaryTelemetry(const char * telemetry, bool newline = true, bool alwaysFlush = false);
    void writeErrorMessage(const char * error);
};